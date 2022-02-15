#pragma once

#include <vector>
#include <algorithm>

#include "common.h"

namespace tspsolver { namespace ga {

    template <typename T>
    std::pair<Genome<T>, Genome<T>> posCrossover(const Genome<T>& lhs, const Genome<T>& rhs, const std::vector<size_t>& positions) {
        Genome<T> leftChild(lhs.size()), rightChild(rhs.size());

        /// Swap genes
        for(size_t index : positions) {
            leftChild[index] = rhs.at(index);
        }
        for(size_t index : positions) {
            rightChild[index] = lhs.at(index);
        }

        /// Copy rest genes
        auto fillChild = [](const Genome<T>& parent, Genome<T>& child, const std::vector<size_t> pos) {
            std::vector<size_t> indicesToFill;
            for(size_t i = 0; i < parent.size(); ++i) {
                if(std::find(pos.begin(), pos.end(), i) == pos.end()) {
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

    template <typename T>
    std::pair<Genome<T>, Genome<T>> ox2Crossover(const Genome<T>& lhs, const Genome<T>& rhs, const std::vector<size_t>& positions) {
        Genome<T> lhsSlice, rhsSlice;

        for(const auto& pos : positions) {
            lhsSlice.push_back(lhs.at(pos));
        }
        for(const auto& pos : positions) {
            rhsSlice.push_back(rhs.at(pos));
        }

        auto fillChild = [](const Genome<T>& parent, const Genome<T> slice) -> Genome<T> {
            size_t sliceIndex = 0;
            Genome<T> child(parent.size());

            for(size_t i = 0; i < parent.size(); ++i) {
                if(std::find(slice.begin(), slice.end(), parent.at(i)) != slice.end()) {
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

    template <typename T>
    std::pair<Genome<T>, Genome<T>> pmxCrossover(const Genome<T>& lhs, const Genome<T>& rhs, size_t begin, size_t end) {
        if(begin > end) {
            std::swap(begin, end);
        }

        Genome<T> leftChild(lhs.begin(), lhs.end()), rightChild(rhs.begin(), rhs.end());

        std::copy(lhs.begin() + begin, lhs.begin() + end + 1, rightChild.begin() + begin);
        std::copy(rhs.begin() + begin, rhs.begin() + end + 1, leftChild.begin() + begin);

        std::unordered_map<T, T> leftMapping, rightMapping;
        for(size_t i = begin; i <= end; ++i) {
            leftMapping[rhs[i]] = lhs[i];
            rightMapping[lhs[i]] = rhs[i];
        }

        auto fillChild = [](Genome<T>& child, std::unordered_map<T, T> mapping, size_t begin, size_t end) {
            for(size_t i = 0; i < begin; ++i) {
                while(mapping.find(child.at(i)) != mapping.end()) {
                    child[i] = mapping[child[i]];
                }
            }
            for(size_t i = end + 1; i < child.size(); ++i) {
                while(mapping.find(child.at(i)) != mapping.end()) {
                    child[i] = mapping[child[i]];
                }
            }
        };

        fillChild(leftChild, leftMapping, begin, end);
        fillChild(rightChild, rightMapping, begin, end);

        return {leftChild, rightChild};
    }

}}

