#pragma once
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include "IFileChecker.h"
#include "INotifier.h"

class FileMonitor {
public:
    FileMonitor(std::vector<std::shared_ptr<IFileChecker>> checkers,
                std::shared_ptr<INotifier> notifier);
    ~FileMonitor();

    bool addFile(const std::string& path);
    bool removeFile(const std::string& path);
    void start();
    void stop();
    void listFiles() const;

private:
    std::vector<std::string> files_;
    std::vector<std::shared_ptr<IFileChecker>> checkers_;
    std::shared_ptr<INotifier> notifier_;
    std::thread worker_;
    std::atomic<bool> running_;
    mutable std::mutex mutex_;

    void run();
    void checkFiles();
};
