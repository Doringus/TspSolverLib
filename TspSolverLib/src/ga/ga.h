#pragma once

#include <random>
#include <functional>

template <typename T, typename Fitness>
class GeneticAlgorithmScheme {
public:
    GeneticAlgorithmScheme() {

    }

    void setFitnessFunction(std::function<Fitness(const T&)> fitness) {

    }

    void setSelectionFunction(std::function<std::vector<T>(std::vector<T>&)>) {

    }

    void setCrossoverFunction(std::function<std::pair<T, T>(const T&, const T&)> crossover) {

    }

    void setMutationFunction(std::function<void(T&)>) {

    }

    void iterate() {

    }

private:

};