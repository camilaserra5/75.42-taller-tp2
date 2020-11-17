#ifndef VERIFIER_H
#define VERIFIER_Hs

#include <vector>
#include <string>

class Verifier {
public:
    Verifier();

    std::string verify(unsigned int numOfThreads,
                       std::vector <std::string> files);

    ~Verifier();
};

#endif //VERIFIER_H
