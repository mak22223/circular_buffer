#include <gtest/gtest.h>
#include <assert.h>

#include "circularbuffer.hpp"

// Test settings
using ElementType = uint8_t;
constexpr size_t BufferSize = 10U;

class CircularBufferTestSuite : public ::testing::Test
{
public:
  static void SetUpTestSuite()
  {

  }

  static void TearDownTestSuite()
  {

  }

public:
  CircularBuffer<ElementType, BufferSize> d_buffer;
};

TEST_F(CircularBufferTestSuite, GetCorrectSize)
{
  EXPECT_EQ(BufferSize, d_buffer.size());
}

TEST_F(CircularBufferTestSuite, PutEmptyData)
{
  ElementType testData[] = "testdata";
  static_assert(sizeof(testData) <= BufferSize);
  EXPECT_TRUE(d_buffer.put(testData, sizeof(testData)));

  ElementType emptyData[BufferSize];
  memset(emptyData, 'a', sizeof(emptyData));
  EXPECT_TRUE(d_buffer.put(emptyData, BufferSize));
  EXPECT_FALSE(d_buffer.put((const unsigned char*)"", BufferSize + 1));

  EXPECT_EQ(sizeof(testData), d_buffer.used());

  ElementType bufData[sizeof(testData)];
  EXPECT_TRUE(d_buffer.get(sizeof(testData), bufData));
  EXPECT_EQ(0, memcmp(testData, bufData, sizeof(testData)));
}

TEST_F(CircularBufferTestSuite, PutHalfLengthData)
{
  ElementType emptyData[BufferSize];
  memset(emptyData, 'a', BufferSize / 2);
  EXPECT_TRUE(d_buffer.put(emptyData, BufferSize));
}

TEST_F(CircularBufferTestSuite, PutFullLengthData)
{
  ElementType emptyData[BufferSize];
  memset(emptyData, 'a', BufferSize);
  EXPECT_TRUE(d_buffer.put(emptyData, BufferSize));
}

TEST_F(CircularBufferTestSuite, PutTooMuchData)
{
  ElementType emptyData[BufferSize];
  memset(emptyData, 'a', BufferSize);
  EXPECT_FALSE(d_buffer.put(emptyData, BufferSize));
}

TEST_F(CircularBufferTestSuite, PeekData)
{
  ElementType testData[] = "testdata";
  static_assert(sizeof(testData) <= BufferSize);

  d_buffer.put(testData, sizeof(testData));

  ElementType bufData[sizeof(testData)];
  EXPECT_TRUE(d_buffer.peek(sizeof(testData), bufData));
  EXPECT_STREQ((char*)testData, (char*)bufData);
}

TEST_F(CircularBufferTestSuite, GetUsed)
{
  EXPECT_EQ(0, d_buffer.used());

  ElementType testData[] = "testdata";
  static_assert(sizeof(testData) <= BufferSize);
  EXPECT_TRUE(d_buffer.put(testData, sizeof(testData)));

  EXPECT_EQ(sizeof(testData), d_buffer.used());
}
