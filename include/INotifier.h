#pragma once
#include "IFileChecker.h"

class INotifier {
public:
    virtual ~INotifier() = default;
    virtual void notify(const FileEvent& event) = 0;
};
