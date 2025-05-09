import networkx as nx
import time
import csv
import random
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns  # Import seaborn

# Load the CSV file to verify unique algorithm names
data = pd.read_csv('algorithm_comparison.csv')
print(data['Algorithm'].unique())

# Function to generate a random graph
def generate_graph(num_nodes, num_edges, graph_type='undirected', density='sparse', weighted=False):
    if density == 'dense':
        num_edges = int(num_nodes * (num_nodes - 1) / 2 * 0.8)  # 80% of possible edges
    
    if graph_type == 'directed':
        graph = nx.erdos_renyi_graph(num_nodes, num_edges / (num_nodes * (num_nodes - 1)))
    else:
        graph = nx.erdos_renyi_graph(num_nodes, num_edges / (num_nodes * (num_nodes - 1) / 2))

    # Add weights to edges if weighted
    if weighted:
        for u, v in graph.edges():
            graph[u][v]['weight'] = random.randint(1, 10)
    
    return graph

# Function to measure execution time for Dijkstra
def measure_dijkstra(graph, source):
    start = time.time()
    nx.single_source_dijkstra(graph, source)
    end = time.time()
    return end - start

# Function to measure execution time for Bellman-Ford
def measure_bellman_ford(graph, source):
    start = time.time()
    nx.single_source_bellman_ford(graph, source)
    end = time.time()
    return end - start

# Function to run comparisons and save to CSV
def compare_algorithms():
    results = []
    node_sizes = [100, 200, 500, 1000]
    edge_density = ['sparse', 'dense']
    graph_types = ['undirected', 'directed']
    weightings = [True, False]

    for nodes in node_sizes:
        for density in edge_density:
            for graph_type in graph_types:
                for weighted in weightings:
                    graph = generate_graph(nodes, 2 * nodes, graph_type, density, weighted)
                    source = random.randint(0, nodes - 1)

                    # Measure time for each algorithm
                    dijkstra_time = measure_dijkstra(graph, source)
                    bellman_ford_time = measure_bellman_ford(graph, source)

                    # Save results to CSV
                    result = {
                        'Algorithm': 'Dijkstra',
                        'Nodes': nodes,
                        'Edges': 2 * nodes,
                        'Graph Type': f'{graph_type}-{density}-{ "Weighted" if weighted else "Unweighted" }',
                        'Execution Time': dijkstra_time * 1000  # Convert to ms
                    }
                    results.append(result)

                    result = {
                        'Algorithm': 'Bellman-Ford',
                        'Nodes': nodes,
                        'Edges': 2 * nodes,
                        'Graph Type': f'{graph_type}-{density}-{ "Weighted" if weighted else "Unweighted" }',
                        'Execution Time': bellman_ford_time * 1000  # Convert to ms
                    }
                    results.append(result)

    # Write results to CSV
    with open('algorithm_comparison.csv', mode='w', newline='') as file:
        writer = csv.DictWriter(file, fieldnames=['Algorithm', 'Nodes', 'Edges', 'Graph Type', 'Execution Time'])
        writer.writeheader()
        writer.writerows(results)

# Step 2: Generate plots
def generate_plots():
    # Read the CSV file to analyze the results
    data = pd.read_csv('algorithm_comparison.csv')
    
    # Standardize algorithm names
    data['Algorithm'] = data['Algorithm'].replace({
        'BellmanFord': 'BellmanFord',
        'Dijkstra': 'Dijkstra',
        'Prims': 'Prims',
        'Kruskal': 'Kruskal',
        'BFS': 'BFS',
        'DFS': 'DFS',
        'Diameter': 'Diameter',
        'CycleDetection': 'Cycle Detection'
    })
    
    algorithms = data['Algorithm'].unique()
    
    # Plot setup
    plt.figure(figsize=(15, 10))
    
    # Create a subplot for each algorithm
    for i, algorithm in enumerate(algorithms):
        # Filter data for the current algorithm
        algorithm_data = data[data['Algorithm'] == algorithm]
        
        # Create a subplot (adjust the number of rows/columns as needed)
        plt.subplot(3, 3, i + 1)
        
        # Plot the data (using lineplot with different hues for graph types)
        sns.lineplot(x='Nodes', y='Execution Time', data=algorithm_data, hue='Graph Type', marker='o')
        
        # Set titles and labels
        plt.title(f'{algorithm} Execution Time')
        plt.xlabel('Number of Nodes')
        plt.ylabel('Execution Time (ms)')
        plt.legend(title='Graph Type', bbox_to_anchor=(1.05, 1), loc='upper left')
    
    plt.tight_layout()  # Adjust layout for better spacing between subplots
    plt.show()

# Running the comparison and generating plots
compare_algorithms()
generate_plots()
