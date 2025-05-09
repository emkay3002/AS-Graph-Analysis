#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <set>
#include <algorithm>
#include <unordered_map>
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

bool edgeExists(const vector<Edge> &edges, int v)
{
    for (const Edge &e : edges)
    {
        if (e.to == v)
            return true;
    }
    return false;
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
    const vector<tuple<string, string>> &algoOutputFiles,
    const Graph &graph,
    const string &graphType,
    const string &csvFileName)
{
    ofstream csv(csvFileName, ios::app); // append to file for multiple runs
    if (!csv.is_open())
    {
        cerr << "Failed to open CSV file: " << csvFileName << endl;
        return;
    }

    // Add header only if the file is empty
    csv.seekp(0, ios::end);
    if (csv.tellp() == 0)
    {
        csv << "Algorithm,Nodes,Edges,Type,Time(ms)\n";
    }

    int nodes = graph.size();
    int edges = 0;
    for (const auto &entry : graph)
        edges += entry.second.size();
    edges /= 2; // undirected

    for (const auto &[algorithm, fileName] : algoOutputFiles)
    {
        ifstream file(fileName);
        if (!file.is_open())
        {
            cerr << "Cannot open " << fileName << endl;
            continue;
        }

        string line, timeMs = "N/A";

        while (getline(file, line))
        {
            if (line.find("Execution Time") != string::npos)
            {
                size_t pos = line.find(":");
                if (pos != string::npos)
                {
                    string raw = line.substr(pos + 1);
                    raw.erase(remove_if(raw.begin(), raw.end(), ::isspace), raw.end()); // remove spaces

                    // Extract number from string
                    size_t secPos = raw.find("seconds");
                    if (secPos != string::npos)
                        raw = raw.substr(0, secPos);

                    try
                    {
                        double seconds = stod(raw);
                        int milliseconds = static_cast<int>(seconds * 1000);
                        timeMs = to_string(milliseconds);
                    }
                    catch (...)
                    {
                        timeMs = "ParseError";
                    }
                }
                break;
            }
        }

        file.close();
        csv << algorithm << "," << nodes << "," << edges << "," << graphType << "," << timeMs << "\n";
    }

    csv.close();
    cout << "Execution times written to " << csvFileName << endl;
}

int getEdgeCount(const Graph &graph)
{
    int total = 0;
    for (const auto &node : graph)
        total += node.second.size();
    return total / 2; // since undirected
}

// Graph generateGraph(int nodes, int edges, bool weighted, bool directed, bool sparse)
// {
//     Graph graph;

//     // Use basic random number generation
//     srand(time(0)); // Initialize random seed based on time

//     // Add edges (adjust for sparse or dense graph)
//     int edgeCount = 0;
//     while (edgeCount < edges)
//     {
//         int u = rand() % nodes; // Random node
//         int v = rand() % nodes; // Random node

//         // Avoid self-loops and duplicate edges
//         if (u != v && std::find(graph[u].begin(), graph[u].end(), v) == graph[u].end())
//         {
//             graph[u].push_back({v, weighted ? rand() % 100 + 1 : 0}); // Random weight if weighted
//             if (!directed)
//             {
//                 graph[v].push_back({u, weighted ? rand() % 100 + 1 : 0}); // Add reverse edge for undirected
//             }
//             edgeCount++;

//             // Adjust for sparse/dense graph density
//             if (sparse && edgeCount > edges)
//                 break; // Sparse graphs should stop at the desired edge count
//         }
//     }

//     return graph;
// }

Graph generateGraph(int nodes, int edges, bool weighted, bool directed, bool sparse)
{
    Graph graph;

    srand(time(0)); // Initialize random seed based on time

    int edgeCount = 0;
    while (edgeCount < edges)
    {
        int u = rand() % nodes; // Random node
        int v = rand() % nodes; // Random node

        // Avoid self-loops and duplicate edges
        if (u != v && !edgeExists(graph[u], v))

        {
            graph[u].push_back({v, weighted ? rand() % 100 + 1 : 0}); // Random weight if weighted
            if (!directed)
            {
                graph[v].push_back({u, weighted ? rand() % 100 + 1 : 0}); // Add reverse edge for undirected
            }
            edgeCount++;
        }

        // For sparse graphs, ensure there are fewer edges per node. This can be controlled by limiting edgeCount growth.
        if (sparse && edgeCount > edges)
        {
            edgeCount = edges; // Limit edge count to the desired number
        }
    }

    return graph;
}

