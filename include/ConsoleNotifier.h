#pragma once
#include "INotifier.h"

class ConsoleNotifier : public INotifier {
public:
    void notify(const FileEvent& event) override;
};
