#include <gtest/gtest.h>
#include "spsc.hpp"

TEST(SpscTests, TestExecutableRuns)
{
    spsc::Queue<int> queue;
    queue.push(5);
    ASSERT_EQ(queue.pop(), 5);
}
