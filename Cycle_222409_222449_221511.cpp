#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <climits>
#include <sstream>
using namespace std;
using namespace chrono;

// Define the Edge structure
struct Edge
{
    int to;     // Destination node
    int weight; // Edge weight, if needed
};

// Function to detect a cycle using DFS
bool detectCycle(unordered_map<int, vector<Edge>> &graph, int currentNode, unordered_map<int, bool> &visitedNodes, int parentNode)
{
    visitedNodes[currentNode] = true;

    // Traverse the neighbors of the current node
    for (const Edge &edge : graph[currentNode])
    {
        int neighbor = edge.to;      // Get the neighbor from the edge
        if (!visitedNodes[neighbor]) // If the neighbor has not been visited yet
        {
            if (detectCycle(graph, neighbor, visitedNodes, currentNode))
                return true;
        }
        // If the neighbor is visited and it's not the parent, then a cycle is detected
        else if (neighbor != parentNode)
        {
            return true;
        }
    }

    return false;
}

// Function to check for cycles in the graph
void checkForCycleInGraph(unordered_map<int, vector<Edge>> &graph, const string &outputFilePath)
{
    unordered_map<int, bool> visitedNodes;
    bool cycleFound = false;

    auto startTime = high_resolution_clock::now();

    // Check each node for cycles
    for (const auto &nodeEntry : graph)
    {
        int node = nodeEntry.first;
        if (!visitedNodes[node]) // If the node is not visited yet
        {
            if (detectCycle(graph, node, visitedNodes, -1))
            {
                cycleFound = true;
                break; // Stop searching if a cycle is found
            }
        }
    }

    auto endTime = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(endTime - startTime).count();

    ofstream output(outputFilePath);
    ofstream trace("trace_cycle.txt");

    output << "Cycle Detection\n";

    if (cycleFound)
    {
        output << "Cycle detected in the graph.\n";
        trace << "Cycle detected in the graph.\n";
    }
    else
    {
        output << "No cycle detected in the graph.\n";
        trace << "No cycle detected in the graph.\n";
    }

    output << "Execution Time: " << duration << " seconds\n";
    trace << "Execution Time: " << duration << " seconds\n";

    cout << "Cycle detection completed. See " << outputFilePath << " and trace_cycle.txt\n";
}
