from collections import deque

def bfs_shortest_path(graph, start, end):
    if start not in graph or end not in graph:
        return None, [f"Error: '{start}' atau '{end}' tidak ditemukan dalam peta."]

    queue = deque([[start]])  # Store paths instead of just nodes
    log = []                  # Store visited nodes log

    while queue:
        path = queue.popleft()  # Get the first path from the queue
        node = path[-1]         # Get the last node in the path

        # Simpan log kunjungan node
        log.append(f"Mengunjungi: {node}")

        if node == end:  # Jika sampai tujuan, return shortest path
            return path, log  

        for neighbor in graph.get(node, []):  # Ambil tetangga dari node saat ini
            if neighbor not in path:  # Hindari siklus
                new_path = path + [neighbor]
                queue.append(new_path)

    return None, log  # Jika tidak ada jalur yang ditemukan

# Graph (Peta)
graph = {
    "Arad": ["Timisoara", "Sibiu", "Zerind"],
    "Timisoara": ["Lugoj"],
    "Lugoj": ["Mehadia"],
    "Mehadia": ["Dobreta"],
    "Dobreta": ["Craiova"],
    "Sibiu": ["Rimicu Vilea", "Fagaras"],
    "Rimicu Vilea": ["Craiova", "Pitesti"],
    "Craiova": ["Dobreta"],
    "Pitesti": ["Craiova"],
    "Fagaras": ["Bucharest"],
    "Bucharest": ["Giurgiu", "Urziceni"],
    "Giurgiu": [],
    "Urziceni": ["Hirsova", "Vaslui"],
    "Hirsova": ["Eforie"],
    "Eforie": [],
    "Vaslui": ["Lasi"],
    "Lasi": ["Neamt"],
    "Neamt": [],
    "Zerind": ["Oradea"],
    "Oradea": ["Sibiu"]
}

# Menerima input dari pengguna
start_node = input("Masukkan kota asal: ").strip()
end_node = input("Masukkan kota tujuan: ").strip()

# Jalankan BFS untuk jalur terpendek
shortest_path, log = bfs_shortest_path(graph, start_node, end_node)

# Cetak hasil
print(f"\nShortest path from {start_node} to {end_node}:")
if shortest_path:
    print(" -> ".join(shortest_path))
else:
    print("No path found.")

# Cetak log kunjungan node
print("\nBFS Visited Nodes:")
print(", ".join(log))