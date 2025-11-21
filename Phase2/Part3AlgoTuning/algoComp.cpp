#include <fstream>
#include "../../Phase1/loadGraph1.hpp"
#include "process3algo.hpp"

using json = nlohmann::json;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <graph.json> <queries_with_truth.json>" << std::endl;
        return 1;
    }

    Graph g = loadGraph_parse(argv[1]);

    std::ifstream qfile(argv[2]);
    if (!qfile.is_open()) {
        std::cerr << "Error: could not open " << argv[2] << std::endl;
        return 1;
    }

    json qjson;
    qfile >> qjson;
    qfile.close();

    std::vector<double> w_values = {0.5, 0.75, 1.0, 1.2, 1.5, 2.0};
    json summary;

    // For each event in the input JSON
    for (size_t e = 0; e < qjson["events"].size(); ++e) {
        const auto& event = qjson["events"][e];
        std::string event_id;

        if (event.contains("id")) {
            if (event["id"].is_string())
                event_id = event["id"].get<std::string>();
            else if (event["id"].is_number_integer())
                event_id = std::to_string(event["id"].get<long long>());
            else
                event_id = "event_" + std::to_string(e);
        } else {
            event_id = "event_" + std::to_string(e);
        }

        for (double w : w_values) {
            double total_err1 = 0, total_err2 = 0, total_err3 = 0;
            double total_time1 = 0, total_time2 = 0, total_time3 = 0;
            int count = 0, count1 = 0, count2 = 0, count3 = 0;

            // Run the 3 algos for this event
            auto start = std::chrono::high_resolution_clock::now();
            auto res1 = process_3algo_approx1(g, event, start, w);
            auto end = std::chrono::high_resolution_clock::now();
            total_time1 += std::chrono::duration<double, std::milli>(end - start).count();

            start = std::chrono::high_resolution_clock::now();
            auto res2 = process_3algo_approx2(g, event, start, w);
            end = std::chrono::high_resolution_clock::now();
            total_time2 += std::chrono::duration<double, std::milli>(end - start).count();

            start = std::chrono::high_resolution_clock::now();
            auto res3 = process_3algo_approx3(g, event, start, w);
            end = std::chrono::high_resolution_clock::now();
            total_time3 += std::chrono::duration<double, std::milli>(end - start).count();

            // Process queries in this event
            for (int i = 0; i < event["queries"].size(); i++) {
                const auto& q = event["queries"][i];
                if (!q.contains("true_distance") || q["true_distance"].is_null()) continue;

                double truth = q["true_distance"].get<double>();
                if (truth <= 0) continue;

                double d1 = res1["distances"][i]["approx_shortest_distance"].get<double>();
                double d2 = res2["distances"][i]["approx_shortest_distance"].get<double>();
                double d3 = res3["distances"][i]["approx_shortest_distance"].get<double>();

                double acc_err = event.value("acceptable_error_pct", 5.0);
                double err1 = std::abs(d1 - truth) / truth * 100.0;
                double err2 = std::abs(d2 - truth) / truth * 100.0;
                double err3 = std::abs(d3 - truth) / truth * 100.0;

                if (err1 <= acc_err) count1++;
                if (err2 <= acc_err) count2++;
                if (err3 <= acc_err) count3++;

                total_err1 += err1;
                total_err2 += err2;
                total_err3 += err3;
                count++;
            }

            summary["w=" + std::to_string(w)] = json({
                {"valid_queries", json(count)},
                {"algo1_acc_queries", json(count1)},
                {"algo2_acc_queries", json(count2)},
                {"algo3_acc_queries", json(count3)},
                {"algo1_avg_error_pct", count ? json(total_err1 / count) : json(nullptr)},
                {"algo2_avg_error_pct", count ? json(total_err2 / count) : json(nullptr)},
                {"algo3_avg_error_pct", count ? json(total_err3 / count) : json(nullptr)},
                {"algo1_total_time_ms", json(total_time1)},
                {"algo2_total_time_ms", json(total_time2)},
                {"algo3_total_time_ms", json(total_time3)},
                {"algo1_avg_time_ms", json(total_time1 / qjson["events"].size())},
                {"algo2_avg_time_ms", json(total_time2 / qjson["events"].size())},
                {"algo3_avg_time_ms", json(total_time3 / qjson["events"].size())}
            });


        }
    }

    std::ofstream out("performance_table.json");
    out << summary.dump(4);
    out.close();
    std::cout << "✅ Results saved to performance_table.json\n";
    return 0;
}
