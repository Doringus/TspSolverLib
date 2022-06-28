#include <gtest/gtest.h>

#include <tspsolverlib.h>

TEST(BBTest, solverTest) {
    tspsolver::SquareMatrix<int> matrix(5);
    matrix.at(0, 0) = tspsolver::infinity<int>;
    matrix.at(0, 1) = 20;
    matrix.at(0, 2) = 18;
    matrix.at(0, 3) = 12;
    matrix.at(0, 4) = 8;


    matrix.at(1, 0) = 5;
    matrix.at(1, 1) = tspsolver::infinity<int>;
    matrix.at(1, 2) = 14;
    matrix.at(1, 3) = 7;
    matrix.at(1, 4) = 11;

    matrix.at(2, 0) = 12;
    matrix.at(2, 1) = 18;
    matrix.at(2, 2) = tspsolver::infinity<int>;
    matrix.at(2, 3) = 6;
    matrix.at(2, 4) = 11;

    matrix.at(3, 0) = 11;
    matrix.at(3, 1) = 17;
    matrix.at(3, 2) = 11;
    matrix.at(3, 3) = tspsolver::infinity<int>;;
    matrix.at(3, 4) = 12;

    matrix.at(4, 0) = 5;
    matrix.at(4, 1) = 5;
    matrix.at(4, 2) = 5;
    matrix.at(4, 3) = 5;
    matrix.at(4, 4) = tspsolver::infinity<int>;;

    tspsolver::BBSolver solver(matrix, 3);

    auto result = solver.solve();

    EXPECT_DOUBLE_EQ(result.weight, 41);
}