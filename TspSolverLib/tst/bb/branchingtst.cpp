#include <gtest/gtest.h>

#include <branchandbound.h>

TEST(BBTest, leftBranchTest) {
    tspsolver::SquareMatrix<int> matrix(3);
    matrix.at(0, 0) = 3;
    matrix.at(0, 1) = 2;
    matrix.at(0, 2) = 5;

    matrix.at(1, 0) = 6;
    matrix.at(1, 1) = 4;
    matrix.at(1, 2) = 9;

    matrix.at(2, 0) = 1;
    matrix.at(2, 1) = 12;
    matrix.at(2, 2) = 3;

    tspsolver::bb::node_t<int> node;
    node.matrixWrapper.matrix = matrix;
    node.matrixWrapper.rowLogicalIndices = {0, 1, 2};
    node.matrixWrapper.columnLogicalIndices = {0, 1, 2};
    node.weight = tspsolver::bb::reduceMatrix(matrix);

    tspsolver::bb::createLeftBranch<int> task;
    auto result = task(node, {0, 1});

    EXPECT_EQ(result.matrixWrapper.matrix.at(0, 1), std::numeric_limits<int>::max());
    EXPECT_EQ(result.matrixWrapper.columnLogicalIndices, node.matrixWrapper.columnLogicalIndices);
    EXPECT_EQ(result.matrixWrapper.rowLogicalIndices, node.matrixWrapper.rowLogicalIndices);
}

TEST(BBTest, rightBranchTest) {
    tspsolver::SquareMatrix<int> matrix(3);
    matrix.at(0, 0) = 1;
    matrix.at(0, 1) = 0;
    matrix.at(0, 2) = 1;

    matrix.at(1, 0) = 2;
    matrix.at(1, 1) = 0;
    matrix.at(1, 2) = 3;

    matrix.at(2, 0) = 0;
    matrix.at(2, 1) = 11;
    matrix.at(2, 2) = 0;

    tspsolver::bb::node_t<int> node;
    node.matrixWrapper.matrix = matrix;
    node.matrixWrapper.rowLogicalIndices = {0, 1, 2};
    node.matrixWrapper.columnLogicalIndices = {0, 1, 2};

    tspsolver::bb::createRightBranch<int> task;
    auto result = task(node, {1, 1});

    tspsolver::SquareMatrix<int> expected(2);
    expected.at(0, 0) = 0;
    expected.at(0, 1) = 0;

    expected.at(1, 0) = 0;
    expected.at(1, 1) = 0;


    EXPECT_EQ(result.matrixWrapper.matrix.size(), 2);
    EXPECT_EQ(result.weight, 1);
    EXPECT_EQ(expected, result.matrixWrapper.matrix);
}