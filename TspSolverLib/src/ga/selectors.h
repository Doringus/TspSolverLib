#pragma once

#include "common.h"

#include <random>
#include <memory>

namespace tspsolver { namespace ga {

    template <typename T, typename F>
    struct ISelection {
        virtual ~ISelection() = default;
        virtual std::vector<std::reference_wrapper<const Individual<T, F>>> select(const Population<T, F>& population, size_t size) = 0;
    };

    template <typename T, typename F>
    class RouletteSelection : public ISelection<T, F> {
    public:
        RouletteSelection(const std::shared_ptr<std::mt19937_64>& randomEngine) : m_RandomEngine(randomEngine) {}

        std::vector<std::reference_wrapper<const Individual<T, F>>> select(const Population<T, F>& population, size_t size) override {
            auto min = std::min_element(population.begin(), population.end(), [](const auto& lhs, const auto& rhs){
                return lhs.getFitness() < rhs.getFitness();
            });
            auto sum = std::accumulate(population.begin(), population.end(), F(), [](auto lhs, const auto& rhs){
                return lhs + rhs.getFitness();
            });

            std::uniform_real_distribution<double> rouletteDist((*min).getFitness(), sum);
            std::vector<std::reference_wrapper<const Individual<T, F>>> result;

            while(result.size() < size) {
                double roulette = rouletteDist(*m_RandomEngine);
                double sum = 0;
                for(size_t i = 0; i < population.size(); ++i) {
                    sum += population.at(i).getFitness();
                    if(roulette < sum) {
                        result.push_back(population.at(i));
                    }
                }
            }
            return result;
        }
    private:
        std::shared_ptr<std::mt19937_64> m_RandomEngine;
    };

        template <typename T, typename F>
        class TournamentSelection : public ISelection<T, F> {
        public:
            TournamentSelection(const std::shared_ptr<std::mt19937_64>& randomEngine) : m_RandomEngine(randomEngine) {}

            std::vector<std::reference_wrapper<const Individual<T, F>>> select(const Population<T, F>& population, size_t size) override {
                std::uniform_int_distribution<size_t> dist(0, population.size() - 1);
                std::vector<std::reference_wrapper<const Individual<T, F>>> result;

                while(result.size() < size) {
                    std::vector<std::reference_wrapper<const Individual<T, F>>> tournament;
                    for(size_t i = 0; i < 5; ++i) {
                        tournament.push_back(population.at(dist(*m_RandomEngine)));
                    }
                    auto it = std::max_element(tournament.begin(), tournament.end(), [](const auto& lhs, const auto& rhs) {
                        return lhs.get().getFitness() < rhs.get().getFitness();
                    });
                    result.push_back(*it);
                }
                return result;
            }
        private:
            std::shared_ptr<std::mt19937_64> m_RandomEngine;
        };

}
}

