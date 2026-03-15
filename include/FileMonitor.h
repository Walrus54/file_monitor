#pragma once
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>

class FileMonitor {
public:
    FileMonitor();
    ~FileMonitor();
    void addFile(const std::string& path);
    bool removeFile(const std::string& path);
    void start();
    void stop();
    void listFiles() const;

private:
    struct FileState {
        std::string path;
        bool exists;
        uintmax_t size;
    };
    std::vector<FileState> files_;
    std::thread worker_;
    std::atomic<bool> running_;
    mutable std::mutex mutex_;

    void run();
    void checkFiles();
    FileState snapshot(const std::string& path) const;
};
