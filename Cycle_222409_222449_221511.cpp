#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <climits>
#include <sstream>
#include <algorithm>
using namespace std;
using namespace chrono;

// Define the Edge structure
struct Edge
{
    int to;     // Destination node
    int weight; // Edge weight, if needed
};

// Function to detect a cycle using DFS
#include <algorithm> // For std::find

bool detectCycle(unordered_map<int, vector<Edge>> &graph, int currentNode, unordered_map<int, bool> &visitedNodes, int parentNode, vector<int> &path, ofstream &outputFile)
{
    visitedNodes[currentNode] = true;
    path.push_back(currentNode); // Add current node to the path

    // Traverse the neighbors of the current node
    for (const Edge &edge : graph[currentNode])
    {
        int neighbor = edge.to;

        // If the neighbor is not visited
        if (!visitedNodes[neighbor])
        {
            if (detectCycle(graph, neighbor, visitedNodes, currentNode, path, outputFile))
                return true;
        }
        // If the neighbor is visited and it's not the parent, a cycle is detected
        else if (neighbor != parentNode)
        {
            // Cycle is detected, write the cycle path to the output file
            outputFile << "Cycle detected: ";
            // Find the start of the cycle in the path vector
            auto cycleStart = find(path.begin(), path.end(), neighbor);
            for (auto it = cycleStart; it != path.end(); ++it)
            {
                outputFile << *it << " "; // Write the cycle path to the file
            }
            outputFile << endl;
            return true;
        }
    }

    path.pop_back(); // Remove the current node from the path before returning
    return false;
}

// Function to check for cycles in the graph
void checkForCycleInGraph(unordered_map<int, vector<Edge>> &graph, const string &outputFilePath)
{
    unordered_map<int, bool> visitedNodes;
    bool cycleFound = false;

    auto startTime = high_resolution_clock::now();

    ofstream outputFile(outputFilePath); // Open output file for writing

    // Write header to the output file
    outputFile << "Graph Cycle Detection\n";

    // Check each node for cycles
    for (const auto &nodeEntry : graph)
    {
        int node = nodeEntry.first;
        if (!visitedNodes[node]) // If the node is not visited yet
        {
            vector<int> path; // To store the current path of nodes
            if (detectCycle(graph, node, visitedNodes, -1, path, outputFile))
            {
                cycleFound = true;
                break; // Stop searching if a cycle is found
            }
        }
    }

    auto endTime = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(endTime - startTime).count();

    if (!cycleFound)
    {
        outputFile << "No cycle detected in the graph.\n";
    }
    outputFile << "Execution Time: " << duration << " seconds\n";

    cout << "Cycle detection completed. See " << outputFilePath << " for the result.\n";
}
