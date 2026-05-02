#include "utils/Config.h"
#include <fstream>

Config Config::fromFile(const std::string& path) {
    Config cfg;
    std::ifstream f(path);
    if (!f.is_open()) return cfg; // файл не найден — возвращаем дефолты

    std::string line;
    while (std::getline(f, line)) {
        if (line.empty() || line[0] == '#') continue; // пропускаем комментарии
        auto pos = line.find('=');
        if (pos == std::string::npos) continue;

        const std::string key = line.substr(0, pos);
        const std::string val = line.substr(pos + 1);

        if (key == "poll_interval") {
            try { cfg.pollInterval = std::stoi(val); } catch (...) {}
        } else if (key == "log_level") {
            cfg.logLevel = val;
        }
    }
    return cfg;
}
