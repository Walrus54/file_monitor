#pragma once
#include "IFileChecker.h"
#include <unordered_map>
#include <string>

class RestorationChecker : public IFileChecker {
public:
    bool hasChanged(const std::string& path) override;
    FileEvent getEvent(const std::string& path) const override;

private:
    enum class State { UNKNOWN, PRESENT, ABSENT };
    std::unordered_map<std::string, State> state_;
    mutable std::unordered_map<std::string, std::string> lastEvent_;
};
