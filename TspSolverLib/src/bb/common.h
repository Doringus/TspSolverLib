#pragma once

#include <limits>

namespace tspsolver {

    template <typename T>
    constexpr T infinity = std::numeric_limits<T>::max();
    using Edge = std::pair<size_t, size_t>;
}