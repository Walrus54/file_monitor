#include "checkers/SizeChecker.h"
#include <filesystem>

namespace fs = std::filesystem;

bool SizeChecker::hasChanged(const std::string& path) {
    if (!fs::exists(path)) return false;
    uintmax_t cur = fs::file_size(path);
    auto it = prevSize_.find(path);
    if (it == prevSize_.end()) {
        prevSize_[path] = cur;
        return false;
    }
    bool changed = (it->second != cur);
    it->second = cur;
    return changed;
}

FileEvent SizeChecker::getEvent(const std::string& path) const {
    uintmax_t size = fs::exists(path) ? fs::file_size(path) : 0;
    return {path, "size changed to " + std::to_string(size) + " bytes"};
}
