#ifndef LOCKER_H
#define LOCKER_H

#include "FileProcessor.h"
#include "ExtendedBPF.h"
#include "ResultsProcessor.h"

class Locker {
public:
    Locker(FileProcessor &&fileProcessor, ResultsProcessor &&resultsProcessor);

    void operator()();

private:
    FileProcessor &fileProcessor;
    ResultsProcessor &resultsProcessor;
};


#endif //LOCKER_H
