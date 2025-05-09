#include <iostream>
#include <fstream>
#include <unordered_set>
#include <chrono>
#include "algorithms.h"

using namespace std;

void dfsRecursiveHelper(const Graph &graph, int node, unordered_set<int> &visited, ofstream &traceFile, ofstream &outFile)
{
    visited.insert(node);
    outFile << node << " ";
    traceFile << "Visited node: " << node << "\n";
    for (const Edge &e : graph.at(node))
    {
        if (!visited.count(e.to))
        {
            traceFile << "Recursing to: " << e.to << " from " << node << "\n";
            dfsRecursiveHelper(graph, e.to, visited, traceFile, outFile);
        }
    }
}
void dfs(const Graph &graph, int start, const string &outputFilePath)
{
    unordered_set<int> visited;
    ofstream traceFile("trace_dfs_recursive.txt");
    ofstream outFile(outputFilePath);
    traceFile << "DFS Recursive Trace:\n";
    outFile << "DFS Recursive Traversal Order (Starting from " << start << "):\n";
    auto start_time = chrono::high_resolution_clock::now();
    dfsRecursiveHelper(graph, start, visited, traceFile, outFile);
    auto end_time = chrono::high_resolution_clock::now();

    chrono::duration<double> exec_time = end_time - start_time;

    outFile << "\nExecution Time: " << exec_time.count() << " seconds\n";
    traceFile << "\nExecution Time: " << exec_time.count() << " seconds\n";

    outFile.close();
    traceFile.close();

    cout << "\nRecursive DFS traversal complete. Output written to " << outputFilePath << " and trace to trace_dfs_recursive.txt\n";
    cout << "Execution Time: " << exec_time.count() << " seconds\n";
}