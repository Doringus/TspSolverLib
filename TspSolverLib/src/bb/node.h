#pragma once

#include "matrixindiceswrapper.h"
#include "path.h"

namespace tspsolver { namespace bb {


    template <typename T>
    class Node {
    public:
        Node() = default;
        Node(const Node& other) = default;
        Node(Node&& other) = default;
        Node& operator=(const Node& other) = default;
        Node& operator=(Node&& other) = default;

        Node(std::vector<Edge> includedEdges, MatrixIndicesWrapper<T> wrapper, Path path, T weight)
        : m_IncludedEdges(std::move(includedEdges)), m_MatrixWrapper(std::move(wrapper)),
        m_Path(std::move(path)), m_Weight(weight) { }


        void setMatrixWrapper(const MatrixIndicesWrapper<T>& wrapper) {
            m_MatrixWrapper = wrapper;
        }

        void setMatrixWrapper(MatrixIndicesWrapper<T>&& wrapper) {
            m_MatrixWrapper = std::move(wrapper);
        }

        void setWeight(T weight) noexcept {
            m_Weight = weight;
        }

        double getWeight() const noexcept {
            return m_Weight;
        }

        std::vector<Edge>& getIncludedEdges() noexcept {
            return m_IncludedEdges;
        }

        std::vector<Edge> const& getIncludedEdges() const noexcept {
            return m_IncludedEdges;
        }

        SquareMatrix<T>& getMatrix() noexcept {
            return m_MatrixWrapper.getMatrix();
        };

        SquareMatrix<T> const& getMatrix() const noexcept {
            return m_MatrixWrapper.getMatrix();
        };

        MatrixIndicesWrapper<T>& getMatrixWrapper() noexcept {
            return m_MatrixWrapper;
        }

        MatrixIndicesWrapper<T> const& getMatrixWrapper() const noexcept {
            return m_MatrixWrapper;
        }

        void addEdge(const Edge& edge) {
            m_Path.addEdge(edge);
        }

        Path const& getPath() const {
            return m_Path;
        }

    private:
        double m_Weight;
        MatrixIndicesWrapper<T> m_MatrixWrapper;
        std::vector<Edge> m_IncludedEdges;
        Path m_Path;
    };

    template <typename T>
    struct nodeComparator_t {
        bool operator()(const Node<T>& lhs, const Node<T>& rhs){
            if(lhs.getWeight() == rhs.getWeight()) {
                return lhs.getMatrix().size() > rhs.getMatrix().size();
            }
            return lhs.getWeight() > rhs.getWeight();
        }
    };

}
}

