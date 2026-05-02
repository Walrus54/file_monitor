#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include "checkers/FileChecker.h"

namespace fs = std::filesystem;
static const std::string TEST_FILE = "/tmp/fm_test_checker.txt";

// ─── Режим Existence ────────────────────────────────────────────────────────

class ExistenceCheckerTest : public ::testing::Test {
protected:
    void SetUp() override {
        fs::remove(TEST_FILE);
        checker_ = std::make_unique<FileChecker>(FileChecker::Mode::Existence);
    }
    void TearDown() override { fs::remove(TEST_FILE); }
    std::unique_ptr<FileChecker> checker_;
};

TEST_F(ExistenceCheckerTest, FirstCallNoChange) {
    EXPECT_FALSE(checker_->hasChanged(TEST_FILE));
}
TEST_F(ExistenceCheckerTest, DetectsCreation) {
    checker_->hasChanged(TEST_FILE);
    { std::ofstream f(TEST_FILE); f << "hello"; }
    EXPECT_TRUE(checker_->hasChanged(TEST_FILE));
    EXPECT_EQ(checker_->getEvent(TEST_FILE).description, "created");
}
TEST_F(ExistenceCheckerTest, DetectsDeletion) {
    { std::ofstream f(TEST_FILE); f << "hello"; }
    checker_->hasChanged(TEST_FILE);
    fs::remove(TEST_FILE);
    EXPECT_TRUE(checker_->hasChanged(TEST_FILE));
    EXPECT_EQ(checker_->getEvent(TEST_FILE).description, "deleted");
}

// ─── Режим Size ─────────────────────────────────────────────────────────────

class SizeCheckerTest : public ::testing::Test {
protected:
    void SetUp() override {
        { std::ofstream f(TEST_FILE); f << "initial"; }
        checker_ = std::make_unique<FileChecker>(FileChecker::Mode::Size);
    }
    void TearDown() override { fs::remove(TEST_FILE); }
    std::unique_ptr<FileChecker> checker_;
};

TEST_F(SizeCheckerTest, FirstCallNoChange) {
    EXPECT_FALSE(checker_->hasChanged(TEST_FILE));
}
TEST_F(SizeCheckerTest, DetectsGrowth) {
    checker_->hasChanged(TEST_FILE);
    { std::ofstream f(TEST_FILE, std::ios::app); f << " more"; }
    EXPECT_TRUE(checker_->hasChanged(TEST_FILE));
}

// ─── Режим Restoration ──────────────────────────────────────────────────────

class RestorationCheckerTest : public ::testing::Test {
protected:
    void SetUp() override {
        fs::remove(TEST_FILE);
        checker_ = std::make_unique<FileChecker>(FileChecker::Mode::Restoration);
    }
    void TearDown() override { fs::remove(TEST_FILE); }
    std::unique_ptr<FileChecker> checker_;
};

TEST_F(RestorationCheckerTest, NoEventOnFirstAbsent) {
    EXPECT_FALSE(checker_->hasChanged(TEST_FILE));
}
TEST_F(RestorationCheckerTest, DetectsDeletion) {
    { std::ofstream f(TEST_FILE); f << "data"; }
    checker_->hasChanged(TEST_FILE);
    fs::remove(TEST_FILE);
    EXPECT_TRUE(checker_->hasChanged(TEST_FILE));
    EXPECT_EQ(checker_->getEvent(TEST_FILE).description, "deleted — watching for restoration");
}
TEST_F(RestorationCheckerTest, DetectsRestoration) {
    { std::ofstream f(TEST_FILE); f << "data"; }
    checker_->hasChanged(TEST_FILE);
    fs::remove(TEST_FILE);
    checker_->hasChanged(TEST_FILE);
    { std::ofstream f(TEST_FILE); f << "back"; }
    EXPECT_TRUE(checker_->hasChanged(TEST_FILE));
    EXPECT_EQ(checker_->getEvent(TEST_FILE).description, "restored");
}
