#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include "core/FileMonitor.hpp"
#include "notifiers/ConsoleNotifier.hpp"
#include "checkers/ExistenceChecker.hpp"
#include "checkers/SizeChecker.hpp"

class FileMonitorTest : public ::testing::Test {
protected:
    void SetUp() override {
        notifier_ = std::make_shared<ConsoleNotifier>();
        checkers_ = {
            std::make_shared<ExistenceChecker>(),
            std::make_shared<SizeChecker>()
        };
        monitor_ = std::make_unique<FileMonitor>(checkers_, notifier_);
    }

    std::shared_ptr<ConsoleNotifier> notifier_;
    std::vector<std::shared_ptr<IFileChecker>> checkers_;
    std::unique_ptr<FileMonitor> monitor_;
};

TEST_F(FileMonitorTest, AddFileReturnsTrue) {
    EXPECT_TRUE(monitor_->addFile("/tmp/test_add.txt"));
}

TEST_F(FileMonitorTest, AddDuplicateReturnsFalse) {
    monitor_->addFile("/tmp/test_dup.txt");
    EXPECT_FALSE(monitor_->addFile("/tmp/test_dup.txt"));
}

TEST_F(FileMonitorTest, RemoveNonExistentReturnsFalse) {
    EXPECT_FALSE(monitor_->removeFile("/tmp/nonexistent.txt"));
}

TEST_F(FileMonitorTest, AddThenRemoveReturnsTrue) {
    monitor_->addFile("/tmp/test_rem.txt");
    EXPECT_TRUE(monitor_->removeFile("/tmp/test_rem.txt"));
}

TEST_F(FileMonitorTest, StartStop) {
    monitor_->start();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    monitor_->stop();
}
