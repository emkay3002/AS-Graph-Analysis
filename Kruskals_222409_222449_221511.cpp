#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <chrono>
#include "algorithms.h"

using namespace std;

// Disjoint Set Union (Union-Find)
class DSU
{
    unordered_map<int, int> parent;
    unordered_map<int, int> rank;

public:
    void makeSet(const Graph &graph)
    {
        for (const auto &node : graph)
        {
            parent[node.first] = node.first;
            rank[node.first] = 0;
        }
    }
    int find(int u)
    {
        if (parent[u] != u)
            parent[u] = find(parent[u]);
        return parent[u];
    }

    bool unite(int u, int v, ofstream &trace)
    {
        int pu = find(u);
        int pv = find(v);

        if (pu == pv)
        {
            trace << "Skipping edge " << u << " - " << v << " (Same parent: " << pu << ")\n";
            return false; // cycle
        }

        if (rank[pu] < rank[pv])
            parent[pu] = pv;
        else if (rank[pu] > rank[pv])
            parent[pv] = pu;
        else
        {
            parent[pv] = pu;
            rank[pu]++;
        }

        trace << "Union of " << u << " and " << v << " (Roots: " << pu << ", " << pv << ")\n";
        return true;
    }
};

void kruskalAlgorithm(const Graph &graph, const string &outputFilePath)
{
    vector<tuple<int, int, int>> edges; // (weight, u, v)
    for (const auto &entry : graph)
    {
        int u = entry.first;
        for (const Edge &e : entry.second)
        {
            if (u < e.to)
            { // avoid duplicate edges
                edges.emplace_back(e.weight, u, e.to);
            }
        }
    }

    sort(edges.begin(), edges.end());

    ofstream outFile(outputFilePath);
    ofstream traceFile("trace_kruskal.txt");

    traceFile << "Kruskal's Algorithm Trace:\n";

    DSU dsu;
    dsu.makeSet(graph);

    vector<tuple<int, int, int>> mst;
    int totalWeight = 0;

    auto start_time = chrono::high_resolution_clock::now();

    for (const auto &[w, u, v] : edges)
    {
        traceFile << "Processing edge " << u << " - " << v << " (Weight: " << w << ")\n";
        if (dsu.unite(u, v, traceFile))
        {
            mst.emplace_back(w, u, v);
            totalWeight += w;
            traceFile << "Edge added to MST: " << u << " - " << v << " (Weight: " << w << ")\n";
        }
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> exec_time = end_time - start_time;

    outFile << "Kruskal's Minimum Spanning Tree:\n";
    for (const auto &[w, u, v] : mst)
    {
        outFile << u << " - " << v << " (Weight: " << w << ")\n";
        cout << u << " - " << v << " (Weight: " << w << ")\n";
    }

    outFile << "\nTotal Weight of MST: " << totalWeight << "\n";
    outFile << "Execution Time: " << exec_time.count() << " seconds\n";
    traceFile << "\nTotal Weight: " << totalWeight << "\n";
    traceFile << "Execution Time: " << exec_time.count() << " seconds\n";

    outFile.close();
    traceFile.close();

    cout << "\nKruskal's algorithm complete. Output written to " << outputFilePath << "\n";
}