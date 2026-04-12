#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "checkers/ExistenceChecker.h"
#include "checkers/SizeChecker.h"

namespace fs = std::filesystem;
static const std::string TEST_FILE = "/tmp/fm_test_checker.txt";

class ExistenceCheckerTest : public ::testing::Test {
protected:
    void SetUp() override {
        fs::remove(TEST_FILE);
        checker_ = std::make_unique<ExistenceChecker>();
    }
    void TearDown() override { fs::remove(TEST_FILE); }
    std::unique_ptr<ExistenceChecker> checker_;
};

TEST_F(ExistenceCheckerTest, FirstCallNoChange) {
    EXPECT_FALSE(checker_->hasChanged(TEST_FILE));
}

TEST_F(ExistenceCheckerTest, DetectsCreation) {
    checker_->hasChanged(TEST_FILE);
    { std::ofstream f(TEST_FILE); f << "hello"; }
    EXPECT_TRUE(checker_->hasChanged(TEST_FILE));
}

TEST_F(ExistenceCheckerTest, DetectsDeletion) {
    { std::ofstream f(TEST_FILE); f << "hello"; }
    checker_->hasChanged(TEST_FILE);
    fs::remove(TEST_FILE);
    EXPECT_TRUE(checker_->hasChanged(TEST_FILE));
}

class SizeCheckerTest : public ::testing::Test {
protected:
    void SetUp() override {
        { std::ofstream f(TEST_FILE); f << "initial"; }
        checker_ = std::make_unique<SizeChecker>();
    }
    void TearDown() override { fs::remove(TEST_FILE); }
    std::unique_ptr<SizeChecker> checker_;
};

TEST_F(SizeCheckerTest, FirstCallNoChange) {
    EXPECT_FALSE(checker_->hasChanged(TEST_FILE));
}

TEST_F(SizeCheckerTest, DetectsGrowth) {
    checker_->hasChanged(TEST_FILE);
    { std::ofstream f(TEST_FILE, std::ios::app); f << " more content"; }
    EXPECT_TRUE(checker_->hasChanged(TEST_FILE));
}
