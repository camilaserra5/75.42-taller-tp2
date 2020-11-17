#ifndef RESULTS_PROCESSOR_H
#define RESULTS_PROCESSOR_H

#include <mutex>
#include <vector>
#include <string>

class ResultsProcessor {
public:
    ResultsProcessor();

    void addResult(const std::string &filename, const std::string &result);

    std::string getResult();

private:
    std::vector <std::string> results;
    std::mutex mutex;
};

#endif
