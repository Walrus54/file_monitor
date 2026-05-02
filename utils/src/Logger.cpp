#include "utils/Logger.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

Logger& Logger::getInstance() {
    // Meyers singleton: static local инициализируется ровно один раз (C++11 §6.7)
    static Logger instance;
    return instance;
}

Logger::Logger() : toFile_(false) {}

Logger::~Logger() {
    if (file_.is_open()) file_.close();
}

void Logger::setOutputFile(const std::string& path) {
    std::lock_guard<std::mutex> lock(mutex_);
    file_.open(path, std::ios::app);
    toFile_ = file_.is_open();
}

void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::string entry = "[" + timestamp() + "] [" + levelToString(level) + "] " + message;
    std::cout << entry << "\n";
    // Дублируем в файл, если он открыт
    if (toFile_ && file_.is_open()) file_ << entry << "\n";
}

std::string Logger::levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::INFO:    return "INFO";
        case LogLevel::WARNING: return "WARN";
        case LogLevel::ERROR:   return "ERROR";
    }
    return "UNKNOWN";
}

std::string Logger::timestamp() {
    auto now = std::chrono::system_clock::now();
    auto t   = std::chrono::system_clock::to_time_t(now);
    // localtime_r вместо localtime: thread-safe (буфер на стеке)
    struct tm tm_buf{};
    localtime_r(&t, &tm_buf);
    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}
