#pragma once

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define Q_DECL_EXPORT __declspec(dllexport)
#  define Q_DECL_IMPORT __declspec(dllimport)
#else
#  define Q_DECL_EXPORT     __attribute__((visibility("default")))
#  define Q_DECL_IMPORT     __attribute__((visibility("default")))
#endif

#if defined(TSPSOLVERLIB_LIBRARY)
#  define TSPSOLVERLIB_EXPORT Q_DECL_EXPORT
#else
#  define TSPSOLVERLIB_EXPORT Q_DECL_IMPORT
#endif


#include <vector>
#include <queue>

#include "matrix.h"
#include "staticthreadpool.h"
#include "branchandbound.h"

namespace tspsolver {

    struct optimalTour_t {
        double weight;
        std::vector<int> path;
    };

    struct TspSolver {
        virtual optimalTour_t solve() = 0;
    };

    template <typename T>
    class BBSolver : public TspSolver {
    public:

        BBSolver(SquareMatrix<T> matrix, size_t threadCount = 1) : m_Matrix(std::move(matrix)) {
            m_ThreadPool.start(threadCount);
        }

        optimalTour_t solve() override {
            auto reduceTask = [](SquareMatrix<T>& matrix) -> T {
                return bb::reduceMatrix(matrix);
            };

            auto branchTask =[]() {

            };

            m_ThreadPool.submit(reduceTask(m_Matrix));

        }

    private:

        void handleReduceTask(bb::node_t<T> result) {

        }

        void handleBranchTask() {

        }

    private:
        SquareMatrix<T> m_Matrix;
        StaticThreadPool m_ThreadPool;
        std::priority_queue<bb::node_t<T>, bb::nodeComparator_t<T>> m_NodesStorage;
    };

}
