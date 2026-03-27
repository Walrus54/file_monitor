#include "FileMonitor.h"
#include <iostream>
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
    std::cout << "[INFO] Watching: " << path << "\n";
    return true;
}

bool FileMonitor::removeFile(const std::string& path) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = std::find(files_.begin(), files_.end(), path);
    if (it == files_.end()) return false;
    files_.erase(it);
    std::cout << "[INFO] Removed: " << path << "\n";
    return true;
}

void FileMonitor::start() {
    if (running_.exchange(true)) return;
    worker_ = std::thread(&FileMonitor::run, this);
    std::cout << "[INFO] Monitoring started\n";
}

void FileMonitor::stop() {
    if (!running_.exchange(false)) return;
    if (worker_.joinable()) worker_.join();
    std::cout << "[INFO] Monitoring stopped\n";
}

void FileMonitor::listFiles() const {
    std::lock_guard<std::mutex> lock(mutex_);
    if (files_.empty()) { std::cout << "No files\n"; return; }
    for (const auto& path : files_) std::cout << "  " << path << "\n";
}

void FileMonitor::run() {
    while (running_) {
        checkFiles();
        std::this_thread::sleep_for(std::chrono::seconds(1));
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
