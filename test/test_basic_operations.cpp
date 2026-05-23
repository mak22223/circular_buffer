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
  EXPECT_TRUE(buf.put(nullptr, 0));
  EXPECT_TRUE(buf.empty());
}

TEST_F(CircularBufferBasicOp, PutTooMuchData)
{
  EXPECT_FALSE(buf.put(oversizeTestData, sizeof(oversizeTestData)));
  EXPECT_TRUE(buf.empty());
}

// TEST_F(CircularBufferBasicOp, GetDataNullptr)
// {
//   EXPECT_TRUE(buf.put(testData, sizeof(testData)));
//   EXPECT_FALSE(buf.get(5, nullptr));
// }

TEST_F(CircularBufferBasicOp, GetDataWhileEmptyBuffer)
{
  ElementType temp[sizeof(testData)];
  EXPECT_FALSE(buf.get(sizeof(testData), temp));
}

TEST_F(CircularBufferBasicOp, GetZeroData)
{
  EXPECT_TRUE(buf.get(0, nullptr)); // Should return true even if nullptr
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

TEST_F(CircularBufferBasicOp, AvailableDataPartialLoad)
{
  EXPECT_EQ(BufferSize, buf.available());
  EXPECT_TRUE(buf.put(oversizeTestData, BufferSize));
  EXPECT_EQ(0, buf.available());
  ElementType temp[BufferSize];
  EXPECT_TRUE(buf.get(BufferSize, temp));
  EXPECT_EQ(BufferSize, buf.available());
}

TEST_F(CircularBufferBasicOp, AvailableDataFullLoad)
{
  EXPECT_EQ(BufferSize, buf.available());
  EXPECT_TRUE(buf.put(testData, sizeof(testData)));
  EXPECT_EQ(BufferSize - sizeof(testData), buf.available());
  ElementType temp[sizeof(testData)];
  EXPECT_TRUE(buf.get(sizeof(testData), temp));
  EXPECT_EQ(BufferSize, buf.available());
}

// TEST_F(CircularBufferBasicOp, )
// {

// }

// TEST_F(CircularBufferBasicOp, )
// {

// }

// TEST_F(CircularBufferBasicOp, )
// {

// }

// TEST_F(CircularBufferBasicOp, )
// {

// }

// TEST_F(CircularBufferBasicOp, )
// {

// }

// TEST_F(CircularBufferBasicOp, )
// {

// }




}