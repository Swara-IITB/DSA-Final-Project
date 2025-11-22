import json
import random
import networkx as nx

def load_graph_as_nx(graph_file):
    """Convert your graph.json to a NetworkX graph."""
    with open(graph_file, 'r') as f:
        data = json.load(f)

    G = nx.DiGraph()
    for e in data["edges"]:
        u, v, w = e["u"], e["v"], e["length"]
        G.add_edge(u, v, weight=w)
        if e.get("oneway") == False:
            G.add_edge(v, u, weight=w)

    return G


def generate_approx_queries(G, n_queries=20):
    """Generate random approximate shortest path queries with adaptive time budgets."""
    nodes = list(G.nodes())
    events = []

    for i in range(n_queries):
        queries = []
        num_pairs = random.randint(3, 15)
        for _ in range(num_pairs):
            src, tgt = random.sample(nodes, 2)
            queries.append({"source": src, "target": tgt})

        error_pct = random.choice([5.0, 10.0, 15.0])

        # Base time per query depends inversely on error tolerance
        if error_pct == 5.0:
            base_per_query = random.uniform(4.0, 6.0)   # tighter accuracy = more time
        elif error_pct == 10.0:
            base_per_query = random.uniform(2.0, 4.0)
        else:  # 15%
            base_per_query = random.uniform(1.0, 2.0)

        # total budget = (base_per_query × num_pairs) + random noise
        time_budget = int(base_per_query * num_pairs+ random.uniform(2.0, 5.0))

        events.append({
            "type": "approx_shortest_path",
            "id": i + 1,
            "queries": queries,
            "time_budget_ms": time_budget,
            "acceptable_error_pct": error_pct
        })

    return {"meta": {"id": "phase2_test"}, "events": events}


def add_ground_truth(G, queries_json):
    """Add exact Dijkstra distances to each query."""
    for event in queries_json["events"]:
        for q in event["queries"]:
            try:
                dist = nx.dijkstra_path_length(G, q["source"], q["target"], weight='weight')
            except nx.NetworkXNoPath:
                dist = None
            q["true_distance"] = dist
    return queries_json


def main():
    graph_file = input("Enter graph file (e.g. graph_tokyo.json): ").strip()
    n_queries = int(input("Number of queries: ") or 20)

    G = load_graph_as_nx(graph_file)
    queries_json = generate_approx_queries(G, n_queries)
    queries_json = add_ground_truth(G, queries_json)

    output_file = "queries_with_truth_test.json"
    with open(output_file, "w") as f:
        json.dump(queries_json, f, indent=2)

    print(f"✅ Created {output_file} with {len(queries_json['events'])} events and ground truth distances.")


if __name__ == "__main__":
    main()
