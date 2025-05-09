import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Load your CSV file
df = pd.read_csv("execution_times_detailed.csv")

# Clean up and convert time to float
df['Time(seconds)'] = df['Time(ms)'].str.replace(' seconds', '', regex=False).astype(float)

# Set plot style
sns.set(style="whitegrid")

def plot_comparison(df, algorithms, graph_type_filter, title, filename):
    """
    Generates a line plot comparing given algorithms under a specific graph type.
    
    :param df: DataFrame containing data
    :param algorithms: list of algorithm names to compare
    :param graph_type_filter: string to filter 'Type' column
    :param title: Title of the graph
    :param filename: Output filename to save the plot
    """
    filtered = df[(df['Algorithm'].isin(algorithms)) & (df['Type'] == graph_type_filter)]
    
    plt.figure(figsize=(10, 6))
    sns.lineplot(data=filtered, x='Nodes', y='Time(seconds)', hue='Algorithm', marker='o')
    plt.title(title)
    plt.xlabel("Number of Nodes")
    plt.ylabel("Execution Time (seconds)")
    plt.tight_layout()
    plt.savefig(filename)
    plt.show()

# Example 1: Dijkstra vs Bellman-Ford (Undirected-Weighted)
plot_comparison(df,
                algorithms=['Dijkstra', 'BellmanFord'],
                graph_type_filter='Undirected-Weighted',
                title='Dijkstra vs Bellman-Ford (Undirected-Weighted)',
                filename='dijkstra_vs_bellman.png')

# Example 2: Prim vs Kruskal (Undirected-Weighted)
plot_comparison(df,
                algorithms=['Prims', 'Kruskal'],
                graph_type_filter='Undirected-Weighted',
                title='Prim vs Kruskal (Undirected-Weighted)',
                filename='prim_vs_kruskal.png')

# Add more calls like the ones above for:
# - Sparse vs Dense
# - Directed vs Undirected
# - Weighted vs Unweighted
