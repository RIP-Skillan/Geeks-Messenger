#include <gtest/gtest.h>

// Basic example: checking an arithmetic condition
TEST(MathTest, AdditionWorks) {
    int result = 2 + 3;
    EXPECT_EQ(result, 5);
}
