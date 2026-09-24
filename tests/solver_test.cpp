#include "solver.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <iostream>
#include <limits>
#include <sstream>
#include <vector>

namespace {

class CoutCapture {
  public:
    CoutCapture() : previous_(std::cout.rdbuf(buffer_.rdbuf())) {}

    ~CoutCapture() { std::cout.rdbuf(previous_); }

  private:
    std::ostringstream buffer_;
    std::streambuf *previous_;
};

countdown::Value solveQuietly(std::vector<countdown::Value> &numbers, countdown::Value target) {
    CoutCapture capture;
    return countdown::solve(numbers, target, numbers.front());
}

} // namespace

TEST(SolverTest, FindsExactTargetUsingTwoNumbers) {
    std::vector<countdown::Value> numbers{2, 3};

    EXPECT_EQ(solveQuietly(numbers, 6), 6);
}

TEST(SolverTest, AllowsUnusedInputs) {
    std::vector<countdown::Value> numbers{2, 3, 100};

    EXPECT_EQ(solveQuietly(numbers, 6), 6);
}

TEST(SolverTest, FindsExactTargetUsingACombinationOfNumbers) {
    std::vector<countdown::Value> numbers{2, 3, 8};

    EXPECT_EQ(solveQuietly(numbers, 19), 19);
}

TEST(SolverTest, ReturnsClosestResultWhenTargetCannotBeReached) {
    std::vector<countdown::Value> numbers{2, 4};

    EXPECT_EQ(solveQuietly(numbers, 100), 8);
}

TEST(SolverTest, SupportsModulo) {
    std::vector<countdown::Value> numbers{10, 3};

    EXPECT_EQ(solveQuietly(numbers, 1), 1);
}

TEST(SolverTest, HandlesLargeProductsWithoutOverflow) {
    std::vector<countdown::Value> numbers{100, 100, 100, 100, 100, 100};

    EXPECT_EQ(solveQuietly(numbers, 500), 500);
}

TEST(SolverTest, ComputesDistanceWithoutSignedOverflow) {
    using Value = countdown::Value;

    EXPECT_EQ(countdown::absoluteDifference(5, 3), 2u);
    EXPECT_EQ(countdown::absoluteDifference(3, 5), 2u);
    EXPECT_EQ(countdown::absoluteDifference(std::numeric_limits<Value>::min(), std::numeric_limits<Value>::max()),
              std::numeric_limits<std::uint64_t>::max());
}
