#include "ExtendedBPF.h"
#include<bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <string>
#include "Graph.h"

using namespace std;

ExtendedBPF::ExtendedBPF(std::string filename) : filename(filename) {

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
    return g.DFS(0);
}





