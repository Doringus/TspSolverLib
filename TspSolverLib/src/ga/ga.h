#pragma once

#include <random>
#include <functional>
#include <ctime>
#include <memory>
#include <exception>

#include "tspsolverlib.h"
#include "common.h"
#include "crossovers.h"
#include "mutations.h"
#include "selectors.h"
#include "individual.h"
#include "statistic.h"

namespace tspsolver { namespace ga {


    template <typename Gene, typename FitnessType>
    class GeneticAlgorithm {
    public:

        using FitnessFunction = std::function<FitnessType(const std::vector<Gene>& individual)>;

        GeneticAlgorithm(const std::vector<std::vector<Gene>>& initialPopulation, double mutationProbability,
                         double crossoverProbability, size_t generations,
                         const std::shared_ptr<ILogger<Gene, FitnessType>>& logger = nullptr) :
                         m_MutationProbability(mutationProbability), m_CrossoverProbability(crossoverProbability),
                         m_Generations(generations), m_Logger(logger) {

            std::random_device rd;
            m_RandomEngine = std::make_unique<std::mt19937_64>(rd());

            for(auto& individual : initialPopulation) {
                m_Population.push_back(Individual<Gene, FitnessType>(individual));
            }
        }

        void setMutationProbability(double probability) noexcept {
            m_MutationProbability = probability;
        }

        void setCrossoverProbability(double probability) noexcept {
            m_CrossoverProbability = probability;
        }

        void setFitness(const FitnessFunction& fitnessFunction) {
            m_Fitness = fitnessFunction;
        }

        void setSelection(const std::shared_ptr<ISelection<Gene, FitnessType>>& selection) {
            m_Selection = selection;
        }

        void setCrossover(const std::shared_ptr<ICrossover<Gene>>& crossover) {
            m_Crossover = crossover;
        }

        void setMutation(const std::shared_ptr<IMutation<Gene>>& mutation) {
            m_Mutation = mutation;
        }

        Individual<Gene, FitnessType> evolve() {
            if(!m_Fitness) {
                throw std::runtime_error("error");
            }

            for(auto& individual : m_Population) {
                individual.setFitness(m_Fitness(individual.getChromosome()));
            }
            if(m_Logger) {
                m_Logger->logIteration(m_Population);
            }
           // normilizeFitness(m_Population);
            std::uniform_real_distribution<double> zeroOneDist(0.0, 1.0);

            for(size_t i = 0; i < m_Generations; ++i) {

                auto matingPool = m_Selection->select(m_Population, m_Population.size());

                std::vector<Individual<Gene, FitnessType>> nextGeneration;
                nextGeneration.reserve(m_Population.size());
                /// Simple mating strategy
                /// TODO: add customization for selecting parents. E.g: random, elitism etc
                std::uniform_int_distribution<size_t> parentsDist(0, matingPool.size() - 1);
                while(nextGeneration.size() != m_Population.size()) {
                    /// Mate
                    const auto& lhs = matingPool.at(parentsDist(*m_RandomEngine)).get().getChromosome();
                    const auto& rhs = matingPool.at(parentsDist(*m_RandomEngine)).get().getChromosome();

                    if(zeroOneDist(*m_RandomEngine) < m_CrossoverProbability) {
                        auto offspring = m_Crossover->mate(lhs, rhs);
                        for(auto& child : offspring) {
                            nextGeneration.emplace_back(child);
                        }
                    } else {
                        nextGeneration.emplace_back(lhs);
                        nextGeneration.emplace_back(rhs);
                    }
                }

                /// Mutate
                for(auto& individual : nextGeneration) {
                    if(zeroOneDist(*m_RandomEngine) < m_MutationProbability) {
                        m_Mutation->mutate(individual.getChromosome());
                    }
                }

                /// Update fitness
                for(auto& individual : nextGeneration) {
                    individual.setFitness(m_Fitness(individual.getChromosome()));
                }
                if(m_Logger) {
                    m_Logger->logIteration(m_Population);
                }
             //   normilizeFitness(nextGeneration);

                m_Population = std::move(nextGeneration);
            }

            auto it = std::max_element(m_Population.begin(), m_Population.end(), [](const auto& lhs, const auto& rhs){
                return lhs.getFitness() < rhs.getFitness();
            });

            return *it;
        }

    private:
        void normilizeFitness(Population<Gene, FitnessType>& population) {
            auto sum = std::accumulate(population.begin(), population.end(), 0.0f, [](const auto& lhs, const auto& rhs){
                return lhs + rhs.getFitness();
            });
            for(auto& ind : population) {
                ind.setFitness(ind.getFitness() / sum);
            }
        }
    private:
        double m_MutationProbability, m_CrossoverProbability;
        size_t m_Generations;
        Population<Gene, FitnessType> m_Population;
        std::unique_ptr<std::mt19937_64> m_RandomEngine;

        std::shared_ptr<ICrossover<Gene>> m_Crossover;
        std::shared_ptr<IMutation<Gene>> m_Mutation;
        std::shared_ptr<ISelection<Gene, FitnessType>> m_Selection;
        std::shared_ptr<ILogger<Gene, FitnessType>> m_Logger;
        FitnessFunction m_Fitness;
    };
}}