#include "ResultsProcessor.h"
#include "Lock.h"
#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>

ResultsProcessor::ResultsProcessor() {}

void ResultsProcessor::addResult(const std::string &filename,
                                 const std::string &result) {
    Lock l(this->mutex);
    this->results.push_back(filename + " " + result);
}

std::string ResultsProcessor::getResult() {
    std::stringstream ss;
    std::sort(this->results.begin(), this->results.end());
    for (unsigned int i = 0; i < this->results.size(); i++) {
        ss << this->results[i] << std::endl;
    }
    return ss.str();
}

