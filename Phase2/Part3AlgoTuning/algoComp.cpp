#include <fstream>
#include "Phase1/loadGraph1.hpp"
#include "process3algo.hpp"

using json = nlohmann::json;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <graph.json> <queries_with_truth.json>" << std::endl;
        return 1;
    }

    Graph g = loadGraph_parse(argv[1]);
    std::ifstream qfile(argv[2]);
    json qjson;
    qfile >> qjson;
    qfile.close();

    std::vector<double> w_values = {0.5, 0.75 ,1.0, 1.2, 1.5, 2.0};
    json summary;

    for (double w : w_values) {
        double total_err1 = 0, total_err2 = 0, total_err3 = 0;
        double total_time1 = 0, total_time2 = 0, total_time3 = 0;
        int count = 0,count1 =0, count2 =0, count3 =0;

        for (auto& event : qjson["events"]) {
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

            // compute error %
            for (int i = 0; i < event["queries"].size(); i++) {
                double truth = event["queries"][i]["true_distance"];
                if (truth <= 0) continue;

                double d1 = res1["distances"][i]["approx_shortest_distance"];
                double d2 = res2["distances"][i]["approx_shortest_distance"];
                double d3 = res3["distances"][i]["approx_shortest_distance"];

                double acc_err = event["acceptable_error_pct"];
                double err1 = std::abs(d1 - truth) / truth * 100.0;
                double err2 = std::abs(d2 - truth) / truth * 100.0;
                double err3 = std::abs(d3 - truth) / truth * 100.0;
                if(err1<=acc_err) count1++;
                if(err2<=acc_err) count2++;
                if(err3<=acc_err) count3++;
                total_err1 += err1;
                total_err2 += err2;
                total_err3 += err3;
                count++;
            }
        }

        summary["w=" + std::to_string(w)] = {
            {"algo1_acc_queries", count1 / 1},
            {"algo2_acc_queries", count2 / 1},
            {"algo3_acc_queries", count3 / 1},
            {"algo1_avg_error_pct", total_err1 / count},
            {"algo2_avg_error_pct", total_err2 / count},
            {"algo3_avg_error_pct", total_err3 / count},
            {"algo1_total_time_ms", total_time1 / 1 },
            {"algo2_total_time_ms", total_time2 / 1 },
            {"algo3_total_time_ms", total_time3 / 1 },
            {"algo1_avg_time_ms", total_time1 / qjson["events"].size()},
            {"algo2_avg_time_ms", total_time2 / qjson["events"].size()},
            {"algo3_avg_time_ms", total_time3 / qjson["events"].size()}
        };
    }

    std::ofstream out("performance_table.json");
    out << summary.dump(4);
    out.close();
    std::cout << "✅ Results saved to performance_table.json\n";
    return 0;
}
