#include <gtest/gtest.h>

#include <branchandbound.h>

TEST(BBTest, reducePlainArrayTest) {
    std::vector<int> v = {4, 2, 5, 8};

    std::vector<int> expected = {2, 0, 3, 6};

    int reduce = tspsolver::bb::reducePlainArray<int>(v.begin(), v.end());

    EXPECT_EQ(v, expected);
    EXPECT_EQ(2, reduce);
}

TEST(BBTest, reducePlainArrayWithInfinityTest) {
    std::vector<int> v = {4, std::numeric_limits<int>::max(), 5, 8};

    std::vector<int> expected = {0, std::numeric_limits<int>::max(), 1, 4};

    int reduce = tspsolver::bb::reducePlainArray<int>(v.begin(), v.end());


    EXPECT_EQ(v, expected);
    EXPECT_EQ(4, reduce);
}