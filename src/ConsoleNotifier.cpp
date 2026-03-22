#include "ConsoleNotifier.h"
#include <iostream>

void ConsoleNotifier::notify(const FileEvent& event) {
    std::cout << "[EVENT] " << event.path << ": " << event.description << "\n";
}
