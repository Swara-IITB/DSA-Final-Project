import osmnx as ox
import networkx as nx
import random, json

# predefined list of POI types
POI_TYPES = ["restaurant", "hospital", "pharmacy", "atm", "hotel", "petrol station"]

def generate_speed_profile():
    """Generate a realistic 96-value speed profile (km/h)."""
    base = random.uniform(40, 70)
    return [round(base + random.uniform(-10, 10), 1) for _ in range(96)]

def extract_graph(place, n_nodes, connected=True):
    """Extract a graph from OpenStreetMap and sample or subgraph nodes."""
    print(f" Extracting map data for: {place} ...")

    # Step 1: fetch base drivable graph
    G = ox.graph_from_place(place, network_type="drive", simplify=True)
    G = nx.Graph(G)  # make undirected for our phase1 format

    # Step 2: ensure we work only on the largest connected component
    largest_cc = max(nx.connected_components(G), key=len)
    G = G.subgraph(largest_cc).copy()

    # Step 3: try to find a dense, small subgraph around a random area
    best_sub = None
    max_edges = 0

    for _ in range(10):  # try multiple random regions
        center = random.choice(list(G.nodes))
        radius = 300
        while radius <= 4000:  # gradually expand until enough nodes
            sub = nx.ego_graph(G, center, radius=radius, distance="length")
            if len(sub.nodes) >= n_nodes and len(sub.edges) > max_edges:
                best_sub = sub.copy()
                max_edges = len(sub.edges)
            if len(sub.nodes) >= n_nodes and len(sub.edges) >= n_nodes:
                break
            radius *= 2

    # fallback in case we didn’t find a dense enough region
    if best_sub is None:
        print("⚠️ No dense subgraph found, using largest connected component instead.")
        best_sub = G

    # Step 4: handle connected/disconnected modes
    if connected:
        # ✅ Connected mode — ensure full connectivity
        if not nx.is_connected(best_sub):
            best_sub = best_sub.subgraph(max(nx.connected_components(best_sub), key=len)).copy()

        # if the graph is larger than required, trim it carefully but stay connected
        if len(best_sub.nodes) > n_nodes:
            start = random.choice(list(best_sub.nodes))
            bfs_nodes = list(nx.bfs_tree(best_sub, source=start, depth_limit=n_nodes).nodes())
            best_sub = best_sub.subgraph(bfs_nodes[:n_nodes]).copy()

            # final connectivity cleanup
            if not nx.is_connected(best_sub):
                best_sub = best_sub.subgraph(max(nx.connected_components(best_sub), key=len)).copy()
    else:
        # ❌ Disconnected mode — just random subset
        sub_nodes = random.sample(list(G.nodes), min(n_nodes, len(G.nodes)))
        best_sub = G.subgraph(sub_nodes).copy()

    print(f"✅ Extracted {len(best_sub.nodes)} nodes and {len(best_sub.edges)} edges.")
    return best_sub



def build_json(G, place):
    """Convert NetworkX graph to project-compliant JSON (0-based node IDs, flexible POIs)."""

    # map original OSM node IDs -> 0-based IDs
    node_map = {old_id: new_id for new_id, old_id in enumerate(G.nodes())}

    nodes_list = []
    for new_id, (node, data) in enumerate(G.nodes(data=True)):
        lat = data.get("y", 0)
        lon = data.get("x", 0)

        node_entry = {
            "id": new_id,
            "lat": round(lat, 6),
            "lon": round(lon, 6)
        }

        # 1/3 chance missing, 1/3 empty, 1/3 filled
        mode = random.choice(["missing", "empty", "filled"])
        if mode == "empty":
            node_entry["pois"] = []
        elif mode == "filled":
            pois = random.sample(POI_TYPES, random.randint(1, 3))
            node_entry["pois"] = pois

        nodes_list.append(node_entry)

    # Edges with remapped node IDs
    edges_list = []
    for eid, (u, v, data) in enumerate(G.edges(data=True), start=1):
        oneway = random.choice([True, False])
        road_type = random.choice(["primary", "secondary", "tertiary", "local", "expressway"])
        has_speed_profile = random.choice([True, False])

        edge_data = {
            "id": 1000 + eid,
            "u": node_map[u],
            "v": node_map[v],
            "length": round(data.get("length", random.uniform(100, 800)), 2),
            "average_time": round(random.uniform(10, 50), 2),
            "oneway": oneway,
            "road_type": road_type,
        }

        if has_speed_profile:
            edge_data["speed_profile"] = generate_speed_profile()

        edges_list.append(edge_data)

    graph_json = {
        "meta": {
            "id": f"graph_osm_{place.lower().replace(',', '').replace(' ', '_')}",
            "nodes": len(nodes_list),
            "description": f"Auto-generated graph for {place}"
        },
        "nodes": nodes_list,
        "edges": edges_list
    }

    return graph_json


def main():
    print("🌍 === OSM to Graph.JSON Generator ===")
    place = input("Enter a place name (e.g. Paris, France): ") or "Paris, France"
    n_nodes = int(input("Enter number of nodes (e.g. 20): ") or 20)
    connected_choice = input("Should the graph be fully connected? (y/n): ").strip().lower()
    connected = connected_choice == "y"

    G = extract_graph(place, n_nodes, connected)
    graph_json = build_json(G, place)

    with open("graph.json", "w") as f:
        json.dump(graph_json, f, indent=2)

    print("✅ graph.json created successfully!")
    print(f"📄 Contains {len(graph_json['nodes'])} nodes and {len(graph_json['edges'])} edges.")


if __name__ == "__main__":
    main()
