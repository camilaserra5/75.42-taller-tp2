#include "ResultsProcessor.h"
#include "Lock.h"
#include <string>

ResultsProcessor::ResultsProcessor() {}

void ResultsProcessor::addResult(std::string filename, std::string result) {
    Lock l(this->mutex);
    this->results.push_back(filename + " " + result);
}

std::string ResultsProcessor::getResult() {
    Lock l(this->mutex);
    std::string temp = this->results.back();
    this->results.pop_back();
    return temp;
}

bool ResultsProcessor::hasResults() {
    Lock l(this->mutex);
    return !this->results.empty();
}
