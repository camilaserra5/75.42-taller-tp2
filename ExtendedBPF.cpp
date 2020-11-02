#include "ExtendedBPF.h"
#include<bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include "Graph.h"

ExtendedBPF::ExtendedBPF(std::string filename) : filename(filename) {}

std::vector <std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector <std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        if (!token.empty())
            res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}


bool isJumpInstruction(std::string instruction) {
    std::set <std::string> jumpInstructions{"jmp", "ja",
                                            "jeq", "jneq",
                                            "jne", "jlt",
                                            "jle", "jgt",
                                            "jge", "jset"};
    return jumpInstructions.find(instruction) != jumpInstructions.end();
}

bool isLabel(std::string instruction) {
    return instruction.find(":") != std::string::npos;
}

std::string ExtendedBPF::process() {
    std::map<std::string, int> labelsToIdx;
    std::ifstream infile(this->filename);

    std::string line;
    int lines = 0;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        if (!iss.str().empty()) {
            std::vector <std::string> v = split(iss.str(), " ");
            if (isLabel(v[0])) {
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
            std::vector <std::string> instructions = split(iss.str(), " ");
            unsigned int idx = 0;
            if (isLabel(instructions[idx])) {
                idx++;
            }

            if (isJumpInstruction(instructions[idx])) {
                if (instructions.size() == 2 + idx) {
                    g.addEdge(lines, labelsToIdx[instructions[idx + 1]]);
                } else if (instructions.size() == 3 + idx) {
                    g.addEdge(lines, labelsToIdx[instructions[idx + 2]]);
                    if (labelsToIdx[instructions[idx + 2]] != lines + 1) {
                        g.addEdge(lines, lines + 1);
                    }
                } else if (instructions.size() == 4 + idx) {
                    std::string label = instructions[idx + 2];
                    label = label.substr(0, label.length() - 1);
                    g.addEdge(lines, labelsToIdx[label]);
                    g.addEdge(lines, labelsToIdx[instructions[idx + 3]]);
                }
            } else if (instructions[idx].find("ret") == std::string::npos) {
                g.addEdge(lines, lines + 1);
            }
            lines++;
        }
    }
    infile.close();
    
    std::string ret = g.checkCycleOrUnused();
    return ret;
}

