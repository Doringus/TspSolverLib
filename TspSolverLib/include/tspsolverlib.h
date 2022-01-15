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
#include <concepts>
#include <algorithm>
#include <numeric>
#include <limits>

#include "matrix.h"

namespace tspsolver {

    template <typename T>
    constexpr T infinity = std::numeric_limits<T>::max();

    struct optimalTour_t {
        double weight;
        std::vector<int> path;
    };

    struct TSPSOLVERLIB_EXPORT TspSolver {
        virtual optimalTour_t solve() = 0;
    };

    template <typename Type>
    constexpr std::vector<Type> getMinByRows(const SquareMatrix<Type>& matrix) {
        std::vector<Type> result;
        std::size_t matrixSize = matrix.getSize();
        result.reserve(matrixSize);
        for(std::size_t i = 0; i < matrixSize; ++i) {
            result.push_back(*std::min_element(matrix.rowBegin(i), matrix.rowEnd(i)));
        }
        return result;
    }

    template <typename Type>
    constexpr std::vector<Type> getMinByColumns(const SquareMatrix<Type>& matrix) {
        std::vector<Type> result;
        std::size_t matrixSize = matrix.getSize();
        result.reserve(matrixSize);
        for(std::size_t i = 0; i < matrixSize; ++i) {
            result.push_back(*std::min_element(matrix.columnBegin(i), matrix.columnEnd(i)));
        }
        return result;
    }

    template <typename Type, typename InputIt>
    constexpr Type getMinExcept(InputIt begin, InputIt end, InputIt except) {
        Type exceptValue = *except;
        *except = infinity<Type>;
        Type result = *std::min_element(begin, end);
        *except = exceptValue;
        return result;
    }


    template <typename Type>
    constexpr Type reduce2dCollection(auto& collection2d) {
        Type weight = static_cast<Type>(0);
        for(auto& plainCollection : collection2d) {
            Type min = *std::min_element(plainCollection.begin(), plainCollection.end());
            weight += min;
            std::for_each(plainCollection.begin(), plainCollection.end(), [min](auto& e) {
                if(e != infinity<Type>) {
                    e -= min;
                }
            });
        }
        return weight;
    }

    template <typename Type>
    constexpr Type reduceMatrix(SquareMatrix<Type>& matrix) {
        return reduce2dCollection<Type>(matrix.rows()) + reduce2dCollection<Type>(matrix.columns());
    }
/*

    template <typename Type> requires Number<Type>
    class TSPSOLVERLIB_EXPORT BBSolver : public TspSolver {
    public:
        BBSolver(<Type> graphMatrix);

        optimalTour_t solve() override;
    private:
        Matrix<Type> m_GraphMatrix;
    };
 */
}
