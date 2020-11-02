#ifndef GRAPH_H
#define GRAPH_H

#include<bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <string>
#include <list>

class Graph {
    int V;
    std::list<int> *adj;

    bool DFSUtil(int v, bool visited[], bool *recStack);

public:
    explicit Graph(int V);

    void addEdge(int v, int w);

    std::string checkCycleOrUnused();
};

#endif //GRAPH_H
