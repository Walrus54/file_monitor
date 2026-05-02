#include <gtest/gtest.h>
#include "core/Signal.h"

// Чистые юнит-тесты Signal<T> — нет ФС, нет потоков, нет зависимостей

TEST(Signal, EmitCallsConnectedSlot) {
    Signal<int> sig;
    int received = -1;
    sig.connect([&](int v) { received = v; });

    sig.emit(42);

    EXPECT_EQ(received, 42);
}

TEST(Signal, EmitWithNoSlotsDoesNotThrow) {
    Signal<int> sig;
    EXPECT_NO_THROW(sig.emit(1));
}

TEST(Signal, AllSlotsCalledOnEmit) {
    Signal<int> sig;
    int a = 0, b = 0;
    sig.connect([&](int v) { a = v; });
    sig.connect([&](int v) { b = v * 2; });

    sig.emit(5);

    EXPECT_EQ(a, 5);
    EXPECT_EQ(b, 10);
}

TEST(Signal, SlotsCalledInConnectOrder) {
    Signal<int> sig;
    std::vector<int> order;
    sig.connect([&](int) { order.push_back(1); });
    sig.connect([&](int) { order.push_back(2); });
    sig.connect([&](int) { order.push_back(3); });

    sig.emit(0);

    ASSERT_EQ(order.size(), 3u);
    EXPECT_EQ(order[0], 1);
    EXPECT_EQ(order[1], 2);
    EXPECT_EQ(order[2], 3);
}

TEST(Signal, MultipleEmitsCallSlotEachTime) {
    Signal<int> sig;
    int count = 0;
    sig.connect([&](int) { count++; });

    sig.emit(0);
    sig.emit(0);
    sig.emit(0);

    EXPECT_EQ(count, 3);
}

TEST(Signal, PassesStructArgument) {
    Signal<FileEvent> sig;
    FileEvent received;
    sig.connect([&](const FileEvent& e) { received = e; });

    sig.emit(FileEvent{"path.txt", "deleted"});

    EXPECT_EQ(received.path, "path.txt");
    EXPECT_EQ(received.description, "deleted");
}
