#include "Config.h"
#include <fstream>

Config& Config::getInstance() {
    static Config instance;
    return instance;
}

Config::Config() {
    data_["poll_interval"] = "1";
    data_["log_level"] = "INFO";
}

void Config::set(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> lock(mutex_);
    data_[key] = value;
}

std::string Config::get(const std::string& key, const std::string& defaultVal) const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = data_.find(key);
    return (it != data_.end()) ? it->second : defaultVal;
}

int Config::getInt(const std::string& key, int defaultVal) const {
    std::string val = get(key);
    if (val.empty()) return defaultVal;
    try { return std::stoi(val); }
    catch (...) { return defaultVal; }
}

void Config::loadFromFile(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) return;
    std::string line;
    while (std::getline(f, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto pos = line.find('=');
        if (pos == std::string::npos) continue;
        set(line.substr(0, pos), line.substr(pos + 1));
    }
}
