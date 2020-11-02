#include "ResultsProcessor.h"
#include "Lock.h"
#include <string>
#include <algorithm>
#include <iostream>

ResultsProcessor::ResultsProcessor() {}

void ResultsProcessor::addResult(std::string filename, std::string result) {
    Lock l(this->mutex);
    this->results.push_back(filename + " " + result);
}

void ResultsProcessor::printResult() {
    std::sort(this->results.begin(), this->results.end());
    for (unsigned int i = 0; i < this->results.size(); i++) {
        std::cout << this->results[i] << std::endl;
    }
}

