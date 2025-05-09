#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_set>
#include <chrono>
#include "algorithms.h"

using namespace std;

void bfs(const Graph &graph, int start, const string &outputFilePath)
{
    unordered_set<int> visited;
    queue<int> q;
    ofstream traceFile("trace_bfs.txt");
    ofstream outFile(outputFilePath);
    traceFile << "BFS Trace:\n";
    outFile << "BFS Traversal Order (Starting from " << start << "):\n";

    // Start timer
    auto start_time = chrono::high_resolution_clock::now();

    q.push(start);
    traceFile << "Inserted into queue: " << start << "\n";

    while (!q.empty())
    {
        int node = q.front();
        q.pop();
        traceFile << "Dequeued: " << node << "\n";

        if (visited.count(node))
            continue;

        visited.insert(node);
        outFile << node << " ";

        for (const Edge &e : graph.at(node))
        {
            if (!visited.count(e.to))
            {
                q.push(e.to);
                traceFile << "Inserted into queue: " << e.to << "\n";
            }
        }
    }

    // End timer
    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> exec_time = end_time - start_time;

    outFile << "\nExecution Time: " << exec_time.count() << " seconds\n";
    traceFile << "\nExecution Time: " << exec_time.count() << " seconds\n";

    traceFile.close();
    outFile.close();

    cout << "\nBFS traversal complete. Output written to " << outputFilePath << " and trace to trace_bfs.txt\n";
    cout << "Execution Time: " << exec_time.count() << " seconds\n";
}
