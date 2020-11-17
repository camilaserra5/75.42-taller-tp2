#include "Locker.h"
#include <string>

Locker::Locker(FileProcessor &&fileProcessor,
               ResultsProcessor &&resultsProcessor) :
        fileProcessor(fileProcessor), resultsProcessor(resultsProcessor) {}

void Locker::operator()() {
    while (fileProcessor.hasFiles()) {
        std::string filename = fileProcessor.getFile();
        ExtendedBPF extendedBpf(filename);
        resultsProcessor.addResult(filename, extendedBpf.process());
    }
}
