#include <gtest/gtest.h>

#include <branchandbound.h>

TEST(BBTest, getMinExceptTest) {
    std::vector<int> v = {6, 3, 0, 5, 4, 2};

    EXPECT_EQ(tspsolver::bb::getMinExcept<int>(v.begin(), v.end(), v.begin() + 2), 2);
}