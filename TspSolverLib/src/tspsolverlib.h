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

        BBSolver(SquareMatrix<T> matrix, size_t threadCount = 1) {
            m_ThreadPool.start(threadCount);
            m_RootNode.matrixWrapper.matrix = std::move(matrix);
            m_RootNode.weight = bb::reduceMatrix(m_RootNode.matrixWrapper.matrix);
            m_NodesStorage.push(m_RootNode);
        }

        optimalTour_t solve() override {
            while(m_NodesStorage.top().matrixWrapper.matrix.size()  > 2) {
                T currentMinWeight = m_NodesStorage.top().weight;
                std::vector<bb::node_t<T>> nodesToSplit;
                while (!m_NodesStorage.empty() && m_NodesStorage.top().weight == currentMinWeight) {
                    nodesToSplit.push_back(m_NodesStorage.top());
                    m_NodesStorage.pop();
                }

                std::vector<std::future<std::vector<bb::Edge>>> possibleEdges;
                possibleEdges.reserve(nodesToSplit.size());
                for(auto& node : nodesToSplit) {
                    possibleEdges.push_back(m_ThreadPool.submit([&node = node]() {
                        bb::findNullsTask<T> task;
                        return task(node);
                    }));
                }

                std::vector<std::future<bb::node_t<T>>> children;

                for(size_t i = 0; i < possibleEdges.size(); ++i) {
                    for(auto& edge : possibleEdges[i].get()) {
                        children.push_back(m_ThreadPool.submit([&node = nodesToSplit[i], &edge = edge](){
                            bb::createLeftBranch<T> task;
                            return task(node, edge);
                        }));
                    }
                }
            }

            return optimalTour_t {};
        }

    private:

    private:
        bb::node_t<T> m_RootNode;
        StaticThreadPool m_ThreadPool;
        std::priority_queue<bb::node_t<T>, std::vector<bb::node_t<T>>, bb::nodeComparator_t<T>> m_NodesStorage;
    };

}
