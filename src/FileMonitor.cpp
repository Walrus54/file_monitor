#include "FileMonitor.h"
#include "Logger.h"
#include "Config.h"
#include <algorithm>
#include <chrono>

FileMonitor::FileMonitor(std::vector<std::shared_ptr<IFileChecker>> checkers,
                         std::shared_ptr<INotifier> notifier)
    : checkers_(std::move(checkers)), notifier_(std::move(notifier)), running_(false) {}

FileMonitor::~FileMonitor() { stop(); }

bool FileMonitor::addFile(const std::string& path) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (std::find(files_.begin(), files_.end(), path) != files_.end()) return false;
    files_.push_back(path);
    Logger::getInstance().log(LogLevel::INFO, "Watching: " + path);
    return true;
}

bool FileMonitor::removeFile(const std::string& path) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = std::find(files_.begin(), files_.end(), path);
    if (it == files_.end()) return false;
    files_.erase(it);
    Logger::getInstance().log(LogLevel::INFO, "Removed: " + path);
    return true;
}

void FileMonitor::start() {
    if (running_.exchange(true)) return;
    worker_ = std::thread(&FileMonitor::run, this);
    Logger::getInstance().log(LogLevel::INFO, "Monitoring started");
}

void FileMonitor::stop() {
    if (!running_.exchange(false)) return;
    if (worker_.joinable()) worker_.join();
    Logger::getInstance().log(LogLevel::INFO, "Monitoring stopped");
}

void FileMonitor::listFiles() const {
    std::lock_guard<std::mutex> lock(mutex_);
    if (files_.empty()) {
        Logger::getInstance().log(LogLevel::INFO, "No files monitored");
        return;
    }
    for (const auto& path : files_)
        Logger::getInstance().log(LogLevel::INFO, "  " + path);
}

void FileMonitor::run() {
    int interval = Config::getInstance().getInt("poll_interval", 1);
    while (running_) {
        checkFiles();
        std::this_thread::sleep_for(std::chrono::seconds(interval));
    }
}

void FileMonitor::checkFiles() {
    std::lock_guard<std::mutex> lock(mutex_);
    for (const auto& path : files_) {
        for (auto& checker : checkers_) {
            if (checker->hasChanged(path)) {
                notifier_->notify(checker->getEvent(path));
            }
        }
    }
}