int main()
{
    cout << "AS Graph Analysis\n";
    cout << "------------------\n";

    // Important declarations.
    string datasetFile = "weighted_edges.txt";
    Graph graph = readGraphFromFile(datasetFile);
    // unordered_map<int, vector<Edge>> graph; // when im detecting cycle
    int source = 0;
    string output;

    // Log execution times for different configurations
    vector<tuple<string, string>> algoOutputFiles = {
        {"Dijkstra", "dijkstra_output.txt"},
        {"BellmanFord", "bellman_output.txt"},
        {"Prims", "prims_output.txt"},
        {"Kruskal", "kruskal_output.txt"},
        {"BFS", "bfs_output.txt"},
        {"DFS", "dfs_recursive_output.txt"},
        {"Diameter", "diameter_output.txt"},
        {"CycleDetection", "cycle_detection_result.txt"}};

    vector<int> nodeSizes = {100, 200, 500, 1000};
    vector<int> edgeDensities = {200, 500, 1000, 2500};
    vector<string> graphTypes = {"Sparse", "Dense"};
    vector<bool> directedOptions = {false, true};
    vector<bool> weightedOptions = {true, false};

    int choice;
    double avgDegree;

    while (true)
    {
        cout << "\n--- Algorithm Menu ---\n";
        cout << "1. Dijkstra\n";
        cout << "2. Bellman-Ford\n";
        cout << "3. Prim's\n";
        cout << "4. Kruskal\n";
        cout << "5. BFS\n";
        cout << "6. DFS\n";
        cout << "7. Graph Diameter\n";
        cout << "8. Cycle Detection\n";
        cout << "9. Average Degree\n";
        cout << "10. Graph Visualisation\n";
        cout << "11. Exit\n";
        cout << "Enter your choice: ";

        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "Enter source node: ";
            cin >> source;
            output = "dijkstra_output.txt";
            dijkstraAlgorithm(graph, source, output);
            break;
        case 2:
            cout << "Enter source node: ";
            cin >> source;
            output = "bellman_output.txt";
            bellmanFordAlgorithm(graph, source, output);
            break;
        case 3:
            cout << "Enter source node: ";
            cin >> source;
            output = "prims_output.txt";
            primsAlgorithm(graph, source, output);
            break;
        case 4:
            cout << "Enter source node: ";
            cin >> source;
            output = "kruskal_output.txt";
            kruskalAlgorithm(graph, output);
            break;
        case 5:
            cout << "Enter source node: ";
            cin >> source;
            output = "bfs_output.txt";
            bfs(graph, source, output);
            break;
        case 6:
            cout << "Enter source node: ";
            cin >> source;
            output = "dfs_output.txt";
            dfs(graph, source, output);
            break;
        case 7:
            output = "diameter_output.txt";
            computeGraphDiameter(graph, output);
            break;
        case 8:
            output = "cycle_detection_result.txt";
            checkForCycleInGraph(graph, output);
            break;
        case 9:
            avgDegree = calculateAverageDegree(graph);
            cout << "Average Degree of the Graph: " << avgDegree << endl;
            break;
        case 10:
            // Loop through different graph configurations
            for (int nodes : nodeSizes)
            {
                for (int edges : edgeDensities)
                {
                    for (const string &graphType : graphTypes)
                    {
                        for (bool directed : directedOptions)
                        {
                            for (bool weighted : weightedOptions)
                            {
                                string graphTypeLabel = graphType + (directed ? "-Directed" : "-Undirected") +
                                                        (weighted ? "-Weighted" : "-Unweighted");

                                // Generate the graph
                                Graph graph = generateGraph(nodes, edges, weighted, directed, graphType == "Dense");

                                // Collect execution times for the algorithms
                                collectDetailedExecutionTimesToCSV(algoOutputFiles, graph, graphTypeLabel, "execution_times_detailed.csv");
                            }
                        }
                    }
                }
            }
            break;
        case 11:
            cout << "Exiting program.\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
