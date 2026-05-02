#pragma once
#include <string>

struct FileEvent {
    std::string path;
    std::string description;
};

class IFileChecker {
public:
    virtual ~IFileChecker() = default;
    virtual bool hasChanged(const std::string& path) = 0;
    virtual FileEvent getEvent(const std::string& path) const = 0;
};
