#ifndef GRAPH_H
#define GRAPH_H

#include<bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class Graph {
    int V;
    std::vector<std::vector<int>> adj;

    bool DFSUtil(int v, std::vector<bool>& visited, std::vector<bool> &recStack);

public:
    explicit Graph(int V);

    ~Graph();
    void print();
    void addEdge(int v, int w);

    std::string checkCycleOrUnused();
};

#endif //GRAPH_H
