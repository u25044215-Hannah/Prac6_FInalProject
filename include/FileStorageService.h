#ifndef FILESTORAGESERVICE_H
#define FILESTORAGESERVICE_H

#include <string>

class FileStorageService {
public:
    bool writeFile(const std::string& path,
                   const std::string& bytes);
};

#endif
