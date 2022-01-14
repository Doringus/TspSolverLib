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

#include "matrix.h"

namespace tspsolver {

    struct optimalTour_t {
        double weight;
        std::vector<int> path;
    };

    struct TSPSOLVERLIB_EXPORT TspSolver {
        virtual optimalTour_t solve() = 0;
    };

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
