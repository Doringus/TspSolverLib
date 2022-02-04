#pragma once

#include <vector>
#include <concepts>
#include <algorithm>
#include <numeric>
#include <limits>

#include "matrix.h"

namespace tspsolver { namespace  bb {

    template <typename T>
    constexpr T infinity = std::numeric_limits<T>::max();

    using Edge = std::pair<size_t, size_t>;

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
    constexpr Type reducePlainArray(InputIt begin, InputIt end) {
        Type min = *std::min_element(begin, end);
        std::for_each(begin, end, [min](auto& e) {
            if(e != infinity<Type>) {
                e -= min;
            }
        });

        return min;
    }

    template <typename Type>
    constexpr Type reduceMatrix(SquareMatrix<Type>& matrix) {
        Type reduceFactor = 0;
        for(size_t i = 0; i < matrix.size(); ++i) {
            reduceFactor += reducePlainArray<Type>(matrix.rowBegin(i), matrix.rowEnd(i));
        }
        for(size_t i = 0; i < matrix.size(); ++i) {
            reduceFactor += reducePlainArray<Type>(matrix.columnBegin(i), matrix.columnEnd(i));
        }

        return reduceFactor;
    }

    template <typename Type>
    constexpr std::vector<Edge>
    findMaxNullPenalties(const SquareMatrix<Type>& matrix) {
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
    constexpr SquareMatrix<Type> includeEdge(SquareMatrix<Type>& matrix, const Edge& edge) {
        matrix.at(edge.second, edge.first) = infinity<Type>;
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

    template<typename T>
    struct node_t {
        SquareMatrix<T> matrix;
        std::vector<size_t> rowOriginalIndices;
        std::vector<size_t> columnOriginalIndices;
        T weight;
    };

    template<typename T>
    struct nodeComparator_t {
        bool operator()(const node_t<T>& lhs, const node_t<T>& rhs){
            return lhs.weight < rhs.weight;
        }
    };


}
}