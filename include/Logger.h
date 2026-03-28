#pragma once
#include <string>
#include <fstream>
#include <mutex>

enum class LogLevel { INFO, WARNING, ERROR };

class Logger {
public:
    static Logger& getInstance();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void log(LogLevel level, const std::string& message);
    void setOutputFile(const std::string& path);

private:
    Logger();
    ~Logger();

    std::ofstream file_;
    bool toFile_;
    std::mutex mutex_;

    static std::string levelToString(LogLevel level);
    static std::string timestamp();
};
