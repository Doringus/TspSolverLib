#include <gtest/gtest.h>

#include <branchandbound.h>

TEST(BBTest, reduceMatrix) {
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

    tspsolver::SquareMatrix<int> expected(3);
    expected.at(0, 0) = 1;
    expected.at(0, 1) = 0;
    expected.at(0, 2) = std::numeric_limits<int>::max();

    expected.at(1, 0) = 2;
    expected.at(1, 1) = 0;
    expected.at(1, 2) = 3;

    expected.at(2, 0) = 0;
    expected.at(2, 1) = 11;
    expected.at(2, 2) = 0;

    int reduce = tspsolver::bb::reduceMatrix(matrix);

    EXPECT_EQ(9, reduce);
}