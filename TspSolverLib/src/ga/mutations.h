#pragma once

#include "common.h"

namespace tspsolver { namespace ga {


    template <typename T>
    struct IMutation {
        virtual ~IMutation() = default;
        virtual void mutate(Chromosome<T>& individual) = 0;
    };

    template <typename T>
    class SwapMutation : public IMutation<T> {
    public:
        SwapMutation(const std::shared_ptr<std::mt19937_64>& randomEngine) noexcept : m_RandomEngine(randomEngine) {}

        void mutate(Chromosome<T>& individual) override {
            std::uniform_int_distribution<size_t> swapPointDist(0, individual.size() - 1);
            std::swap(individual[swapPointDist(*m_RandomEngine)], individual[swapPointDist(*m_RandomEngine)]);
        }

    private:
        std::shared_ptr<std::mt19937_64> m_RandomEngine;
    };

    template <typename T>
    class InverseMutation : public IMutation<T> {
    public:
        InverseMutation(const std::shared_ptr<std::mt19937_64>& randomEngine) noexcept : m_RandomEngine(randomEngine) {}

        void mutate(Chromosome<T>& individual) override {
            std::uniform_int_distribution<size_t> inversePointDist(0, individual.size() - 1);
            size_t lhs = inversePointDist(*m_RandomEngine);
            size_t rhs = inversePointDist(*m_RandomEngine);
            if(lhs > rhs) {
                std::swap(lhs, rhs);
            }
            std::reverse(individual.begin() + lhs, individual.begin() + rhs);
        }
    private:
        std::shared_ptr<std::mt19937_64> m_RandomEngine;
    };

}}