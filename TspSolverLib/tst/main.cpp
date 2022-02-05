#include <iostream>

#include <gtest/gtest.h>

#include <tspsolverlib.h>

int main(int argc, char **argv) {
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

   // tspsolver::BBSolver solver(matrix, 3);
   // solver.solve();

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
