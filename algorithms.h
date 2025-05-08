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
void primAlgorithm();
void kruskalAlgorithm();

#endif
