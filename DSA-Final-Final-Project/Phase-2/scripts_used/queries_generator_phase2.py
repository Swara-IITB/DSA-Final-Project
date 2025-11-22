import json
import random

def generate_phase2_queries(graph_data, n_queries=10):
    """Generate random Phase 2 queries (kSP exact, heuristic, and approximate shortest paths)."""
    n_nodes = len(graph_data["nodes"])

    events = []
    event_id = 1

    # define balanced type ratios
    type_choices = (
        ["k_shortest_paths"] * 4 +
        ["k_shortest_paths_heuristic"] * 4 +
        ["approx_shortest_path"] * 2
    )

    for _ in range(n_queries):
        q_type = random.choice(type_choices)
        event = {"id": event_id, "type": q_type}

        # 1️⃣ K Shortest Paths (Exact)
        if q_type == "k_shortest_paths":
            src, tgt = random.sample(range(n_nodes), 2)
            k = random.randint(2, 20)
            # fixed to "distance" as per Phase 2 spec
            event.update({
                "source": src,
                "target": tgt,
                "k": k,
                "mode": "distance"
            })

        # 2️⃣ K Shortest Paths (Heuristic)
        elif q_type == "k_shortest_paths_heuristic":
            src, tgt = random.sample(range(n_nodes), 2)
            k = random.randint(2, 7)
            overlap = random.randint(50, 90)
            event.update({
                "source": src,
                "target": tgt,
                "k": k,
                "overlap_threshold": overlap
            })

        # 3️⃣ Approximate Shortest Path
        elif q_type == "approx_shortest_path":
            num_pairs = random.randint(3, 15)
            queries = []
            for _ in range(num_pairs):
                src, tgt = random.sample(range(n_nodes), 2)
                queries.append({"source": src, "target": tgt})

            error_pct = random.choice([5.0, 10.0, 15.0])

            # dynamic scaling logic based on accuracy vs time
            if error_pct == 5.0:
                base = random.randint(15, 30)
                time_budget_ms = int(base + num_pairs * 1.2)
            elif error_pct == 10.0:
                base = random.randint(8, 20)
                time_budget_ms = int(base + num_pairs * 0.8)
            else:  # 15% error
                base = random.randint(3, 10)
                time_budget_ms = int(base + num_pairs * 0.5)

            event.update({
                "queries": queries,
                "time_budget_ms": time_budget_ms,
                "acceptable_error_pct": error_pct
            })

        # irregular ID increments for realism
        events.append(event)
        event_id += random.randint(1, 3)

    # metadata block
    queries_json = {
        "meta": {
            "id": f"phase2_queries_{graph_data['meta']['id']}"
        },
        "events": events
    }
    return queries_json


def main():
    print("⚙️ === Phase 2 Queries Generator ===")
    graph_file = input("Enter graph filename (e.g. graph_tokyo.json): ").strip()

    try:
        with open(graph_file, "r") as f:
            graph_data = json.load(f)
    except FileNotFoundError:
        print(f"❌ File '{graph_file}' not found.")
        return

    n_queries = int(input("Enter number of queries to generate (default 10): ") or 10)
    queries_json = generate_phase2_queries(graph_data, n_queries)

    # output file naming: graph_x.json → queries_x.json
    if graph_file.startswith("graph_"):
        queries_filename = "queries_phase2_" + graph_file[len("graph_"):]
    else:
        queries_filename = "queries_phase2_" + graph_file

    with open(queries_filename, "w") as f:
        json.dump(queries_json, f, indent=2)

    print(f"✅ {queries_filename} created successfully with {len(queries_json['events'])} events!")


if __name__ == "__main__":
    main()
