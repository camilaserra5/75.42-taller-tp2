#include <cstdio>
#include <vector>
#include <string>
#include <thread>
#include "FileProcessor.h"
#include "ExtendedBPF.h"
#include "ResultsProcessor.h"
#include <iostream>

class Locker {
public:
    Locker(FileProcessor &fileProcessor, ResultsProcessor &resultsProcessor);

    void operator()();

    void printResult() const;

private:
    FileProcessor &fileProcessor;
    ResultsProcessor &resultsProcessor;
};


int main(int argc, char **argv) {
    std::vector <std::thread> threads;
    std::vector <Locker> locker;

    FileProcessor fileProcessor;
    ResultsProcessor resultsProcessor;

    for (int i = 1; i < (argc - 1); i++) {
        fileProcessor.addFile(argv[i + 1]);
    }

    unsigned int numOfThreads = atoi(argv[1]);
    locker.reserve(numOfThreads);
    for (unsigned int i = 0; i < numOfThreads; i++) {
        locker.push_back(Locker(fileProcessor, resultsProcessor));
    }

    for (unsigned int i = 0; i < numOfThreads; i++) {
        threads.push_back(std::thread(std::ref(locker[i])));
    }

    for (unsigned int i = 0; i < numOfThreads; i++) {
        threads[i].join();
    }

    for (unsigned int i = 0; i < numOfThreads; i++) {
        locker[i].printResult();
    }
}

Locker::Locker(FileProcessor &fileProcessor,
               ResultsProcessor &resultsProcessor) :
        fileProcessor(fileProcessor), resultsProcessor(resultsProcessor) {}

void Locker::printResult() const {
    while (resultsProcessor.hasResults()) {
        std::cout << resultsProcessor.getResult();
    }
}

void Locker::operator()() {
    while (fileProcessor.hasFiles()) {
        std::string filename = fileProcessor.getFile();
        ExtendedBPF extendedBpf = ExtendedBPF(filename);
        resultsProcessor.addResult(filename, extendedBpf.process());
    }
}
