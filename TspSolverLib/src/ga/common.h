#pragma once

#include <vector>

namespace tspsolver { namespace ga {

    template <typename Gene>
    using Chromosome = std::vector<Gene>;

    template <typename Gene, typename FitnessType>
    class Individual;

    template <typename Gene, typename Fitness>
    using Population = std::vector<Individual<Gene, Fitness>>;


}}