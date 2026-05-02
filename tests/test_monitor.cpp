#include <atomic>
#include <chrono>
#include <gtest/gtest.h>
#include <thread>
#include "core/FileMonitor.h"
#include "core/IFileChecker.h"

// ─── Мок-чекеры (без файловой системы) ─────────────────────────────────────

/// Всегда сообщает об изменении — для проверки что сигнал испускается
struct AlwaysChecker : public IFileChecker {
    bool hasChanged(const std::string&) override { return true; }
    FileEvent getEvent(const std::string& p) const override { return {p, "mock changed"}; }
};

/// Никогда не сообщает об изменении — для проверки тишины
struct NeverChecker : public IFileChecker {
    bool hasChanged(const std::string&) override { return false; }
    FileEvent getEvent(const std::string& p) const override { return {p, "mock"}; }
};

// Конфиг с нулевым интервалом — поток крутится без задержки
static Config fastConfig() {
    Config c;
    c.pollInterval = 0;
    return c;
}

// ─── Тесты addFile / removeFile ─────────────────────────────────────────────

TEST(FileMonitorFiles, AddNewPathReturnsTrue) {
    FileMonitor m({std::make_shared<NeverChecker>()}, fastConfig());
    EXPECT_TRUE(m.addFile("a.txt"));
}

TEST(FileMonitorFiles, AddDuplicatePathReturnsFalse) {
    FileMonitor m({std::make_shared<NeverChecker>()}, fastConfig());
    m.addFile("a.txt");
    EXPECT_FALSE(m.addFile("a.txt"));
}

TEST(FileMonitorFiles, RemoveUnknownPathReturnsFalse) {
    FileMonitor m({std::make_shared<NeverChecker>()}, fastConfig());
    EXPECT_FALSE(m.removeFile("unknown.txt"));
}

TEST(FileMonitorFiles, RemoveKnownPathReturnsTrue) {
    FileMonitor m({std::make_shared<NeverChecker>()}, fastConfig());
    m.addFile("a.txt");
    EXPECT_TRUE(m.removeFile("a.txt"));
}

TEST(FileMonitorFiles, RemovedPathCanBeAddedAgain) {
    FileMonitor m({std::make_shared<NeverChecker>()}, fastConfig());
    m.addFile("a.txt");
    m.removeFile("a.txt");
    EXPECT_TRUE(m.addFile("a.txt"));
}

// ─── Тесты сигнала ──────────────────────────────────────────────────────────

TEST(FileMonitorSignal, EmittedWhenCheckerFires) {
    FileMonitor m({std::make_shared<AlwaysChecker>()}, fastConfig());

    std::atomic<int> fired{0};
    m.fileChanged.connect([&](const FileEvent&) { fired++; });
    m.addFile("dummy");

    m.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    m.stop();

    EXPECT_GT(fired.load(), 0);
}

TEST(FileMonitorSignal, SilentWhenCheckerDoesNotFire) {
    FileMonitor m({std::make_shared<NeverChecker>()}, fastConfig());

    std::atomic<int> fired{0};
    m.fileChanged.connect([&](const FileEvent&) { fired++; });
    m.addFile("dummy");

    m.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    m.stop();

    EXPECT_EQ(fired.load(), 0);
}

TEST(FileMonitorSignal, EventContainsCorrectPath) {
    FileMonitor m({std::make_shared<AlwaysChecker>()}, fastConfig());

    std::string receivedPath;
    m.fileChanged.connect([&](const FileEvent& e) { receivedPath = e.path; });
    m.addFile("expected.txt");

    m.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    m.stop();

    EXPECT_EQ(receivedPath, "expected.txt");
}

TEST(FileMonitorSignal, NoEmitForUnregisteredFile) {
    FileMonitor m({std::make_shared<AlwaysChecker>()}, fastConfig());

    std::atomic<int> fired{0};
    m.fileChanged.connect([&](const FileEvent&) { fired++; });
    // Намеренно не добавляем файл

    m.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    m.stop();

    EXPECT_EQ(fired.load(), 0);
}
