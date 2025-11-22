import json
import random


def load_graph(graph_path):
    """Load node IDs from the graph JSON."""
    with open(graph_path, "r") as f:
        data = json.load(f)
    return [node["id"] for node in data["nodes"]]


def generate_standard_query(node_ids):
    total_nodes = len(node_ids)

    # RANDOM number of orders (1 … total_nodes)
    num_orders = random.randint(1, total_nodes)

    orders = []
    for oid in range(1, num_orders + 1):
        pickup = random.choice(node_ids)
        dropoff = random.choice(node_ids)
        while dropoff == pickup:
            dropoff = random.choice(node_ids)

        orders.append({
            "order_id": oid,
            "pickup": pickup,
            "dropoff": dropoff
        })

    # RANDOM number of delivery boys
    num_delivery_guys = random.randint(1, total_nodes)

    # depot can be ANY node
    depot = random.choice(node_ids)

    query = {
        "orders": orders,
        "fleet": {
            "num_delivery_guys": num_delivery_guys,
            "depot_node": depot
        }
    }

    return query


def save_query(query, index):
    filename = f"query_{index}.json"
    with open(filename, "w") as f:
        json.dump(query, f, indent=2)
    print(f"Saved {filename}")


def main():
    graph_path = input("Enter graph JSON path: ")
    num_queries = int(input("How many queries to generate? "))

    node_ids = load_graph(graph_path)

    for i in range(1, num_queries + 1):
        q = generate_standard_query(node_ids)
        save_query(q, i)


if __name__ == "__main__":
    main()
