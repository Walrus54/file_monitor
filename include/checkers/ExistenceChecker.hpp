#pragma once
#include "core/IFileChecker.hpp"
#include <unordered_map>

class ExistenceChecker : public IFileChecker {
public:
    bool hasChanged(const std::string& path) override;
    FileEvent getEvent(const std::string& path) const override;

private:
    std::unordered_map<std::string, bool> prevState_;
};
