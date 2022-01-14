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



namespace tspsolver {

    class TSPSOLVERLIB_EXPORT TspSolver {

    };

}

#include <string>

class TSPSOLVERLIB_EXPORT TspSolverLib
{
public:
    TspSolverLib();
    std::string print() {
        return "Hello fron dll\n";
    }
};
