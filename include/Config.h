#pragma once
#include <string>
#include <unordered_map>
#include <mutex>

class Config {
public:
    static Config& getInstance();

    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    void set(const std::string& key, const std::string& value);
    std::string get(const std::string& key, const std::string& defaultVal = "") const;
    int getInt(const std::string& key, int defaultVal = 0) const;
    void loadFromFile(const std::string& path);

private:
    Config();
    std::unordered_map<std::string, std::string> data_;
    mutable std::mutex mutex_;
};
