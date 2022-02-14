#pragma once

#include <vector>
#include <algorithm>

namespace tspsolver { namespace ga {

    template <typename T>
    using Genome = std::vector<T>;

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
}}

