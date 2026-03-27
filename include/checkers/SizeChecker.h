#pragma once
#include "IFileChecker.h"
#include <unordered_map>

class SizeChecker : public IFileChecker {
public:
    bool hasChanged(const std::string& path) override;
    FileEvent getEvent(const std::string& path) const override;
private:
    std::unordered_map<std::string, uintmax_t> prevSize_;
};
