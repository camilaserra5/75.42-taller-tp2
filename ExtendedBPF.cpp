#include "ExtendedBPF.h"
#include "Lock.h"
#include<bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

ExtendedBPF::ExtendedBPF(std::string filename) : filename(filename) {

}

using namespace std;

class Graph {
    int V;    // No. of vertices
    list<int> *adj;    // Pointer to an array containing adjacency lists
    bool DFSUtil(int v, bool visited[], bool *recStack);

public:
    Graph(int V);   // Constructor
    void addEdge(int v, int w);   // to add an edge to graph
    string DFS(int v);

    void print();
};

Graph::Graph(int V) {
    this->V = V;
    adj = new list<int>[V];
}

void Graph::print() {
    for (int i = 0; i < V; i++) {
        cout << "\n NODO: " << i;
        list<int>::iterator j;
        for (j = adj[i].begin(); j != adj[i].end(); ++j)
            cout << "\n ARISTA: " << i << "-" << *j;

    }

}

bool Graph::DFSUtil(int v, bool visited[], bool *recStack) {
    if (visited[v] == false) {
        // Mark the current node as visited and part of recursion stack
        visited[v] = true;
        recStack[v] = true;

        // Recur for all the vertices adjacent to this vertex
        list<int>::iterator i;
        for (i = adj[v].begin(); i != adj[v].end(); ++i) {
            if (!visited[*i] && DFSUtil(*i, visited, recStack))
                return true;
            else if (recStack[*i])
                return true;
        }

    }
    recStack[v] = false;  // remove the vertex from recursion stack
    return false;
}

string Graph::DFS(int v) {
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

vector <string> split(string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector <string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        if (!token.empty())
            res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}


std::string ExtendedBPF::process() {
    std::map<std::string, int> labelsToIdx;
    std::ifstream infile(this->filename);

    std::string line;
    int lines = 0;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        if (!iss.str().empty()) {
            vector <string> v = split(iss.str(), " ");
            if (v[0].find(":") != std::string::npos) {
                labelsToIdx[v[0].substr(0, v[0].length() - 1)] = lines;
            }
            lines++;
        }
    }
    Graph g(lines);
    lines = 0;
    infile.clear();
    infile.seekg(0);
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        if (!iss.str().empty()) {
            vector <string> v = split(iss.str(), " ");
            unsigned int idx = 0;
            if (v[idx].find(":") != std::string::npos) {
                idx++;
            }
            if ((v[idx].find("jmp") != std::string::npos) ||
                (v[idx].find("ja") != std::string::npos) ||
                (v[idx].find("jeq") != std::string::npos) ||
                (v[idx].find("jneq") != std::string::npos) ||
                (v[idx].find("jne") != std::string::npos) ||
                (v[idx].find("jlt") != std::string::npos) ||
                (v[idx].find("jle") != std::string::npos) ||
                (v[idx].find("jgt") != std::string::npos) ||
                (v[idx].find("jge") != std::string::npos) ||
                (v[idx].find("jset") != std::string::npos)) {

                if (v.size() == 2 + idx) {
                    g.addEdge(lines, labelsToIdx[v[idx + 1]]);
                } else if (v.size() == 3 + idx) {
                    g.addEdge(lines, labelsToIdx[v[idx + 2]]);
                    if (labelsToIdx[v[idx + 2]] != lines + 1)
                        g.addEdge(lines, lines + 1);
                } else if (v.size() == 4 + idx) {
                    g.addEdge(lines, labelsToIdx[v[idx + 2].substr(0, v[idx + 2].length() - 1)]);
                    g.addEdge(lines, labelsToIdx[v[idx + 3]]);
                }
            } else if (v[idx].find("ret") != std::string::npos) {
                // nada
            } else {
                g.addEdge(lines, lines + 1);
            }
            lines++;
        }
    }
    g.print();
    return g.DFS(0);
}





