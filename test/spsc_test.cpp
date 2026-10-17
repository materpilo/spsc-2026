#include <cstddef>
#include <gtest/gtest.h>
#include <memory>
#include <utility>

#include "spsc.hpp"

TEST(SpscTests, TestExecutableRuns)
{
  constexpr std::size_t Size = 10;
  constexpr int expectedValue = 5;
  spsc::Queue<int, Size> queue{};
  queue.push(int{ expectedValue });
  ASSERT_EQ(queue.pop(), expectedValue);
}

/**
 * Test boundaries on a small queue of size 2 - 1 element available.
 * Push and pop single element on every position in the queue and check the count after each operation.
 */
TEST(SpscTests, CapacityBoundarySmallQueue)
{
  constexpr std::size_t Size = 2U;
  static constexpr int value_top_push{ 1234 };

  using TestQueue = spsc::Queue<int, Size>;
  TestQueue queue{};
  ASSERT_EQ(queue.Count(), 0U);

  for (std::size_t index = 0U; index < Size; ++index) {
    queue.push(int{ value_top_push });
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
  constexpr int expectedValue = 42;
  spsc::Queue<std::unique_ptr<int>, Size> queue{};
  auto value = std::make_unique<int>(expectedValue);

  ASSERT_EQ(queue.Count(), 0U);
  queue.push(std::move(value));

  EXPECT_EQ(value, nullptr);
  ASSERT_EQ(queue.Count(), 1U);

  const std::unique_ptr<int> result = queue.pop();
  ASSERT_NE(result, nullptr);
  EXPECT_EQ(*result, expectedValue);
  EXPECT_EQ(queue.Count(), 0U);
}
