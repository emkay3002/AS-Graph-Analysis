#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <set>
#include <sstream>
#include "algorithms.h"
using namespace std;

Graph readGraphFromFile(const string &filePath)
{
    ifstream file(filePath);
    Graph graph;
    int u, v, w;

    while (file >> u >> v >> w)
    {
        graph[u].push_back({v, w});
        graph[v].push_back({u, w});
    }

    cout << "Graph Loaded:\n";
    for (const auto &node : graph)
    {
        cout << "Node " << node.first << ": ";
        for (const auto &edge : node.second)
        {
            cout << "(to: " << edge.to << ", weight: " << edge.weight << ") ";
        }
        cout << endl;
    }

    return graph;
}

int main()
{
    cout << "AS Graph Analysis\n";
    cout << "------------------\n";

    string datasetFile = "weighted_edges.txt";
    // string outputFile = "dijkstra_output.txt";
    Graph graph = readGraphFromFile(datasetFile);
    // unordered_map<int, vector<Edge>> graph; // when im detecting cycle
    int source;
    cout << "Enter source node: ";
    cin >> source;

    // dijkstraAlgorithm(graph, source, outputFile);

    // string bellmanOutput = "bellman_output.txt";
    // bellmanFordAlgorithm(graph, source, bellmanOutput);

    // string primsOutput = "prims_output.txt";
    // primsAlgorithm(graph, source, primsOutput);

    // string bfsOutput = "bfs_output.txt";
    // bfs(graph, source, bfsOutput);

    // string dfsRecursiveOutput = "dfs_recursive_output.txt";
    // dfs(graph, source, dfsRecursiveOutput);

    // string kruskalOutput = "kruskal_output.txt";
    // kruskalAlgorithm(graph, kruskalOutput);

    // string diameterOutput = "diameter_output.txt";
    // computeGraphDiameter(graph, diameterOutput);

    string cycleDetectionResultFile = "cycle_detection_result.txt";
    checkForCycleInGraph(graph, cycleDetectionResultFile);

    return 0;
}
