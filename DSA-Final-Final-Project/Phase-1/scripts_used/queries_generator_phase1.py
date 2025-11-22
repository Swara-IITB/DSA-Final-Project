import json
import random

POI_TYPES = ["restaurant", "hospital",
             "pharmacy", "atm", "hotel", "petrol station"]


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
    """Count POIs in the graph."""
    poi_counts = {poi: 0 for poi in POI_TYPES}
    for n in nodes:
        if "pois" in n:
            for p in n["pois"]:
                if p in poi_counts:
                    poi_counts[p] += 1
    return poi_counts


def generate_random_patch():
    """Random patch for modify_edge."""
    fields = ["length", "average_time", "road_type", "speed_profile"]

    # 25% chance the patch is empty
    if random.random() < 0.25:
        return {}

    selected = random.sample(fields, random.randint(1, len(fields)))
    patch = {}

    for f in selected:
        if f == "length":
            patch["length"] = round(random.uniform(50, 800), 2)
        elif f == "average_time":
            patch["average_time"] = round(random.uniform(5, 60), 2)
        elif f == "road_type":
            patch["road_type"] = random.choice(
                ["primary", "secondary", "tertiary", "local", "expressway"]
            )
        elif f == "speed_profile":
            patch["speed_profile"] = [
                round(random.uniform(30, 80), 1) for _ in range(96)]

    return patch


def generate_queries(graph_data, n_queries=10):
    """Generate random Phase 1 query events."""
    nodes = graph_data["nodes"]
    edges = graph_data["edges"]

    n_nodes = len(nodes)
    if n_nodes < 2:
        print(
            f"❌ Cannot generate queries: graph has only {n_nodes} node(s). Need at least 2.")
        return None

    edge_ids = [edge["id"] for edge in edges]
    poi_counts = count_pois(nodes)

    events = []
    event_id = 1

    query_types = ["shortest_path", "knn", "remove_edge", "modify_edge"]

    for _ in range(n_queries):
        q_type = random.choice(query_types)
        event = {"id": event_id, "type": q_type}

        # ---- SHORTEST PATH ----
        if q_type == "shortest_path":
            src, tgt = random.sample(range(n_nodes), 2)
            mode = random.choice(["distance", "time"])
            event.update({"source": src, "target": tgt, "mode": mode})

        # ---- KNN ----
        elif q_type == "knn":
            poi = random.choice(POI_TYPES)
            total_poi = poi_counts.get(poi, 0)
            k = random.randint(1, 10)
            k = min(k, total_poi) if total_poi > 0 else 0
            qp = random_query_point(nodes)
            metric = random.choice(["euclidean", "shortest_path"])
            event.update({
                "poi": poi.lower(),
                "query_point": qp,
                "k": k,
                "metric": metric
            })

        # ---- REMOVE EDGE ----
        elif q_type == "remove_edge":
            if edge_ids:
                event["edge_id"] = random.choice(edge_ids)
            else:
                continue

        # ---- MODIFY EDGE ----
        elif q_type == "modify_edge":
            if edge_ids:
                eid = random.choice(edge_ids)
                event.update(
                    {"edge_id": eid, "patch": generate_random_patch()})
            else:
                continue

        events.append(event)
        event_id += random.randint(1, 3)

    return {"meta": {"id": f"queries_for_{graph_data['meta']['id']}"}, "events": events}


def main():
    print("🧠 === Random Query Generator (Phase 1) ===")

    graph_file = input(
        "Enter graph filename (default: graph.json): ").strip() or "graph.json"

    try:
        with open(graph_file, "r") as f:
            graph_data = json.load(f)
    except FileNotFoundError:
        print(f"❌ File '{graph_file}' not found!")
        return

    if len(graph_data.get("nodes", [])) < 2:
        print(
            f"❌ Graph '{graph_file}' has insufficient nodes (<2). Cannot generate queries.")
        return

    n_queries = int(
        input("Enter number of queries to generate (default 10): ") or 10)

    queries_json = generate_queries(graph_data, n_queries)
    if queries_json is None:
        return

    with open("queries.json", "w") as f:
        json.dump(queries_json, f, indent=2)

    print(
        f"✅ queries.json created successfully with {len(queries_json['events'])} events!")
    print(f"📄 Based on graph: {graph_data['meta']['id']}")


if __name__ == "__main__":
    main()
