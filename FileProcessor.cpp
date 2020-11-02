#include "FileProcessor.h"
#include "Lock.h"

FileProcessor::FileProcessor() {

}

void FileProcessor::addFile(std::string name) {
    Lock l(this->mutex);
    this->files.push_back(name);
}

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
