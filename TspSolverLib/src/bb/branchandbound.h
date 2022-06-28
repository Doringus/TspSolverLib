#pragma once

#include <vector>
#include <concepts>
#include <algorithm>
#include <numeric>
#include <limits>
#include <optional>

#include "node.h"
#include "common.h"

namespace tspsolver { namespace  bb {



    template <typename Type, typename InputIt>
    constexpr Type getMinExcept(InputIt begin, InputIt end, InputIt except) {
        Type lhs = infinity<Type>;
        Type rhs = infinity<Type>;

        if(begin != except) {
            lhs = *std::min_element(begin, except);
        }
        if(end - 1 != except) {
            rhs = *std::min_element(except + 1, end);
        }

        return std::min(lhs, rhs);
    }

    template <typename Type, typename InputIt>
    constexpr double reducePlainArray(InputIt begin, InputIt end) {
        double min = *std::min_element(begin, end);
        std::for_each(begin, end, [min](auto& e) {
            if(e != infinity<Type>) {
                e -= min;
            }
        });

        return min;
    }

    template <typename Type>
    constexpr double reduceMatrix(SquareMatrix<Type>& matrix) {
        double reduceFactor = 0;
        for(size_t i = 0; i < matrix.size(); ++i) {
            auto r = reducePlainArray<Type>(matrix.rowBegin(i), matrix.rowEnd(i));
            if(r == infinity<Type>) {
                r = 0;
            }
            reduceFactor += r;
        }
        for(size_t i = 0; i < matrix.size(); ++i) {
            auto r = reducePlainArray<Type>(matrix.columnBegin(i), matrix.columnEnd(i));;
            if(r == infinity<Type>) {
                r = 0;
            }
            reduceFactor += r;
        }

        return reduceFactor;
    }

    template <typename Type>
    std::vector<Edge> findMaxNullPenalties(const SquareMatrix<Type>& matrix) {
        std::vector<Edge> result;

        struct penalty_t {
            Type weight;
            Edge edge;
        };

        std::vector<penalty_t> penalties;
        for(size_t i = 0; i < matrix.size(); ++i) {
            for(size_t j = 0; j < matrix.size(); ++j) {
                if(matrix.at(i, j) == 0) {
                    penalties.push_back({getMinExcept<Type>(matrix.rowBegin(i),
                                                  matrix.rowEnd(i),
                                                  matrix.rowBegin(i) + j)
                                     + getMinExcept<Type>(matrix.columnBegin(j),
                                                    matrix.columnEnd(j),
                                                    matrix.columnBegin(j) + i),
                                      {i, j}});
                }
            }
        }

        if(penalties.size() == 0) {
            return {};
        }

        Type maxWeight = (*std::max_element(penalties.begin(), penalties.end(), [](penalty_t& lhs, penalty_t& rhs){
                return lhs.weight < rhs.weight;
        })).weight;

        std::for_each(penalties.begin(), penalties.end(), [&result, maxWeight](penalty_t& penalty){
            if(penalty.weight == maxWeight) {
                result.push_back(penalty.edge);
            }
        });

        return result;
    }

    template <typename Type>
    constexpr SquareMatrix<Type> excludeEdge(const SquareMatrix<Type>& matrix, const Edge& edge) {
        SquareMatrix<Type> result(matrix);
        result.at(edge.first, edge.second) = infinity<Type>;
        return result;
    }

    template <typename Type>
    constexpr SquareMatrix<Type> includeEdge(const SquareMatrix<Type>& matrix, const Edge& edge) {
        SquareMatrix<Type> result(matrix.size() - 1);
        for(size_t i = 0, resultMatrixRow = 0; i < matrix.size(); ++i) {
            for(size_t j = 0, resultMatrixCol = 0; j < matrix.size(); ++j) {
                if(edge.first != i && edge.second != j) {
                    result.at(resultMatrixRow, resultMatrixCol) = matrix.at(i, j);
                    resultMatrixCol++;
                }
            }
            if(edge.first != i) {
                resultMatrixRow++;
            }
        }
        return result;
    }

    template <typename T>
    std::vector<Edge> findNullsTask(const Node<T>& node) {
        return findMaxNullPenalties(node.getMatrix());
    };

    template <typename T>
    Node<T> createLeftBranch(const Node<T>& node, const Edge& edge) {
        MatrixIndicesWrapper<T> wrapper(node.getMatrixWrapper().getRowIndices(),
                                        node.getMatrixWrapper().getColumnIndices());

        auto matrix = excludeEdge(node.getMatrix(), edge);
        auto weight = reduceMatrix(matrix);
        if(weight < infinity<double> - 1) {
            weight += node.getWeight();
        }
        wrapper.setMatrix(std::move(matrix));

        return Node<T>(node.getIncludedEdges(), std::move(wrapper), node.getPath(), weight);
    };

    /// Edge - physical coords in matrix
    template <typename T>
    Node<T> createRightBranch(Node<T>& node, const Edge& edge) {
        MatrixIndicesWrapper<T> wrapper(node.getMatrixWrapper().getRowIndices(),
                                        node.getMatrixWrapper().getColumnIndices());

        /// Place in path edge [rowLogicalIndex, columnLogicalIndex]
        size_t rowLogicalIndex = wrapper.getRowLogicalIndex(edge.first);
        size_t columnLogicalIndex = wrapper.getColumnLogicalIndex(edge.second);

        /// Remove edge
        auto matrix = includeEdge(node.getMatrix(), edge);

        wrapper.eraseRowIndex(edge.first);
        wrapper.eraseColumnIndex(edge.second);
        wrapper.setMatrix(std::move(matrix));

        /// We need to replace element [columnLogicalIndex, rowLogicalIndex] with infinity
        /// so we need to find their physical indices

        auto rowIndex = wrapper.getPhysicalRowIndex(columnLogicalIndex);
        auto colIndex = wrapper.getPhysicalColumnIndex(rowLogicalIndex);

        if(rowIndex && colIndex) {
            wrapper.getMatrix().at(*rowIndex, *colIndex) = infinity<T>;
        }

        auto weight = reduceMatrix(wrapper.getMatrix());
        if(weight < infinity<double> - 1) {
            weight += node.getWeight();
        }
        auto includedEdges = node.getIncludedEdges();
        Path p = node.getPath();
        p.addEdge({rowLogicalIndex, columnLogicalIndex});
        p.removeCycles(wrapper);
      //  node.addEdge({rowLogicalIndex, columnLogicalIndex});
      //  node.getPath().removeCycles(wrapper);

        Node<T> result(includedEdges, std::move(wrapper), p, weight);
        result.getIncludedEdges().push_back({rowLogicalIndex, columnLogicalIndex});

        return result;
    };
}
}
