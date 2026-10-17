#include <cstddef>
#include <gtest/gtest.h>

#include "spsc.hpp"

TEST(SpscTests, TestExecutableRuns)
{
  constexpr std::size_t Size = 10;
  spsc::Queue<int, Size> queue;
  queue.push(5);
  ASSERT_EQ(queue.pop(), 5);
}
