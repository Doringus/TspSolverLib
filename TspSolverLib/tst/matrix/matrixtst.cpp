#include <gtest/gtest.h>

#include <algorithm>

#include <matrix.h>

TEST(MatrixTest, matrixFillTest) {
    tspsolver::SquareMatrix<int> matrix(2);
    matrix.at(0, 0) = 1;
    matrix.at(0, 1) = 2;
    matrix.at(1, 0) = 3;
    matrix.at(1, 1) = 4;

    EXPECT_EQ(matrix.at(0, 0), 1);
    EXPECT_EQ(matrix.at(1, 1), 4);
    EXPECT_EQ(matrix.size(), 2);
}

TEST(MatrixTest, matrixRowIteratorTest) {
    tspsolver::SquareMatrix<int> matrix(2);
    matrix.at(0, 0) = 1;
    matrix.at(0, 1) = 2;
    matrix.at(1, 0) = 3;
    matrix.at(1, 1) = 4;

    std::vector<int> expected = {1, 2};

    std::vector<int> row;
    std::copy(matrix.rowBegin(0), matrix.rowEnd(0), std::back_inserter(row));

    EXPECT_EQ(expected, row);
}

TEST(MatrixTest, matrixColumnIteratorTest) {
    tspsolver::SquareMatrix<int> matrix(2);
    matrix.at(0, 0) = 1;
    matrix.at(0, 1) = 2;
    matrix.at(1, 0) = 3;
    matrix.at(1, 1) = 4;

    std::vector<int> expected = {1, 3};

    std::vector<int> column;
    std::copy(matrix.columnBegin(0), matrix.columnEnd(0), std::back_inserter(column));

    EXPECT_EQ(expected, column);
}
