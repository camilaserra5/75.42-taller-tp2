#include "ExtendedBPF.h"
#include "Lock.h"

ExtendedBPF::ExtendedBPF() {
    this->balance = 0;
}

void ExtendedBPF::withdraw(int amount) {
    Lock l(this->mutex);
    this->balance -= amount;
}

void ExtendedBPF::deposit(int amount) {
    Lock l(this->mutex);
    this->balance += amount;
}

int ExtendedBPF::getBalance() const {
    return this->balance;
}
