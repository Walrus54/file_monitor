#include "notifiers/ConsoleNotifier.h"
#include "utils/Logger.h"

void ConsoleNotifier::notify(const FileEvent& event) {
    // Делегируем вывод Logger: он потокобезопасен и умеет писать в файл
    Logger::getInstance().log(LogLevel::INFO, "[EVENT] " + event.path + ": " + event.description);
}
