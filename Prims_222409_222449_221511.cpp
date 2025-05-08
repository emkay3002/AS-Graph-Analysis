#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <climits>
#include <set>
#include <chrono>
#include "algorithms.h"

using namespace std;

void primsAlgorithm(const Graph &graph, int start, const string &outputFilePath)
{
    unordered_map<int, int> parent;
    unordered_map<int, int> key;
    set<int> inMST;

    for (const auto &node : graph)
    {
        key[node.first] = INT_MAX;
        parent[node.first] = -1;
    }

    key[start] = 0;

    auto cmp = [&](int left, int right)
    {
        return key[left] > key[right];
    };
    priority_queue<int, vector<int>, decltype(cmp)> pq(cmp);
    pq.push(start);

    ofstream trace("trace_prims.txt");
    trace << "Prim's Algorithm Trace (Start Node: " << start << ")\n\n";

    auto start_time = chrono::high_resolution_clock::now();

    while (!pq.empty())
    {
        int u = pq.top();
        pq.pop();

        if (inMST.count(u))
            continue;

        inMST.insert(u);

        if (parent[u] != -1)
        {
            trace << "Added edge " << parent[u] << " - " << u << " (Weight: " << key[u] << ")\n";
        }

        for (const Edge &e : graph.at(u))
        {
            int v = e.to;
            int weight = e.weight;

            if (!inMST.count(v) && weight < key[v])
            {
                trace << "Updating key of " << v << " from " << key[v] << " to " << weight << " via " << u << "\n";
                key[v] = weight;
                parent[v] = u;
                pq.push(v);
            }
        }
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> exec_time = end_time - start_time;

    ofstream out(outputFilePath);
    out << "Prim's Minimum Spanning Tree (starting from node " << start << "):\n";
    cout << "\nPrim's Minimum Spanning Tree (starting from node " << start << "):\n";

    int totalWeight = 0;
    for (const auto &node : graph)
    {
        int v = node.first;
        if (parent[v] != -1)
        {
            out << parent[v] << " - " << v << " (Weight: " << key[v] << ")\n";
            cout << parent[v] << " - " << v << " (Weight: " << key[v] << ")\n";
            totalWeight += key[v];
        }
    }

    out << "\nTotal Weight of MST:" << totalWeight << "\n";
    out << "Execution Time:" << exec_time.count() << " seconds\n";

    cout << "\nTotal Weight of MST: " << totalWeight << endl;
    cout << "Execution Time:" << exec_time.count() << " seconds" << endl;

    trace << "\nTotal Weight of MST: " << totalWeight << "\n";
    trace << "Execution Time:" << exec_time.count() << " seconds\n";

    out.close();
    trace.close();

    cout << "\nPrims algorithm completed. Output written to " << outputFilePath << endl;
}
