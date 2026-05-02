#pragma once
#include "core/IFileChecker.hpp"

class INotifier {
public:
    virtual ~INotifier() = default;
    virtual void notify(const FileEvent& event) = 0;
};
