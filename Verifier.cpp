#include "Verifier.h"
#include <cstdio>
#include <thread>
#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <thread>
#include "FileProcessor.h"
#include "ExtendedBPF.h"
#include "ResultsProcessor.h"
#include "Locker.h"

Verifier::Verifier() {}

std::string Verifier::verify(unsigned int numOfThreads,
                             std::vector <std::string> files) {
    std::vector <std::thread> threads;
    std::vector <Locker> locker;

    FileProcessor fileProcessor(files);
    ResultsProcessor resultsProcessor;

    locker.reserve(numOfThreads);
    for (unsigned int i = 0; i < numOfThreads; i++) {
        locker.push_back(Locker(std::move(fileProcessor),
                                std::move(resultsProcessor)));
        threads.push_back(std::thread(std::ref(locker[i])));
    }

    for (unsigned int i = 0; i < numOfThreads; i++) {
        threads[i].join();
    }

    return resultsProcessor.getResult();
}

Verifier::~Verifier() {}
