#ifndef EXTENDED_BPF_H
#define EXTENDED_BPF_H

#include <mutex>
#include <vector>

class ExtendedBPF {
public:
    ExtendedBPF(std::string filename);

    std::string process();

private:
    std::string filename;
};

#endif