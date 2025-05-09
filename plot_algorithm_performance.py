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

# Function to measure execution time for Prim's Algorithm
def measure_prims(graph):
    start = time.time()
    nx.minimum_spanning_tree(graph)
    end = time.time()
    return end - start

# Function to measure execution time for Kruskal's Algorithm
def measure_kruskal(graph):
    start = time.time()
    list(nx.minimum_spanning_edges(graph, data=False))
    end = time.time()
    return end - start

# Function to measure execution time for BFS
def measure_bfs(graph, source):
    start = time.time()
    nx.bfs_edges(graph, source)
    end = time.time()
    return end - start

# Function to measure execution time for DFS
def measure_dfs(graph, source):
    start = time.time()
    nx.dfs_edges(graph, source)
    end = time.time()
    return end - start

# Function to measure execution time for Cycle Detection
def measure_cycle_detection(graph):
    start = time.time()
    cycle = nx.find_cycle(graph, orientation='ignore')
    end = time.time()
    return end - start

# Function to measure the graph's diameter
def measure_diameter(graph):
    if nx.is_connected(graph):  # Check if the graph is connected
        diameter = nx.diameter(graph)
        return diameter
    else:
        return None  # Return None or any other value indicating the graph is not connected

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
                    prims_time = measure_prims(graph)
                    kruskal_time = measure_kruskal(graph)
                    bfs_time = measure_bfs(graph, source)
                    dfs_time = measure_dfs(graph, source)
                    cycle_detection_time = measure_cycle_detection(graph)
                    diameter_time = measure_diameter(graph)

                    # Save results to CSV for each algorithm
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

                    result = {
                        'Algorithm': 'Prim\'s',
                        'Nodes': nodes,
                        'Edges': 2 * nodes,
                        'Graph Type': f'{graph_type}-{density}-{ "Weighted" if weighted else "Unweighted" }',
                        'Execution Time': prims_time * 1000  # Convert to ms
                    }
                    results.append(result)

                    result = {
                        'Algorithm': 'Kruskal\'s',
                        'Nodes': nodes,
                        'Edges': 2 * nodes,
                        'Graph Type': f'{graph_type}-{density}-{ "Weighted" if weighted else "Unweighted" }',
                        'Execution Time': kruskal_time * 1000  # Convert to ms
                    }
                    results.append(result)

                    result = {
                        'Algorithm': 'BFS',
                        'Nodes': nodes,
                        'Edges': 2 * nodes,
                        'Graph Type': f'{graph_type}-{density}-{ "Weighted" if weighted else "Unweighted" }',
                        'Execution Time': bfs_time * 1000  # Convert to ms
                    }
                    results.append(result)

                    result = {
                        'Algorithm': 'DFS',
                        'Nodes': nodes,
                        'Edges': 2 * nodes,
                        'Graph Type': f'{graph_type}-{density}-{ "Weighted" if weighted else "Unweighted" }',
                        'Execution Time': dfs_time * 1000  # Convert to ms
                    }
                    results.append(result)

                    result = {
                        'Algorithm': 'Cycle Detection',
                        'Nodes': nodes,
                        'Edges': 2 * nodes,
                        'Graph Type': f'{graph_type}-{density}-{ "Weighted" if weighted else "Unweighted" }',
                        'Execution Time': cycle_detection_time * 1000  # Convert to ms
                    }
                    results.append(result)

                    # Handle the case when the graph is not connected for Diameter
                    if diameter_time is not None:
                        result = {
                            'Algorithm': 'Diameter',
                            'Nodes': nodes,
                            'Edges': 2 * nodes,
                            'Graph Type': f'{graph_type}-{density}-{ "Weighted" if weighted else "Unweighted" }',
                            'Execution Time': diameter_time * 1000  # Convert to ms
                        }
                        results.append(result)

    # Write results to CSV
    with open('algorithm_comparison.csv', mode='w', newline='') as file:
        writer = csv.DictWriter(file, fieldnames=['Algorithm', 'Nodes', 'Edges', 'Graph Type', 'Execution Time'])
        writer.writeheader()
        writer.writerows(results)

# Step 2: Generate plots
# Step 2: Generate plots with multiple pages or groups of plots for better readability
def generate_plots():
    # Read the CSV file to analyze the results
    data = pd.read_csv('algorithm_comparison.csv')
    
    # Standardize algorithm names
    data['Algorithm'] = data['Algorithm'].replace({
        'BellmanFord': 'Bellman-Ford',
        'Dijkstra': 'Dijkstra',
        'Prims': 'Prim\'s',
        'Kruskal': 'Kruskal\'s',
        'BFS': 'BFS',
        'DFS': 'DFS',
        'Diameter': 'Diameter',
        'CycleDetection': 'Cycle Detection'
    })
    
    algorithms = data['Algorithm'].unique()
    
    # Define the number of plots per page (1 or 2 per page)
    plots_per_page = 2  # You can change this value to 1 or 2
    
    for start_idx in range(0, len(algorithms), plots_per_page):
        # Create a new figure for the current set of plots
        plt.figure(figsize=(15, 10))
        
        # Iterate through the algorithms for this page
        for i, algorithm in enumerate(algorithms[start_idx:start_idx + plots_per_page]):
            # Filter data for the current algorithm
            algorithm_data = data[data['Algorithm'] == algorithm]
            
            # Create a subplot for each algorithm
            plt.subplot(1, plots_per_page, i + 1)
            
            # Plot the data (using lineplot with different hues for graph types)
            sns.lineplot(x='Nodes', y='Execution Time', data=algorithm_data, hue='Graph Type', marker='o')
            
            # Set titles and labels
            plt.title(f'{algorithm} Execution Time')
            plt.xlabel('Number of Nodes')
            plt.ylabel('Execution Time (ms)')
            plt.legend(title='Graph Type', bbox_to_anchor=(1.05, 1), loc='upper left')
        
        # Adjust layout for better spacing between subplots
        plt.tight_layout()
        
        # Show the current page of plots
        plt.show()

# Running the comparison and generating plots
compare_algorithms()
generate_plots()

