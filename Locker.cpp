#include "Locker.h"
#include <string>
#include <utility>

Locker::Locker(FileProcessor &&fileProcessor,
               ResultsProcessor &&resultsProcessor) :
        fileProcessor(fileProcessor), resultsProcessor(resultsProcessor) {}

void Locker::operator()() {
    while (fileProcessor.hasFiles()) {
        std::string filename = fileProcessor.getFile();
        ExtendedBPF extendedBpf(std::move(filename));
        resultsProcessor.addResult(filename, extendedBpf.process());
    }
}
