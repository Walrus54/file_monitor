/**
 * @file main.cpp
 * @brief Точка входа и composition root приложения.
 *
 * Единственное место, где создаются конкретные типы (composition root).
 * FileMonitor получает все зависимости через конструктор — принцип DIP соблюдён.
 */

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "checkers/FileChecker.h"
#include "core/FileMonitor.h"
#include "notifiers/ConsoleNotifier.h"
#include "utils/Config.h"
#include "utils/Logger.h"

int main(int argc, char* argv[]) {
    // Загружаем конфиг: из файла если передан аргумент, иначе — дефолты
    Config config = (argc > 1) ? Config::fromFile(argv[1]) : Config{};

    auto notifier = std::make_shared<ConsoleNotifier>();
    std::vector<std::shared_ptr<IFileChecker>> checkers = {
        std::make_shared<FileChecker>(FileChecker::Mode::Existence),  // создание / удаление
        std::make_shared<FileChecker>(FileChecker::Mode::Size),        // изменение размера
        std::make_shared<FileChecker>(FileChecker::Mode::Restoration)  // восстановление
    };

    FileMonitor monitor(std::move(checkers), notifier, config);

    Logger::getInstance().log(LogLevel::INFO, "File Monitor started");
    std::cout << "Commands: add <path>, remove <path>, start, stop, list, quit\n";

    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        if (cmd == "add") {
            std::string path; iss >> path;
            if (!path.empty()) monitor.addFile(path);
        } else if (cmd == "remove") {
            std::string path; iss >> path;
            if (!path.empty()) monitor.removeFile(path);
        } else if (cmd == "start") {
            monitor.start();
        } else if (cmd == "stop") {
            monitor.stop();
        } else if (cmd == "list") {
            monitor.listFiles();
        } else if (cmd == "quit" || cmd == "exit") {
            break;
        } else if (!cmd.empty()) {
            std::cout << "Unknown command: " << cmd << "\n";
        }
    }
    return 0;
}
