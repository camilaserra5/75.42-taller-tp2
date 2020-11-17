#include "FileProcessor.h"
#include "Lock.h"
#include <string>
#include <vector>

FileProcessor::FileProcessor(std::vector <std::string> files) :
        files(files) {}

std::string FileProcessor::getFile() {
    Lock l(this->mutex);
    std::string temp = this->files.back();
    this->files.pop_back();
    return temp;
}

bool FileProcessor::hasFiles() {
    Lock l(this->mutex);
    return !this->files.empty();
}
