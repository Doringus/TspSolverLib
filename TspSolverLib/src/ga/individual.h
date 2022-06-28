#pragma once

#include "common.h"

namespace tspsolver { namespace ga {

    template <typename Gene, typename FitnessType>
    class Individual {
    public:
        Individual() = default;
        Individual(const Chromosome<Gene>& chromosome) : m_Chromosome(chromosome) { }

        void setFitness(const FitnessType& fitnessType) {
            m_Fitness = fitnessType;
        }

        FitnessType getFitness() const noexcept {
            return m_Fitness;
        }

        Chromosome<Gene>& getChromosome() noexcept {
            return m_Chromosome;
        }

        Chromosome<Gene> const& getChromosome() const noexcept {
            return m_Chromosome;
        }

    private:
        Chromosome<Gene> m_Chromosome;
        FitnessType m_Fitness;
    };

}
}