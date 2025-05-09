#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <climits>
#include <chrono>
#include "algorithms.h"
using namespace std;

// Function to perform Dijkstra's Algorithm
void dijkstraAlgorithm(const Graph &graph, int source, const string &outputFilePath)
{
    unordered_map<int, int> dist;
    unordered_map<int, int> prev;

    for (const auto &node : graph)
    {
        dist[node.first] = INT_MAX;
        prev[node.first] = -1;
    }

    dist[source] = 0;

    // Min-heap priority queue: (distance, node)
    priority_queue<pair<int, int>,
                   vector<pair<int, int>>,
                   greater<>>
        pq;

    pq.push({0, source});

    // Trace file
    ofstream traceFile("trace_dijkstra.txt");
    traceFile << "Dijkstra Trace:\n";

    auto start = chrono::high_resolution_clock::now();

    while (!pq.empty())
    {
        auto [d, u] = pq.top();
        pq.pop();

        traceFile << "Popped node: " << u << " with distance: " << d << "\n";

        if (d > dist[u])
            continue;

        for (const Edge &edge : graph.at(u))
        {
            int v = edge.to;
            int weight = edge.weight;

            if (dist[u] + weight < dist[v])
            {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                pq.push({dist[v], v});

                traceFile << "Updated node: " << v << " to distance: " << dist[v] << "\n";
            }
        }
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> exec_time = end - start;

    traceFile.close();

    // Output shortest paths
    ofstream outFile(outputFilePath);
    outFile << "Source: " << source << "\n";
    for (const auto &pair : dist)
    {
        outFile << "To " << pair.first << ": Distance = " << pair.second << "\n";
    }

    outFile << "\nExecution Time: " << exec_time.count() << " seconds\n";
    outFile.close();

    cout << "Dijkstra completed. Results saved to " << outputFilePath << "\n";
}