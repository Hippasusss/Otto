#i








#include "pch.h"
#include "gtest/gtest.h"
#include "RingBuffer.h"

class RingBufferTest : public ::testing::Test {
protected:
    void SetUp() override {
        rb = std::make_unique<RingBufferVector<int>>(5);
    }
    std::unique_ptr<RingBufferVector<int>> rb;
};

TEST_F(RingBufferTest, ReadsEmptyBuffer) {
    EXPECT_TRUE(rb->readAllValues().empty());
}

TEST_F(RingBufferTest, ReadsSingleElement) {
    rb->writeValue(42);
    auto result = rb->readAllValues();
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], 42);
}

TEST_F(RingBufferTest, ReadsFullBuffer) {
    for (int i = 0; i < 5; ++i) rb->writeValue(i);
    auto result = rb->readAllValues();
    ASSERT_EQ(result.size(), 5);
}

TEST_F(RingBufferTest, HandlesWrapAround) {
    // Force wrap-around
    for (int i = 0; i < 3; ++i) rb->writeValue(i);
    rb->readAllValues();
    for (int i = 10; i < 15; ++i) rb->writeValue(i);
    auto result = rb->readAllValues();
    ASSERT_EQ(result.size(), 5);
}

TEST_F(RingBufferTest, MaintainsOrder) {
    rb->writeValue(1);
    rb->writeValue(2);
    rb->readAllValues();
    rb->writeValue(3);
    rb->writeValue(4);
    auto result = rb->readAllValues();
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], 3);
    EXPECT_EQ(result[1], 4);
}
