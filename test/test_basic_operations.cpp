#include <gtest/gtest.h>

#include <circularbuffer.hpp>

// Test constants
using ElementType = uint8_t;
constexpr size_t BufferSize = 10U;
const ElementType testData[] = "testdata";
const ElementType oversizeTestData[] = "oversizetestdata";


namespace {

class CircularBufferBasicOp : public testing::Test
{
public:
  CircularBuffer<ElementType, BufferSize> buf;

};

TEST_F(CircularBufferBasicOp, DefaultConstructor)
{
  EXPECT_TRUE(buf.empty());
  EXPECT_FALSE(buf.full());
  EXPECT_EQ(BufferSize, buf.size());
  EXPECT_EQ(BufferSize, buf.available());
  EXPECT_EQ(0U, buf.used());
}

TEST_F(CircularBufferBasicOp, PutData)
{
  EXPECT_TRUE(buf.put(testData, sizeof(testData)));
  EXPECT_EQ(sizeof(testData), buf.used());
  ElementType temp[sizeof(testData)];
  buf.get(sizeof(testData), temp);
  EXPECT_EQ(0, memcmp(testData, temp, sizeof(testData) * sizeof(ElementType)));
}

TEST_F(CircularBufferBasicOp, PutEmptyData)
{
  EXPECT_FALSE(buf.put(nullptr, 0));
  EXPECT_TRUE(buf.empty());
}

TEST_F(CircularBufferBasicOp, PutTooMuchData)
{
  EXPECT_FALSE(buf.put(oversizeTestData, sizeof(oversizeTestData)));
  EXPECT_TRUE(buf.empty());
}

TEST_F(CircularBufferBasicOp, GetDataNullptr)
{
  EXPECT_TRUE(buf.put(testData, sizeof(testData)));
  EXPECT_FALSE(buf.get(5, nullptr));
}

TEST_F(CircularBufferBasicOp, GetDataWhileEmptyBuffer)
{
  ElementType temp[sizeof(testData)];
  EXPECT_FALSE(buf.get(sizeof(testData), temp));
}

TEST_F(CircularBufferBasicOp, GetZeroData)
{
  ElementType temp[BufferSize];
  EXPECT_TRUE(buf.get(0, temp));
  EXPECT_FALSE(buf.get(0, nullptr));
}

TEST_F(CircularBufferBasicOp, GetDataExactly)
{
  EXPECT_TRUE(buf.put(testData, sizeof(testData)));

  ElementType temp[sizeof(testData)];
  EXPECT_TRUE(buf.get(sizeof(testData), temp));

  EXPECT_EQ(0, memcmp(testData, temp, sizeof(testData) * sizeof(ElementType)));
  EXPECT_EQ(0, buf.used());
}

TEST_F(CircularBufferBasicOp, GetDataTwoTakes)
{
  EXPECT_TRUE(buf.put(testData, sizeof(testData) / 2 * 2));

  ElementType temp[sizeof(testData)];
  EXPECT_TRUE(buf.get(sizeof(testData) / 2, temp));
  EXPECT_EQ(0, memcmp(testData, temp, sizeof(testData) / 2 * sizeof(ElementType)));
  EXPECT_TRUE(buf.get(sizeof(testData) / 2, temp));
  EXPECT_EQ(0, memcmp(testData + sizeof(testData) / 2, temp, sizeof(testData) / 2 * sizeof(ElementType)));

  EXPECT_EQ(0, buf.used());
}

TEST_F(CircularBufferBasicOp, GetTooMuchData)
{
  EXPECT_TRUE(buf.put(testData, sizeof(testData) / 2));

  ElementType temp[sizeof(testData)];
  EXPECT_FALSE(buf.get(sizeof(testData), temp));
}

TEST_F(CircularBufferBasicOp, AvailableDataFullLoad)
{
  EXPECT_EQ(BufferSize, buf.available());
  EXPECT_TRUE(buf.put(oversizeTestData, BufferSize));
  EXPECT_EQ(0, buf.available());
  ElementType temp[BufferSize];
  EXPECT_TRUE(buf.get(BufferSize, temp));
  EXPECT_EQ(BufferSize, buf.available());
}

TEST_F(CircularBufferBasicOp, AvailableDataPartialLoad)
{
  EXPECT_EQ(BufferSize, buf.available());
  EXPECT_TRUE(buf.put(testData, sizeof(testData)));
  EXPECT_EQ(BufferSize - sizeof(testData), buf.available());
  ElementType temp[sizeof(testData)];
  EXPECT_TRUE(buf.get(sizeof(testData), temp));
  EXPECT_EQ(BufferSize, buf.available());
}

TEST_F(CircularBufferBasicOp, PeekSomeData)
{
  EXPECT_TRUE(buf.put(testData, sizeof(testData)));
  size_t used_before = buf.used();
  ElementType actual[BufferSize];
  EXPECT_TRUE(buf.peek(sizeof(testData) / 2, actual));
  EXPECT_EQ(memcmp(actual, testData, sizeof(testData) / 2), 0);
  size_t used_after = buf.used();
  EXPECT_EQ(used_before, used_after);
}

TEST_F(CircularBufferBasicOp, PeekZeroData)
{
  ElementType ref[BufferSize] = {};
  ElementType temp[BufferSize] = {};
  EXPECT_TRUE(buf.peek(0, temp));
  EXPECT_EQ(memcmp(ref, temp, BufferSize), 0);
}

TEST_F(CircularBufferBasicOp, PeekDataNullptrBuffer)
{
  EXPECT_FALSE(buf.peek(0, nullptr));
  EXPECT_FALSE(buf.peek(BufferSize / 2, nullptr));
}

TEST_F(CircularBufferBasicOp, FindData)
{
  ASSERT_TRUE(buf.put(testData, sizeof(testData)));
  // data should have unique elements or this test might fail
  EXPECT_EQ(buf.find(testData[0]), 0);
  EXPECT_EQ(buf.find(testData[1]), 1);
  EXPECT_EQ(buf.find(testData[2]), 2);
}

TEST_F(CircularBufferBasicOp, GetTotalSize)
{
  EXPECT_EQ(buf.size(), BufferSize);
}

TEST_F(CircularBufferBasicOp, GetTotalSizeAfterDataInsertion)
{
  EXPECT_TRUE(buf.put(testData, sizeof(testData)));
  EXPECT_EQ(buf.size(), BufferSize);
}

TEST_F(CircularBufferBasicOp, GetAvailable)
{
  for (size_t i = 0; i < BufferSize; ++i) {
    EXPECT_EQ(buf.available(), BufferSize - i);
    EXPECT_TRUE(buf.put(testData, 1));
  }
  EXPECT_EQ(buf.available(), 0);
}

TEST_F(CircularBufferBasicOp, GetUsed)
{
  for (size_t i = 0; i < BufferSize; ++i) {
    EXPECT_EQ(buf.used(), i);
    EXPECT_TRUE(buf.put(testData, 1));
  }
  EXPECT_EQ(buf.used(), BufferSize);
}

TEST_F(CircularBufferBasicOp, GetEmpty)
{
  EXPECT_TRUE(buf.empty());
  for (size_t i = 0; i < BufferSize; ++i) {
    EXPECT_TRUE(buf.put(testData, 1));
    EXPECT_FALSE(buf.empty());
  }
}

TEST_F(CircularBufferBasicOp, GetFull)
{
  for (size_t i = 0; i < BufferSize; ++i) {
    EXPECT_FALSE(buf.full());
    EXPECT_TRUE(buf.put(testData, 1));
  }
  EXPECT_TRUE(buf.full());
}

TEST_F(CircularBufferBasicOp, ResetBuffer)
{
  EXPECT_TRUE(buf.put(testData, sizeof(testData)));
  EXPECT_FALSE(buf.empty());
  EXPECT_EQ(buf.used(), sizeof(testData));
  buf.reset();
  EXPECT_TRUE(buf.empty());
  EXPECT_EQ(buf.used(), 0);
}

TEST_F(CircularBufferBasicOp, BufferValidAfterReset)
{
  ASSERT_TRUE(buf.put(testData, sizeof(testData)));
  buf.reset();
  ASSERT_TRUE(buf.put(testData, sizeof(testData) / 2));
  EXPECT_EQ(buf.used(), sizeof(testData) / 2);
  ElementType actual[BufferSize];
  ElementType expected[BufferSize];
  memcpy(expected, testData, sizeof(testData) / 2 * sizeof(ElementType));
  ASSERT_TRUE(buf.get(sizeof(testData) / 2, actual));
  EXPECT_EQ(memcmp(actual, expected, sizeof(testData) / 2 * sizeof(ElementType)), 0);
}

}