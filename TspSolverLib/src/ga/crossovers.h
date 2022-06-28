#pragma once

#include <vector>
#include <algorithm>
#include <random>
#include <set>
#include <cmath>

#include "common.h"

namespace tspsolver { namespace ga {

    template <typename T>
    struct ICrossover {
        virtual ~ICrossover() = default;
        virtual std::vector<Chromosome<T>> mate(const Chromosome<T>& lhs, const Chromosome<T>& rhs) = 0;
    };

    template <typename T>
    class PositionCrossover : public ICrossover<T> {
    public:
        PositionCrossover(const std::shared_ptr<std::mt19937_64>& randomEngine) : m_RandomEngine(randomEngine) { }

        std::vector<Chromosome<T>> mate(const Chromosome<T>& lhs, const Chromosome<T>& rhs) override {
            std::uniform_int_distribution<size_t> positionsDist(0, lhs.size() - 1);
            std::uniform_int_distribution<size_t> countDist(1, lhs.size() - 1);

            std::set<size_t> positions;
            for(size_t i = 0; i < countDist(*m_RandomEngine); ++i) {
                positions.insert(positionsDist(*m_RandomEngine));
            }

            Chromosome<T> leftChild(lhs.size()), rightChild(rhs.size());

            /// Swap genes
            for(size_t index : positions) {
                leftChild[index] = rhs.at(index);
            }
            for(size_t index : positions) {
                rightChild[index] = lhs.at(index);
            }

            /// Copy rest genes
            auto fillChild = [](const Chromosome<T>& parent, Chromosome<T>& child, const std::set<size_t>& pos) {
                std::vector<size_t> indicesToFill;
                for(size_t i = 0; i < parent.size(); ++i) {
                    if(pos.find(i) == pos.end()) {
                        indicesToFill.push_back(i);
                    }
                }
                size_t parentGeneIndex = 0;
                for(size_t index : indicesToFill) {
                    for(; parentGeneIndex < parent.size(); ++parentGeneIndex){
                        if(std::find(child.begin(), child.end(), parent.at(parentGeneIndex)) == child.end()) {
                            child[index] = parent.at(parentGeneIndex);
                            parentGeneIndex++;
                            break;
                        }
                    }
                }
            };

            fillChild(lhs, leftChild, positions);
            fillChild(rhs, rightChild, positions);

            return {leftChild, rightChild};
        }

    private:
        std::shared_ptr<std::mt19937_64> m_RandomEngine;
    };

    template <typename T>
    class Ox2Crossover : public ICrossover<T> {
    public:
        Ox2Crossover(const std::shared_ptr<std::mt19937_64>& randomEngine) : m_RandomEngine(randomEngine) { }

        std::vector<Chromosome<T>> mate(const Chromosome<T>& lhs, const Chromosome<T>& rhs) override {
            Chromosome<T> lhsSlice, rhsSlice;

            std::uniform_int_distribution<size_t> positionsDist(0, lhs.size() - 1);
            std::uniform_int_distribution<size_t> countDist(1, lhs.size() - 1);

            std::set<size_t> positions;
            for(size_t i = 0; i < countDist(*m_RandomEngine); ++i) {
                positions.insert(positionsDist(*m_RandomEngine));
            }

            for (const auto &pos: positions) {
                lhsSlice.push_back(lhs.at(pos));
            }
            for (const auto &pos: positions) {
                rhsSlice.push_back(rhs.at(pos));
            }

            auto fillChild = [](const Chromosome<T>& parent, const Chromosome<T>& slice) -> Chromosome<T> {
                size_t sliceIndex = 0;
                Chromosome<T> child(parent.size());
                for (size_t i = 0; i < parent.size(); ++i) {
                    if (std::find(slice.begin(), slice.end(), parent.at(i)) != slice.end()) {
                        child[i] = slice.at(sliceIndex);
                        sliceIndex++;
                    } else {
                        child[i] = parent[i];
                    }
                }
                return child;
            };

            return {fillChild(lhs, rhsSlice), fillChild(rhs, lhsSlice)};
        }

    private:
        std::shared_ptr<std::mt19937_64> m_RandomEngine;
    };

    template <typename T>
    class PmxCrossover : public ICrossover<T> {
    public:
        PmxCrossover(const std::shared_ptr<std::mt19937_64>& randomEngine) : m_RandomEngine(randomEngine) { }


        std::vector<Chromosome<T>> mate(const Chromosome<T>& lhs, const Chromosome<T>& rhs) override {
            Chromosome<T> leftChild(lhs.begin(), lhs.end()), rightChild(rhs.begin(), rhs.end());

            std::uniform_int_distribution<size_t> positionsDist(0, lhs.size() - 2);
            size_t begin = positionsDist(*m_RandomEngine);
            std::uniform_int_distribution<size_t> endDist(begin + 1, lhs.size() - 1);
            size_t end = endDist(*m_RandomEngine);;

            std::copy(lhs.begin() + begin, lhs.begin() + end + 1, rightChild.begin() + begin);
            std::copy(rhs.begin() + begin, rhs.begin() + end + 1, leftChild.begin() + begin);

            std::unordered_map<T, T> leftMapping, rightMapping;
            for(size_t i = begin; i <= end; ++i) {
                leftMapping[rhs[i]] = lhs[i];
                rightMapping[lhs[i]] = rhs[i];
            }

            auto fillChild = [](Chromosome<T>& child, const std::unordered_map<T, T>& mapping, size_t begin, size_t end) {
                for(size_t i = 0; i < begin; ++i) {
                    while(mapping.find(child.at(i)) != mapping.end()) {
                        child[i] = mapping.at(child[i]);
                    }
                }
                for(size_t i = end + 1; i < child.size(); ++i) {
                    while(mapping.find(child.at(i)) != mapping.end()) {
                        child[i] = mapping.at(child[i]);
                    }
                }
            };

            fillChild(leftChild, leftMapping, begin, end);
            fillChild(rightChild, rightMapping, begin, end);

            return {leftChild, rightChild };
        }

    private:
        std::shared_ptr<std::mt19937_64> m_RandomEngine;
    };

}}

