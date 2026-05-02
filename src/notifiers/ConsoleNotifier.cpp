#include "notifiers/ConsoleNotifier.hpp"
#include "utils/Logger.hpp"

void ConsoleNotifier::notify(const FileEvent& event) {
    Logger::getInstance().log(LogLevel::INFO, "[EVENT] " + event.path + ": " + event.description);
}
