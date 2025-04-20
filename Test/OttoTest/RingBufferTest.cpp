






#include "pch.h"
#include "gtest/gtest.h"
#include "RingBuffer.h"

class RingBufferTest : public ::testing::Test 
{
protected:
    void SetUp() override 
    {
        rb = std::make_unique<RingBufferVector<int>>(bufferSize);
    }
    std::unique_ptr<RingBufferVector<int>> rb;
    const size_t bufferSize = 5;
};

TEST_F(RingBufferTest, ReadsSingleElement) 
{
    rb->writeValue(42);
    auto result = rb->readAllValues();
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], 42);
}

TEST_F(RingBufferTest, ReadsFullBuffer) 
{
    for (int i = 0; i < 5; ++i) rb->writeValue(i);
    auto result = rb->readAllValues();
    ASSERT_EQ(result.size(), 5);
}

TEST_F(RingBufferTest, HandlesWrapAround) 
{
    // Force wrap-around
    for (int i = 0; i < 3; ++i) rb->writeValue(i);
    auto firsResult = rb->readAllValues();
    ASSERT_EQ(firsResult.size(), 3);
    for (int i = 10; i < 15; ++i) rb->writeValue(i);
    auto result = rb->readAllValues();
    ASSERT_EQ(result.size(), 5);
}

TEST_F(RingBufferTest, MaintainsOrder) 
{
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

TEST_F(RingBufferTest, ReadPreviousValues_PartiallyFilled) 
{
    rb->writeValue(10);
    rb->writeValue(20);
    
    std::vector<int> result(2);
    rb->readPreviousValues(result);
    
    ASSERT_EQ(result[0], 10);
    ASSERT_EQ(result[1], 20);
}

TEST_F(RingBufferTest, ReadPreviousValues_FullyFilledNoWrap) 
{
    for (int i = 1; i <= bufferSize; ++i) 
    {
        rb->writeValue(i*10);
    }
    
    std::vector<int> result(bufferSize);
    rb->readPreviousValues(result);
    
    for (int i = 0; i < bufferSize; ++i) 
    {
        ASSERT_EQ(result[i], (i+1)*10);
    }
}

TEST_F(RingBufferTest, ReadPreviousValues_WrappedBuffer) 
{
    
    // Fill buffer completely
    for (int i = 1; i <= bufferSize; ++i) 
    {
        rb->writeValue(i*10);
    }
    // Add one more to cause wrap
    rb->writeValue(60);
    
    std::vector<int> result(bufferSize);
    rb->readPreviousValues(result);
    
    ASSERT_EQ(result[0], 20);
    ASSERT_EQ(result[1], 30);
    ASSERT_EQ(result[2], 40);
    ASSERT_EQ(result[3], 50);
    ASSERT_EQ(result[4], 60);
}


