#ifndef EXTENDED_BPF_H
#define EXTENDED_BPF_H

#include <mutex>
#include <vector>
#include <string>

class ExtendedBPF {
public:
    explicit ExtendedBPF(std::string filename);

    std::string process();

private:
    std::string filename;
};

#endif
