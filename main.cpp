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

double calculateAverageDegree(const Graph &graph)
{
    int totalEdges = 0;
    int totalNodes = graph.size();

    // Count the number of edges
    for (const auto &node : graph)
    {
        totalEdges += node.second.size(); // degree of node
    }

    // Since each edge is counted twice (once for each node), divide by 2
    return static_cast<double>(totalEdges) / totalNodes;
}

void collectDetailedExecutionTimesToCSV(
    const vector<tuple<string, string>> &algoOutputFiles, // {algorithm, file}
    const Graph &graph,
    const string &graphType,
    const string &csvFileName)
{
    ofstream csv(csvFileName);
    csv << "Algorithm,Nodes,Edges,Type,Time(ms)\n";

    int nodes = graph.size();
    int edges = 0;
    for (const auto &entry : graph)
        edges += entry.second.size();
    edges /= 2; // Undirected edge count

    for (const auto &[algorithm, fileName] : algoOutputFiles)
    {
        ifstream file(fileName);
        if (!file.is_open())
        {
            cerr << "Cannot open " << fileName << endl;
            continue;
        }

        string line;
        string time = "N/A";
        while (getline(file, line))
        {
            if (line.find("Execution Time") != string::npos)
            {
                size_t pos = line.find(":");
                if (pos != string::npos)
                {
                    time = line.substr(pos + 1);
                    break;
                }
            }
        }

        file.close();
        csv << algorithm << "," << nodes << "," << edges << "," << graphType << "," << time << "\n";
    }

    csv.close();
    cout << "Detailed execution time CSV created at " << csvFileName << endl;
}

int getEdgeCount(const Graph &graph)
{
    int total = 0;
    for (const auto &node : graph)
        total += node.second.size();
    return total / 2; // since undirected
}

int main()
{
    cout << "AS Graph Analysis\n";
    cout << "------------------\n";

    string datasetFile = "weighted_edges.txt";
    // string outputFile = "dijkstra_output.txt";
    Graph graph = readGraphFromFile(datasetFile);
    // unordered_map<int, vector<Edge>> graph; // when im detecting cycle

    // Calculate and print the average degree
    // double avgDegree = calculateAverageDegree(graph);
    // cout << "Average Degree of the Graph: " << avgDegree << endl;
    // int source;
    // cout << "Enter source node: ";
    // cin >> source;

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

    // ignore it asking for source node, it does nothing
    // string cycleDetectionResultFile = "cycle_detection_result.txt";
    // checkForCycleInGraph(graph, cycleDetectionResultFile);

    vector<tuple<string, string>> outputs = {
        {"Dijkstra", "dijkstra_output.txt"},
        {"BellmanFord", "bellman_output.txt"},
        {"Prims", "prims_output.txt"},
        {"Kruskal", "kruskal_output.txt"},
        {"BFS", "bfs_output.txt"},
        {"DFS", "dfs_recursive_output.txt"},
        {"Diameter", "diameter_output.txt"},
        {"CycleDetection", "cycle_detection_result.txt"}};

    int edges = getEdgeCount(graph);
    string graphType = "Undirected-Weighted";
    collectDetailedExecutionTimesToCSV(outputs, graph, graphType, "execution_times_detailed.csv");

    return 0;
}
