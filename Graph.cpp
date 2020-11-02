#include "Graph.h"
#include<bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <string>

Graph::Graph(int V) {
    this->V = V;
    adj = new list<int>[V];
}

bool Graph::DFSUtil(int v, bool visited[], bool *recStack) {
    if (!visited[v]) {
        visited[v] = true;
        recStack[v] = true;

        std::list<int>::iterator i;
        for (i = adj[v].begin(); i != adj[v].end(); ++i) {
            if (!visited[*i] && DFSUtil(*i, visited, recStack))
                return true;
            else if (recStack[*i])
                return true;
        }

    }
    recStack[v] = false;
    return false;
}

std::string Graph::DFS(int v) {
    bool *visited = new bool[V];
    bool *recStack = new bool[V];
    for (int i = 0; i < V; i++) {
        recStack[i] = false;
        visited[i] = false;
    }
    if (DFSUtil(0, visited, recStack))
        return " FAIL: cycle detected";

    for (int i = 0; i < V; i++) {
        if (!visited[i])
            return " FAIL: unused instructions detected";
    }
    return " GOOD";
}

void Graph::addEdge(int v, int w) {
    adj[v].push_back(w);
}