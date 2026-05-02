#pragma once
#include "core/INotifier.hpp"

class ConsoleNotifier : public INotifier {
public:
    void notify(const FileEvent& event) override;
};
