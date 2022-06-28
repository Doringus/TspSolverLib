#include <gtest/gtest.h>

#include <ga/crossovers.h>
/*
TEST(GATest, posCrossoverMiddleTest) {
    std::vector<int> p1 = {1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<int> p2 = {2, 4, 6, 8, 7, 5, 3, 1};
    std::vector<size_t> positions = {1, 5, 2};


    std::vector<int> c1 = {1, 4, 6, 2, 3, 5, 7, 8};
    std::vector<int> c2 = {4, 2, 3, 8, 7, 6, 5, 1};

    auto offsping = tspsolver::ga::posCrossover(p1, p2, positions);

    EXPECT_EQ(offsping.first, c1);
    EXPECT_EQ(offsping.second, c2);
}

TEST(GATest, posCrossoverBeginTest) {
    std::vector<int> p1 = {1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<int> p2 = {2, 4, 6, 8, 7, 5, 3, 1};
    std::vector<size_t> positions = {0, 1, 2};


    std::vector<int> c1 = {2, 4, 6, 1, 3, 5, 7, 8};
    std::vector<int> c2 = {1, 2, 3, 4, 6, 8, 7, 5};

    auto offsping = tspsolver::ga::posCrossover(p1, p2, positions);

    EXPECT_EQ(offsping.first, c1);
    EXPECT_EQ(offsping.second, c2);
}

TEST(GATest, posCrossoverEndTest) {
    std::vector<int> p1 = {1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<int> p2 = {2, 4, 6, 8, 7, 5, 3, 1};
    std::vector<size_t> positions = {5, 6, 7};


    std::vector<int> c1 = {2, 4, 6, 7, 8, 5, 3, 1};
    std::vector<int> c2 = {2, 4, 5, 3, 1, 6, 7, 8};

    auto offsping = tspsolver::ga::posCrossover(p1, p2, positions);

    EXPECT_EQ(offsping.first, c1);
    EXPECT_EQ(offsping.second, c2);
}

TEST(GATest, ox2CrossoverTest) {
    std::vector<int> p1 = {1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<int> p2 = {2, 4, 6, 8, 7, 5, 3, 1};
    std::vector<size_t> positions = {1, 2, 5};


    std::vector<int> c1 = {1, 2, 3, 4, 6, 5, 7, 8};
    std::vector<int> c2 = {2, 4, 3, 8, 7, 5, 6, 1};

    auto offsping = tspsolver::ga::ox2Crossover(p1, p2, positions);

    EXPECT_EQ(offsping.first, c1);
    EXPECT_EQ(offsping.second, c2);
}

TEST(GATest, pmxCrossoverTest) {
    std::vector<int> p1 = {1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<int> p2 = {3, 7, 5, 1, 6, 8, 2, 4};
    std::vector<size_t> positions = {1, 2, 5};


    std::vector<int> c1 = {4, 2, 3, 1, 6, 8, 7, 5};
    std::vector<int> c2 = {3, 7, 8, 4, 5, 6, 2, 1};

    auto offsping = tspsolver::ga::pmxCrossover(p1, p2, 3, 5);

    EXPECT_EQ(offsping.first, c1);
    EXPECT_EQ(offsping.second, c2);
}*/