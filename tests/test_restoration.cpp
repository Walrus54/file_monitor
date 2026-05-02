#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include "checkers/RestorationChecker.h"

namespace fs = std::filesystem;
static const std::string TEST_FILE = "/tmp/fm_test_restoration.txt";

class RestorationCheckerTest : public ::testing::Test {
protected:
    void SetUp() override {
        fs::remove(TEST_FILE);
        checker_ = std::make_unique<RestorationChecker>();
    }
    void TearDown() override { fs::remove(TEST_FILE); }
    std::unique_ptr<RestorationChecker> checker_;
};

TEST_F(RestorationCheckerTest, NoEventOnFirstAbsent) {
    EXPECT_FALSE(checker_->hasChanged(TEST_FILE));
}
TEST_F(RestorationCheckerTest, NoEventOnFirstPresent) {
    { std::ofstream f(TEST_FILE); f << "data"; }
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
    { std::ofstream f(TEST_FILE); f << "restored"; }
    EXPECT_TRUE(checker_->hasChanged(TEST_FILE));
    EXPECT_EQ(checker_->getEvent(TEST_FILE).description, "restored");
}
TEST_F(RestorationCheckerTest, NoEventWhileAbsent) {
    { std::ofstream f(TEST_FILE); f << "data"; }
    checker_->hasChanged(TEST_FILE);
    fs::remove(TEST_FILE);
    checker_->hasChanged(TEST_FILE);
    EXPECT_FALSE(checker_->hasChanged(TEST_FILE));
}
