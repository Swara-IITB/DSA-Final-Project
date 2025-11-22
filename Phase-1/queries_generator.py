import json
import random

POI_TYPES = ["restaurant", "hospital", "pharmacy", "atm", "hotel", "petrol station"]

def random_query_point(nodes):
    """Generate random lat/lon within graph bounding box."""
    lats = [n["lat"] for n in nodes if "lat" in n]
    lons = [n["lon"] for n in nodes if "lon" in n]
    if not lats or not lons:
        return {"lat": 0.0, "lon": 0.0}
    lat = random.uniform(min(lats), max(lats))
    lon = random.uniform(min(lons), max(lons))
    return {"lat": round(lat, 6), "lon": round(lon, 6)}

def count_pois(nodes):
    """Count how many nodes contain each POI type."""
    poi_counts = {poi: 0 for poi in POI_TYPES}
    for n in nodes:
        if "pois" in n:
            for p in n["pois"]:
                if p in poi_counts:
                    poi_counts[p] += 1
    return poi_counts

def generate_queries(graph_data, n_queries=10):
    """Generate random query events based on the graph."""
    n_nodes = len(graph_data["nodes"])
    n_edges = len(graph_data["edges"])
    edge_ids = [edge["id"] for edge in graph_data["edges"]]
    nodes = graph_data["nodes"]
    poi_counts = count_pois(nodes)

    events = []
    event_id = 1

    for _ in range(n_queries):
        q_type = random.choice(["shortest_path", "knn", "remove_edge"])
        event = {"id": event_id, "type": q_type}

        if q_type == "shortest_path":
            # Randomly pick source and target nodes (not the same)
            src, tgt = random.sample(range(n_nodes), 2)
            mode = random.choice(["distance", "time"])
            event.update({
                "source": src,
                "target": tgt,
                "mode": mode
            })

        elif q_type == "knn":
            poi = random.choice(POI_TYPES)
            total_available = poi_counts.get(poi, 0)
            k = random.randint(1, 10)  # any k, not restricted to 3
            if total_available > 0:
                k = min(k, total_available)
            else:
                k = 0  # if no POIs of that type exist
            metric = random.choice(["euclidean", "shortest_path"])
            query_point = random_query_point(nodes)
            event.update({
                "poi": poi.lower(),
                "query_point": query_point,
                "k": k,
                "metric": metric
            })

        elif q_type == "remove_edge":
            if edge_ids:
                event["edge_id"] = random.choice(edge_ids)
            else:
                continue  # skip if graph has no edges

        events.append(event)
        event_id += random.randint(1, 3)  # irregular numbering for realism

    # Metadata section
    queries_json = {
        "meta": {
            "id": f"queries_for_{graph_data['meta']['id']}"
        },
        "events": events
    }
    return queries_json


def main():
    print("🧠 === Random Query Generator ===")

    # Always load 'graph.json' from current directory
    try:
        with open("graph.json", "r") as f:
            graph_data = json.load(f)
    except FileNotFoundError:
        print("❌ graph.json not found in current directory!")
        return

    try:
        n_queries = int(input("Enter number of queries to generate (default 10): ") or 10)
    except ValueError:
        n_queries = 10

    queries_json = generate_queries(graph_data, n_queries)

    with open("queries.json", "w") as f:
        json.dump(queries_json, f, indent=2)

    print(f"✅ queries.json created successfully with {len(queries_json['events'])} events!")
    print(f"📄 Based on graph: {graph_data['meta']['id']}")


if __name__ == "__main__":
    main()
