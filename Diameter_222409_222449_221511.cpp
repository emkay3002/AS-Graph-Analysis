#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <climits>
#include <chrono>
#include "algorithms.h"

using namespace std;

// Run Dijkstra from a single source and return shortest distances to all nodes
unordered_map<int, int> runDijkstra(const Graph &graph, int source, ofstream &trace)
{
    unordered_map<int, int> dist;
    for (const auto &node : graph)
        dist[node.first] = INT_MAX;
    dist[source] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.push({0, source});

    trace << "[Dijkstra from node " << source << "]\n";

    while (!pq.empty())
    {
        auto [currDist, node] = pq.top();
        pq.pop();

        for (const Edge &edge : graph.at(node))
        {
            int neighbor = edge.to;
            int weight = edge.weight;

            if (dist[node] + weight < dist[neighbor])
            {
                dist[neighbor] = dist[node] + weight;
                pq.push({dist[neighbor], neighbor});
                trace << "  → Updated distance: " << neighbor << " = " << dist[neighbor] << " (via " << node << ")\n";
            }
        }
    }

    trace << "  Final distances:\n";
    for (const auto &[node, d] : dist)
        trace << "    " << source << " → " << node << " = " << (d == INT_MAX ? -1 : d) << "\n";

    trace << "\n";
    return dist;
}
// Compute and save the graph's diameter (max shortest path between any two nodes)
void computeGraphDiameter(const Graph &graph, const string &outputFilePath)
{
    ofstream trace("trace_diameter.txt");
    ofstream output(outputFilePath);
    auto start = chrono::high_resolution_clock::now();
    int maxDistance = 0;
    int nodeU = -1, nodeV = -1;

    for (const auto &[source, _] : graph)
    {
        unordered_map<int, int> dist = runDijkstra(graph, source, trace);
        for (const auto &[target, d] : dist)
        {
            if (d != INT_MAX && d > maxDistance)
            {
                maxDistance = d;
                nodeU = source;
                nodeV = target;
            }
        }
    }

    auto end = chrono::high_resolution_clock::now();
    double duration = chrono::duration<double>(end - start).count();

    output << "Graph Diameter\n";
    output << "--------------\n";
    output << "Diameter (max shortest path length): " << maxDistance << "\n";
    output << "Between nodes: " << nodeU << " and " << nodeV << "\n";
    output << "Execution Time: " << duration << " seconds\n";

    trace << "Longest shortest path found between " << nodeU << " and " << nodeV << " = " << maxDistance << "\n";
    trace << "Execution Time: " << duration << " seconds\n";

    cout << "Diameter calculated. See " << outputFilePath << " and trace_diameter.txt\n";
}