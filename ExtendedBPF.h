#ifndef EXTENDED_BPF_H
#define EXTENDED_BPF_H

#include <mutex>

class ExtendedBPF {
public:
    ExtendedBPF();

    void withdraw(int amount);

    void deposit(int amount);

    int getBalance() const;

private:
    int balance;
    std::mutex mutex;
};

#endif