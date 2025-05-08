#include <queue>
#include <unordered_set>
#include <fstream>
#include <iostream>
#include "algorithms.h"

using namespace std;

void bfs(const Graph &graph, int start, const string &outputFile)
{
    unordered_set<int> visited;
    queue<int> q;

    ofstream out(outputFile);
    out << "Breadth-First Search (starting from node " << start << "):\n";
    cout << "\nBreadth-First Search (starting from node " << start << "):\n";

    q.push(start);
    visited.insert(start);

    while (!q.empty())
    {
        int current = q.front();
        q.pop();

        cout << current << " ";
        out << current << " ";

        for (const Edge &edge : graph.at(current))
        {
            int neighbor = edge.to;
            if (!visited.count(neighbor))
            {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }

    cout << endl;
    out << endl;
    out.close();
}
