#include <gtest/gtest.h>

#include <bb/branchandbound.h>

TEST(BBTest, penaltiesTest) {
    tspsolver::SquareMatrix<int> matrix(3);
    matrix.at(0, 0) = 3;
    matrix.at(0, 1) = 2;
    matrix.at(0, 2) = std::numeric_limits<int>::max();

    matrix.at(1, 0) = 6;
    matrix.at(1, 1) = 4;
    matrix.at(1, 2) = 9;

    matrix.at(2, 0) = 1;
    matrix.at(2, 1) = 12;
    matrix.at(2, 2) = 3;

    std::vector<tspsolver::bb::Edge> expected = {{2, 2}};

    tspsolver::bb::reduceMatrix(matrix);
    std::vector<tspsolver::bb::Edge> penalties = tspsolver::bb::findMaxNullPenalties(matrix);

    EXPECT_EQ(penalties, expected);
}