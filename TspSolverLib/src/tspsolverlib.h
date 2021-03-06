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
#include <chrono>

#include "matrix.h"
#include "threading/staticthreadpool.h"
#include "bb/branchandbound.h"

namespace tspsolver {

    struct optimalTour_t {
        double weight;
        std::vector<size_t> path;
    };

    struct TspSolver {
        virtual optimalTour_t solve() = 0;
    };

    template <typename T>
    class BBSolver : public TspSolver {
    public:

        BBSolver(SquareMatrix<T> matrix, size_t threadCount = 1) {
            m_ThreadPool.start(threadCount);
            bb::MatrixIndicesWrapper<T> wrapper(matrix);
            T weight = bb::reduceMatrix(wrapper.getMatrix());
            bb::Node<T> rootNode({}, std::move(wrapper), bb::Path(), weight);
            m_NodesStorage.push(std::move(rootNode));
        }

        ~BBSolver() {
            m_ThreadPool.shutdown();
        }

        optimalTour_t solve() override {
            while(m_NodesStorage.top().getMatrix().size()  > 2) {
                T currentMinWeight = m_NodesStorage.top().getWeight();
           //     std::cout << "Current min: " << currentMinWeight << "\n";
                std::vector<bb::Node<T>> nodesToSplit;

                while (!m_NodesStorage.empty() && m_NodesStorage.top().getWeight() == currentMinWeight) {
                    nodesToSplit.push_back(m_NodesStorage.top());
                    m_NodesStorage.pop();
                }


                std::vector<std::future<std::vector<bb::Edge>>> possibleEdges;
                possibleEdges.reserve(nodesToSplit.size());
                for(auto& node : nodesToSplit) {
                    possibleEdges.push_back(m_ThreadPool.submit([&node = node]() {
                        return bb::findNullsTask(node);
                    }));
                }

                std::vector<std::future<bb::Node<T>>> children;

                for(size_t i = 0; i < possibleEdges.size(); ++i) {
                    auto edges = possibleEdges[i].get();
                    for(auto edge : edges) {
                        bb::Node<T>& node = nodesToSplit.at(i);
                        children.push_back(m_ThreadPool.submit([](bb::Node<T>& node, const bb::Edge& edge){
                            return bb::createLeftBranch(node, edge);
                        }, std::ref(node), edge));

                        children.push_back(m_ThreadPool.submit([](bb::Node<T>& node, const bb::Edge& edge){
                            return bb::createRightBranch(node, edge);
                        }, std::ref(node), edge));
                    }
                }

                for(auto& child : children) {
                    m_NodesStorage.push(child.get());
                }
            }

            auto node = m_NodesStorage.top();

            for(size_t i = 0; i < 2; ++i) {
                for(size_t j = 0; j < 2; ++j) {
                    if(node.getMatrix().at(i, j) != infinity<T>) {
                        node.getIncludedEdges().push_back({node.getMatrixWrapper().getRowLogicalIndex(i),
                                                          node.getMatrixWrapper().getColumnLogicalIndex(j)});
                    }


                }
            }


            auto path = buildPath(node.getIncludedEdges());

            return { node.getWeight(), path };
        }

    private:

        std::vector<size_t> buildPath(const std::vector<bb::Edge>& edges) {
            std::vector<size_t> result;
            result.push_back(edges.at(0).first);
            result.push_back(edges.at(0).second);
            size_t pushedEdges = 1;

            while(pushedEdges < edges.size() - 1) {
                int lastIndex = result.back();
                auto it = std::find_if(edges.begin(), edges.end(), [lastIndex](const bb::Edge& edge){
                    return lastIndex == edge.first;
                });
                result.push_back((*it).second);
                pushedEdges++;
            }
            return result;
        }

    private:
        StaticThreadPool m_ThreadPool;
        std::priority_queue<bb::Node<T>, std::vector<bb::Node<T>>, bb::nodeComparator_t<T>> m_NodesStorage;
    };

}
