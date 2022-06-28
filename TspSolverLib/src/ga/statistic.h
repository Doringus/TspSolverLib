#pragma once

#include "common.h"

namespace tspsolver { namespace ga {

    template <typename Gene, typename Fitness>
    class ILogger {
    public:
        virtual void logIteration(const Population<Gene, Fitness>& population) = 0;
    };

}}