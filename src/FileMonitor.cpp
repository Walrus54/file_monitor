#include "FileMonitor.h"
#include <iostream>
#include <filesystem>
#include <chrono>
#include <algorithm>

namespace fs = std::filesystem;

FileMonitor::FileMonitor() : running_(false) {}

FileMonitor::~FileMonitor() { stop(); }

void FileMonitor::addFile(const std::string& path) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (const auto& f : files_)
        if (f.path == path) return;
    files_.push_back(snapshot(path));
    std::cout << "[INFO] Watching: " << path << "\n";
}

bool FileMonitor::removeFile(const std::string& path) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = std::remove_if(files_.begin(), files_.end(),
        [&](const FileState& f){ return f.path == path; });
    if (it == files_.end()) return false;
    files_.erase(it, files_.end());
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
    for (const auto& f : files_)
        std::cout << (f.exists ? "[+]" : "[-]") << " " << f.path << " " << f.size << "B\n";
}

void FileMonitor::run() {
    while (running_) {
        checkFiles();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void FileMonitor::checkFiles() {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& f : files_) {
        auto cur = snapshot(f.path);
        if (cur.exists != f.exists) {
            std::cout << "[EVENT] " << f.path << (cur.exists ? ": created" : ": deleted") << "\n";
        } else if (cur.exists && cur.size != f.size) {
            std::cout << "[EVENT] " << f.path << ": modified ("
                      << f.size << " -> " << cur.size << " bytes)\n";
        }
        f = cur;
    }
}

FileMonitor::FileState FileMonitor::snapshot(const std::string& path) const {
    bool ex = fs::exists(path);
    return {path, ex, ex ? fs::file_size(path) : 0};
}
