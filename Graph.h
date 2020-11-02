#ifndef GRAPH_H
#define GRAPH_H

#include<bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class Graph {
    int V;
    list<int> *adj;

    bool DFSUtil(int v, bool visited[], bool *recStack);

public:
    Graph(int V);

    void addEdge(int v, int w);

    std::string DFS(int v);
};

#endif //GRAPH_H
