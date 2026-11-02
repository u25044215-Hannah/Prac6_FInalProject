#include "FileStorageService.h"
#include <fstream>
bool FileStorageService::writeFile(const std::string& path, const std::string& bytes) {
    if (path.empty()) return false;
    std::ofstream file(path.c_str(), std::ios::binary | std::ios::trunc);
    if (!file) return false;
    file.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
    file.close();
    return !file.fail();
}
