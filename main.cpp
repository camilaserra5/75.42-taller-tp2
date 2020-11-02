#include <cstdio>
#include <vector>
#include <string>
#include <thread>
#include "FileProcessor.h"
#include "ExtendedBPF.h"
#include <iostream>

class Locker {
public:
    Locker(FileProcessor &accountFrom, ExtendedBPF &accountTo);

    void operator()();

    void printResult() const;

private:
    FileProcessor &accountFrom;
    ExtendedBPF &accountTo;
};


int main(int argc, char **argv) {
    std::vector <std::thread> threads;
    std::vector <Locker> locker;

    FileProcessor ac1;
    ExtendedBPF ac2;

    for (int i = 1; i < (argc - 1); i++) {
        ac1.addFile(argv[i + 1]);
    }

    unsigned int numOfThreads = atoi(argv[1]);
    locker.reserve(numOfThreads);
    for (unsigned int i = 0; i < numOfThreads; i++) {
        locker.push_back(Locker(ac1, ac2));
        threads.push_back(std::thread(std::ref(locker[i])));
    }

    for (unsigned int i = 0; i < numOfThreads; i++) {
        threads[i].join();
    }

}

Locker::Locker(FileProcessor &accountFrom, ExtendedBPF &accountTo) :
        accountFrom(accountFrom), accountTo(accountTo) {}

void Locker::printResult() const {
    printf("acFrom: ");
}

void Locker::operator()() {
    while (accountFrom.hasFiles()) {
        std::cout << "Filee: " << accountFrom.getFile();
    }
// dame file
// proceso
// guardo resultado
}
