#include <iostream>
#include <fstream>
#include <unordered_map>
#include <climits>
#include <chrono>
#include <vector>
#include <tuple>
#include <stack>
#include <sstream>
#include <string>
#include "algorithms.h"
using namespace std;

// Helper function to reconstruct the path from source to a node
string reconstructPath(int target, const unordered_map<int, int> &prev)
{
    stack<int> path;
    int current = target;

    while (current != -1)
    {
        path.push(current);
        current = prev.at(current);
    }

    stringstream ss;
    while (!path.empty())
    {
        ss << path.top();
        path.pop();
        if (!path.empty())
            ss << " -> ";
    }

    return ss.str();
}

void bellmanFordAlgorithm(const Graph &graph, int source, const string &outputFilePath)
{
    unordered_map<int, int> dist;
    unordered_map<int, int> prev;

    vector<tuple<int, int, int>> edges;
    for (const auto &[u, neighbors] : graph)
    {
        for (const Edge &e : neighbors)
        {
            edges.push_back({u, e.to, e.weight});
        }
        dist[u] = INT_MAX;
        prev[u] = -1;
    }

    dist[source] = 0;

    ofstream traceFile("trace_bellman.txt");
    traceFile << "Bellman-Ford Trace:\n";

    auto start = chrono::high_resolution_clock::now();

    int V = dist.size();

    for (int i = 1; i < V; ++i)
    {
        for (const auto &[u, v, w] : edges)
        {
            if (dist[u] != INT_MAX && dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                prev[v] = u;
                traceFile << "Iteration " << i << ": Updated " << v << " to distance " << dist[v] << "\n";
            }
        }
    }

    bool hasNegativeCycle = false;
    for (const auto &[u, v, w] : edges)
    {
        if (dist[u] != INT_MAX && dist[u] + w < dist[v])
        {
            traceFile << "Negative weight cycle detected!\n";
            hasNegativeCycle = true;
            break;
        }
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> exec_time = end - start;
    traceFile.close();

    ofstream outFile(outputFilePath);
    outFile << "Source: " << source << "\n";
    if (!hasNegativeCycle)
    {
        for (const auto &[node, distance] : dist)
        {
            outFile << "To " << node << ": Distance = " << distance;
            if (distance != INT_MAX)
            {
                outFile << " | Path: " << reconstructPath(node, prev);
            }
            else
            {
                outFile << " | Path: Unreachable";
            }
            outFile << "\n";
        }
    }
    else
    {
        outFile << "Graph contains a negative weight cycle. Shortest path not reliable.\n";
    }

    outFile << "\nExecution Time: " << exec_time.count() << " seconds\n";
    outFile.close();

    cout << "Bellman-Ford completed. Results saved to " << outputFilePath << "\n";
}
