#ifndef FILE_PROCESSOR_H
#define FILE_PROCESSOR_H

#include <mutex>
#include <vector>
#include <string>

class FileProcessor {
public:
    explicit FileProcessor(std::vector <std::string> files);

    std::string getFile();

    bool hasFiles();

private:
    std::vector <std::string> files;
    std::mutex mutex;
};

#endif
