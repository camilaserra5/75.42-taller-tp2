#include "Graph.h"
#include<bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

Graph::Graph(int V) : V(V) {
    adj.resize(V);
}

Graph::~Graph() {}

void Graph::addEdge(int v, int w) {
    adj[v].push_back(w);
}

bool Graph::DFSUtil(int v, std::vector<bool> &visited,
                    std::vector<bool> &recStack) {
    if (!visited[v]) {
        visited[v] = true;
        recStack[v] = true;

        for (unsigned int i = 0; i < adj[v].size(); ++i) {
            if (!visited[adj[v][i]] && DFSUtil(adj[v][i], visited, recStack))
                return true;
            else if (recStack[adj[v][i]])
                return true;
        }
    }
    recStack[v] = false;
    return false;
}

std::string Graph::checkCycleOrUnused() {
    std::vector<bool> visited;
    std::vector<bool> recStack;
    for (int i = 0; i < V; i++) {
        visited.push_back(false);
        recStack.push_back(false);
    }

    if (DFSUtil(0, visited, recStack)) {
        return "FAIL: cycle detected";
    }

    for (int i = 0; i < V; i++) {
        if (!visited[i]) {
            return "FAIL: unused instructions detected";
        }
    }
    return "GOOD";
}
