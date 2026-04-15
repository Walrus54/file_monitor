#include "ConsoleNotifier.h"
#include "Logger.h"

void ConsoleNotifier::notify(const FileEvent& event) {
    Logger::getInstance().log(LogLevel::INFO, "[EVENT] " + event.path + ": " + event.description);
}
