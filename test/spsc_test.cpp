#include <cstddef>
#include <gtest/gtest.h>
#include <memory>
#include <thread>
#include <utility>

#include "spsc.hpp"

TEST(SpscTests, TestExecutableRuns)
{
  constexpr std::size_t Size = 10;
  constexpr int ExpectedValue = 5;
  spsc::Queue<int, Size> queue{};
  queue.push(int{ ExpectedValue });
  ASSERT_EQ(queue.pop(), ExpectedValue);
}

/**
 * Test boundaries on a small queue of size 2 - 1 element available.
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
    queue.push(int{ ValueTopPush });
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
  using TestQueue = spsc::Queue<int, Size>;
  TestQueue queue{};

  for (std::size_t index = 0U; index < TestQueue::Capacity(); ++index) {
    ASSERT_LT(queue.Count(), TestQueue::Capacity());
    queue.push(static_cast<int>(index));
    ASSERT_EQ(queue.Count(), index + 1U);
  }

  ASSERT_EQ(queue.Count(), TestQueue::Capacity());

  for (std::size_t index = 0U; index < TestQueue::Capacity(); ++index) {
    ASSERT_GT(queue.Count(), 0U);
    ASSERT_EQ(queue.pop(), static_cast<int>(index));
    ASSERT_EQ(queue.Count(), TestQueue::Capacity() - index - 1U);
  }

  ASSERT_EQ(queue.Count(), 0U);
}

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

// Templates used by test for various types.

// Create a value of type T from an index.
template<class T>
T FromIndex(std::size_t);
// Check if value of type T is equal as it was created from given index
template<class T>
bool IsEqualToIndex(std::size_t index, const T &value);

// int
template<>
int FromIndex<int>(std::size_t value)
{
  return static_cast<int>(value);
}
template<>
bool IsEqualToIndex<int>(std::size_t index, const int &value)
{
  return static_cast<std::size_t>(value) == index;
}

// std::unique_ptr<int>
template<>
std::unique_ptr<int> FromIndex<std::unique_ptr<int>>(std::size_t value)
{
  return std::make_unique<int>(static_cast<int>(value));
}

template<>
bool IsEqualToIndex<std::unique_ptr<int>>(std::size_t index, const std::unique_ptr<int> &value)
{
  return value != nullptr && static_cast<std::size_t>(*value) == index;
}

template<class TestedType>
void RunConcurrentProducerConsumerStress()
{
  constexpr std::size_t Size = 64U;
  constexpr std::size_t InsertionCount = 1'000'000U;
  spsc::Queue<TestedType, Size> queue{};

  std::thread producer([&queue] {
    for (std::size_t index = 0U; index < InsertionCount; ++index) {
      queue.push(FromIndex<TestedType>(index));
    }
  });

  bool values_match = true;
  std::size_t first_mismatch_index = InsertionCount;
  for (std::size_t index = 0U; index < InsertionCount; ++index) {
    const TestedType actual_value = queue.pop();
    if (!IsEqualToIndex<TestedType>(index, actual_value) && values_match) {
      values_match = false;
      first_mismatch_index = index;
    }
  }

  producer.join();

  EXPECT_TRUE(values_match) << "First mismatch at index " << first_mismatch_index;
  EXPECT_EQ(queue.Count(), 0U);
}

TEST(SpscTests, ConcurrentProducerConsumerStress)
{
  RunConcurrentProducerConsumerStress<int>();
}

TEST(SpscTests, ConcurrentProducerConsumerStressUniquePtr)
{
  RunConcurrentProducerConsumerStress<std::unique_ptr<int>>();
}
