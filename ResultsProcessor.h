#ifndef RESULTS_PROCESSOR_H
#define RESULTS_PROCESSOR_H

#include <mutex>
#include <vector>

class ResultsProcessor {
public:
    ResultsProcessor();

    void addResult(std::string name);

    std::string getResult();

    bool hasResults();

private:
    std::vector<std::string> results;
    std::mutex mutex;
};

#endif