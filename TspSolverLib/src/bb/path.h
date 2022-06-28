#pragma once

#include <vector>
#include <list>

#include "common.h"

namespace tspsolver { namespace bb {

    using Edge = std::pair<size_t, size_t>;

    class Path {
    public:

        Path() = default;
        Path(const Path& other) = default;

        void addEdge(Edge edge) {

            auto prependIt = std::find_if(m_Chains.begin(), m_Chains.end(), [&edge](const std::list<Edge>& chain){
                return chain.front().first == edge.second;
            });
            auto appendIt = std::find_if(m_Chains.begin(), m_Chains.end(), [&edge](const std::list<Edge>& chain){
                return chain.back().second == edge.first;
            });

            if(m_Chains.size() == 0) {
                std::list<Edge> chain;
                chain.push_back(std::move(edge));
                m_Chains.push_back(chain);
                return;
            }

            /// 3 ways



            /// 1 - merge chains
            if(prependIt != m_Chains.end() && appendIt != m_Chains.end()) {
                if(prependIt == appendIt) {
                    (*appendIt).push_back(std::move(edge));
                    return;
                }
                (*appendIt).push_back(std::move(edge));
                (*appendIt).splice((*appendIt).end(), (*prependIt));
                //(*appendIt).merge((*prependIt));
                m_Chains.erase(prependIt);
            } else if(prependIt != m_Chains.end()) {
                /// 2 - place edge in the beginning of chain
                (*prependIt).push_front(std::move(edge));
            } else if (appendIt != m_Chains.end()){
                /// 3 - place edge in the end of chain
                (*appendIt).push_back(std::move(edge));
            } else {
                std::list<Edge> chain;
                chain.push_back(std::move(edge));
                m_Chains.push_back(chain);
            }
        }

        std::vector<std::list<Edge>> const& getChains() const noexcept {
            return m_Chains;
        }

        template <typename T>
        void removeCycles(MatrixIndicesWrapper<T>& wrapper) const {
                for(const auto& chain : m_Chains) {
                if(chain.size() == 1) {
                    continue;
                }
                auto rowIndex = wrapper.getPhysicalRowIndex(chain.back().second);
                auto colIndex = wrapper.getPhysicalColumnIndex(chain.front().first);

                if(rowIndex && colIndex) {
                    wrapper.getMatrix().at(*rowIndex, *colIndex) = infinity<T>;
                }
            }
        }

    private:
        std::vector<std::list<Edge>> m_Chains;
    };

}}

