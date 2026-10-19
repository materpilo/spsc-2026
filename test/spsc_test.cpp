#include <cstddef>
#include <gtest/gtest.h>
#include <memory>
#include <thread>
#include <utility>

#include "spsc.hpp"

/**
 * Test basic functionality of the queue with a single push and pop operation.
 */
TEST(SpscTests, SunnyScenario)
{
  constexpr std::size_t Size = 10;
  constexpr int ExpectedValue = 5;
  spsc::Queue<int, Size> queue{};
  queue.push(ExpectedValue);
  ASSERT_EQ(queue.pop(), ExpectedValue);
}

/**
 * Test boundaries on a small queue of size 2, where 1 element is available.
 * Push and pop single element on every position in the queue and check the count after each operation.
 */
TEST(SpscTests, CapacityBoundarySmallQueue)
{
  constexpr std::size_t Size = 2U;
  static constexpr int ValueTopPush{ 1234 };

  using TestQueue = spsc::Queue<int, Size>;
  TestQueue queue{};
  ASSERT_EQ(queue.Count(), 0U);

  for (std::size_t index = 0U; index < Size; ++index) {
    queue.push(ValueTopPush);
    ASSERT_EQ(queue.Count(), 1U);
    queue.pop();
    ASSERT_EQ(queue.Count(), 0U);
  }
}


/**
 * Test boundaries on a queue of underlying storage size 10.
 * Fill all elements, then remove them.
 * Validate the count after each operation.
 */
TEST(SpscTests, CapacityBoundary)
{
  constexpr std::size_t Size = 10U;
  constexpr std::size_t Capacity = Size - 1U;

  using TestQueue = spsc::Queue<int, Size>;
  TestQueue queue{};

  EXPECT_EQ(queue.Capacity(), Capacity);

  for (std::size_t index = 0U; index < Capacity; ++index) {
    queue.push(static_cast<int>(index));
    ASSERT_EQ(queue.Count(), index + 1U);
  }

  ASSERT_EQ(queue.Count(), Capacity);

  for (std::size_t index = 0U; index < Capacity; ++index) {
    ASSERT_EQ(queue.pop(), static_cast<int>(index));
    ASSERT_EQ(queue.Count(), Capacity - index - 1U);
  }

  ASSERT_EQ(queue.Count(), 0U);
}

/**
 * Test that the queue can handle move-only types, specifically std::unique_ptr.
 */
TEST(SpscTests, MoveOnlyValue)
{
  constexpr std::size_t Size = 2U;
  constexpr int ExpectedValue = 42;
  spsc::Queue<std::unique_ptr<int>, Size> queue{};

  auto value = std::make_unique<int>(ExpectedValue);

  ASSERT_EQ(queue.Count(), 0U);
  queue.push(std::move(value));

  EXPECT_EQ(value, nullptr);
  ASSERT_EQ(queue.Count(), 1U);

  const std::unique_ptr<int> result = queue.pop();
  ASSERT_NE(result, nullptr);
  EXPECT_EQ(*result, ExpectedValue);
  EXPECT_EQ(queue.Count(), 0U);
}

/**
 * Template helpers used by stress tests for various types contained by queue.
 * They operate on an integer index value and corresponding representation in given type.
 */

// Create a value of type T from an integer.
template<class T>
T FromInt(int value);
// Check if value of type T is equal to value created from given integer.
template<class T>
bool IsEqualToInt(int expected_value, const T &test_value);

// Specializations for 'int'
template<>
int FromInt<int>(int value)
{
  return value;
}
template<>
bool IsEqualToInt<int>(int expected_value, const int &test_value)
{
  return test_value == expected_value;
}

// Specializations for 'std::unique_ptr<int>'
template<>
std::unique_ptr<int> FromInt<std::unique_ptr<int>>(int value)
{
  return std::make_unique<int>(value);
}
template<>// precondition: test_value is not nullptr
bool IsEqualToInt<std::unique_ptr<int>>(int expected_value, const std::unique_ptr<int> &test_value)
{
  return *test_value == expected_value;
}

struct Foo
{
  int a;
  int b;
  int c;
};

// Specializations for 'struct Foo'
template<>
Foo FromInt<Foo>(int value)
{
  return Foo{ .a = value, .b = value, .c = value };
}

template<>
bool IsEqualToInt<Foo>(int expected_value, const Foo &test_value)
{
  return (test_value.a == expected_value) && (test_value.b == expected_value) && (test_value.c == expected_value);
}


template<class T>
class SpscStressTest : public ::testing::Test
{
};

using SpscStressTypes = ::testing::Types<int, std::unique_ptr<int>, Foo>;
TYPED_TEST_SUITE(SpscStressTest, SpscStressTypes);

TYPED_TEST(SpscStressTest, ConcurrentProducerConsumer)
{
  constexpr std::size_t Size = 64U;
  constexpr int InsertionCount = 1'000'000;
  spsc::Queue<TypeParam, Size> queue{};

  std::thread producer([&queue] {
    for (int index = 0; index < InsertionCount; ++index) {
      queue.push(FromInt<TypeParam>(index));
    }
  });

  for (int index = 0; index < InsertionCount; ++index) {
    const TypeParam actual_value = queue.pop();
    if (!IsEqualToInt<TypeParam>(index, actual_value)) {
      // FAIL() exits function. Destruction of the producer thread object will
      // terminate the program. Not most graceful but simple and deterministic.
      FAIL() << "First mismatch at index " << index;
    }
  }

  producer.join();

  EXPECT_EQ(queue.Count(), 0U);
}
