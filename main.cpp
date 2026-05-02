/**
 * @file main.cpp
 * @brief Точка входа и composition root приложения.
 *
 * Здесь создаются конкретные типы и подключаются слоты к сигналам.
 * FileMonitor ничего не знает о том, кто обрабатывает его сигнал.
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
    Config config = (argc > 1) ? Config::fromFile(argv[1]) : Config{};

    std::vector<std::shared_ptr<IFileChecker>> checkers = {
        std::make_shared<FileChecker>(FileChecker::Mode::Existence),
        std::make_shared<FileChecker>(FileChecker::Mode::Size),
        std::make_shared<FileChecker>(FileChecker::Mode::Restoration)
    };

    FileMonitor monitor(std::move(checkers), config);

    // Подключаем слот: ConsoleNotifier::notify вызывается при каждом fileChanged
    auto notifier = std::make_shared<ConsoleNotifier>();
    monitor.fileChanged.connect([notifier](const FileEvent& e) {
        notifier->notify(e);
    });

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
