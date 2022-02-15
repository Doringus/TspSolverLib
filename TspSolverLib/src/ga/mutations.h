#pragma once

#include "common.h"

namespace tspsolver { namespace ga {

    template <typename T>
    void swapMutation(Genome<T>& individual, size_t lhs, size_t rhs) {
        std::swap(individual[lhs], individual[rhs]);
    }

    template <typename T>
    void inverseMutation(Genome<T>& individual, size_t lhs, size_t rhs) {
        std::reverse(individual.begin() + lhs, individual.begin() + lhs + rhs);
    }

}}