#include "Verifier.h"
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char **argv) {
    std::vector <std::string> files;
    for (int i = 1; i < (argc - 1); i++) {
        files.push_back(argv[i + 1]);
    }
    Verifier verifier;
    std::cout << verifier.verify(atoi(argv[1]), files);
}
