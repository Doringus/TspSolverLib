#include <gtest/gtest.h>

#include <bb/branchandbound.h>

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

    tspsolver::bb::Node<int> node;
    tspsolver::bb::MatrixIndicesWrapper<int> wrapper(matrix);
    node.setMatrixWrapper(wrapper);
    node.setWeight(tspsolver::bb::reduceMatrix(wrapper.getMatrix()));

    auto result = tspsolver::bb::createLeftBranch(node, {0, 1});

    EXPECT_EQ(result.getMatrix().at(0, 1), std::numeric_limits<int>::max());
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


    tspsolver::bb::MatrixIndicesWrapper<int> wrapper(matrix);
    tspsolver::bb::Node<int> node({}, wrapper, 0);

    auto result = tspsolver::bb::createRightBranch(node, {1, 1});

    tspsolver::SquareMatrix<int> expected(2);
    expected.at(0, 0) = 0;
    expected.at(0, 1) = 0;

    expected.at(1, 0) = 0;
    expected.at(1, 1) = 0;


    EXPECT_EQ(result.getMatrix().size(), 2);
    EXPECT_EQ(result.getWeight(), 1);
    EXPECT_EQ(expected, result.getMatrix());
}