#include <iostream>
#include <string>
#include <sstream>
#include "FileMonitor.h"

int main() {
    FileMonitor monitor;
    std::cout << "File Monitor v0.1\n"
              << "Commands: add <path>, remove <path>, start, stop, list, quit\n";
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
