import osmnx as ox
import random
import json
import networkx as nx

def extract_graph(place_name="Pune, Mumbai", num_nodes=20, edge_density=0.4):
    print(f"Extracting map data for: {place_name} ...")
    G = ox.graph_from_place(place_name, network_type="drive")
    G = ox.simplify_graph(G)
    
    # Take a random subset of nodes
    nodes = list(G.nodes)
    if num_nodes < len(nodes):
        selected = random.sample(nodes, num_nodes)
        G = G.subgraph(selected).copy()
    else:
        selected = nodes
    
    # Prune edges based on density
    edges = list(G.edges(data=True))
    keep = random.sample(edges, int(len(edges)*edge_density)) if len(edges) > 0 else []
    
    # Prepare JSON structure
    json_nodes = []
    for nid, data in G.nodes(data=True):
        json_nodes.append({
            "id": int(nid),
            "lat": round(data["y"], 6),
            "lon": round(data["x"], 6),
            "pois": random.sample(["restaurant", "atm", "hotel", "hospital", "pharmacy", "petrol station"], k=random.randint(0, 2))
        })

    json_edges = []
    eid = 1000
    for u, v, data in keep:
        json_edges.append({
            "id": eid,
            "u": int(u),
            "v": int(v),
            "length": round(data.get("length", random.uniform(100, 500)), 2),
            "average_time": round(random.uniform(10, 50), 2),
            "oneway": bool(data.get("oneway", random.choice([True, False]))),
            "road_type": random.choice(["primary", "secondary", "tertiary", "local", "expressway"]),
            "speed_profile": [round(random.uniform(40, 80), 1) for _ in range(96)] if random.random() < 0.3 else None
        })
        eid += 1

    graph_json = {
        "meta": {
            "id": f"graph_osm_{place_name.replace(',', '').replace(' ', '_').lower()}",
            "nodes": len(json_nodes),
            "description": f"Auto-generated graph for {place_name}"
        },
        "nodes": json_nodes,
        "edges": json_edges
    }

    # Remove None speed_profiles for cleanliness
    for e in graph_json["edges"]:
        if e["speed_profile"] is None:
            del e["speed_profile"]

    return graph_json

if __name__ == "__main__":
    place = input("Enter a place name (e.g. Paris, France): ").strip() or "Paris, France"
    n = int(input("Enter number of nodes (e.g. 20): ") or 20)
    d = float(input("Enter edge density (0–1, e.g. 0.5): ") or 0.5)

    result = extract_graph(place, n, d)
    with open("graph.json", "w") as f:
        json.dump(result, f, indent=2)
    print("graph.json created successfully!")
