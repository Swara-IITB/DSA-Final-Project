import json
import random


def load_graph(graph_path):
    with open(graph_path, "r") as f:
        data = json.load(f)
    return [node["id"] for node in data["nodes"]]


def generate_penalty_query(node_ids, N_orders):

    depot = random.choice(node_ids)

    all_order_ids = list(range(1, N_orders + 1))

    num_driver_orders = random.randint(1, N_orders)
    driver_orders = random.sample(all_order_ids, num_driver_orders)

    orders_list = []
    for oid in driver_orders:
        pickup = random.choice(node_ids)
        dropoff = random.choice(node_ids)
        while dropoff == pickup:
            dropoff = random.choice(node_ids)

        max_time = random.randint(10, 120)
        penalty = random.randint(1, 20)

        orders_list.append({
            "order_id": oid,
            "pickup": pickup,
            "dropoff": dropoff,
            "max_time": max_time,
            "penalty_multiplier": penalty
        })

    query = {
        "depot_node": depot,
        "delivery_boy": {
            "id": 1,
            "orders": orders_list
        }
    }

    return query


def save_query(query, index):
    filename = f"penalty_query_{index}.json"
    with open(filename, "w") as f:
        json.dump(query, f, indent=2)
    print(f"Saved {filename}")


def main():
    graph_path = input("Enter graph JSON path: ")
    num_queries = int(input("How many penalty queries to generate? "))

    node_ids = load_graph(graph_path)
    N = len(node_ids)

    for i in range(1, num_queries + 1):
        q = generate_penalty_query(node_ids, N)
        save_query(q, i)


if __name__ == "__main__":
    main()
