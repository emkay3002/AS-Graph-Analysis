#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>
#include <queue>
#include <unordered_map>
#include <climits>
#include <string>

using namespace std;

struct Edge
{
    int to;
    int weight;
};

using Graph = unordered_map<int, vector<Edge>>;

void dijkstraAlgorithm(const Graph &graph, int source, const string &outputFilePath);
void bellmanFordAlgorithm(const Graph &graph, int source, const string &outputFilePath);
void primsAlgorithm(const Graph &graph, int start, const std::string &outputFilePath);
void bfs(const Graph &graph, int start, const string &outputFile);
void dfs(const Graph &graph, int start, const string &outputFilePath);
void kruskalAlgorithm(const Graph &graph, const string &outputFilePath);
void computeGraphDiameter(const Graph &graph, const string &outputFilePath);
// void computeGraphDiameter(const Graph &graph, const string &outputFilePath, int maxNodesToProcess = -1);
// void cycleUndirected(const Graph &graph, const std::string &outputFilePath);
// void detectCycleDirected(const Graph &graph, const std::string &outputFilePath);
bool detectCycle(unordered_map<int, vector<Edge>> &graph, int currentNode, unordered_map<int, bool> &visitedNodes, int parentNode);
// void detectCycle(const Graph &graph, const string &resultFilePath);
void checkForCycleInGraph(unordered_map<int, vector<Edge>> &graph, const string &outputFilePath);

#endif
