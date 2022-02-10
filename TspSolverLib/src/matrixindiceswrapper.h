#pragma once

#include "matrix.h"

namespace tspsolver { namespace bb {

        template <typename T>
        class MatrixIndicesWrapper {
        public:
            MatrixIndicesWrapper() = default;
            MatrixIndicesWrapper(const MatrixIndicesWrapper& other) = default;
            MatrixIndicesWrapper(MatrixIndicesWrapper&& other) = default;
            MatrixIndicesWrapper& operator=(MatrixIndicesWrapper&& other) = default;
            MatrixIndicesWrapper& operator=(const MatrixIndicesWrapper& other) = default;

            MatrixIndicesWrapper(const SquareMatrix<T> matrix) : m_Matrix(matrix) {
                m_RowLogicalIndices.resize(matrix.size());
                m_ColumnLogicalIndices.resize(matrix.size());
                std::iota(m_RowLogicalIndices.begin(), m_RowLogicalIndices.end(), 0);
                m_ColumnLogicalIndices = m_RowLogicalIndices;
            }

            MatrixIndicesWrapper(const std::vector<std::size_t>& rowIndices, const std::vector<std::size_t>& colIndices) :
                m_RowLogicalIndices(rowIndices), m_ColumnLogicalIndices(colIndices) { }

            void eraseRowIndex(size_t index) {
                m_RowLogicalIndices.erase(m_RowLogicalIndices.begin() + index);
            }

            void eraseColumnIndex(size_t index) {
                m_ColumnLogicalIndices.erase(m_ColumnLogicalIndices.begin() + index);
            }

            std::optional<T> tryGetByLogicalIndex(size_t rowIndex, size_t colIndex) {
                auto rowIt = std::find(m_RowLogicalIndices.begin(),
                                       m_RowLogicalIndices.end(), rowIndex);
                auto columnIt = std::find(m_ColumnLogicalIndices.begin(),
                                          m_ColumnLogicalIndices.end(), colIndex);
                if(rowIt != m_RowLogicalIndices.end() && columnIt != m_ColumnLogicalIndices.end()) {
                    return  m_Matrix.at(std::distance(m_RowLogicalIndices.begin(), rowIt),
                                        std::distance(m_ColumnLogicalIndices.begin(), columnIt));
                }
                return std::nullopt;
            }

            std::optional<size_t> getPhysicalRowIndex(size_t logicalRowIndex) {
                return getPhysicalIndex(m_RowLogicalIndices, logicalRowIndex);
            }

            std::optional<size_t> getPhysicalColumnIndex(size_t logicalColumnIndex) {
                return getPhysicalIndex(m_ColumnLogicalIndices, logicalColumnIndex);
            }

            std::vector<std::size_t>& getRowIndices() noexcept {
                return m_RowLogicalIndices;
            }

            std::vector<std::size_t> const & getRowIndices() const noexcept {
                return m_RowLogicalIndices;
            }

            std::vector<std::size_t>& getColumnIndices() noexcept {
                return m_ColumnLogicalIndices;
            }

            std::vector<std::size_t> const & getColumnIndices() const noexcept {
                return m_ColumnLogicalIndices;
            }

            SquareMatrix<T>& getMatrix() {
                return m_Matrix;
            }

            SquareMatrix<T> const & getMatrix() const {
                return m_Matrix;
            }

            size_t getRowLogicalIndex(size_t index) {
                return m_RowLogicalIndices[index];
            }

            size_t getColumnLogicalIndex(std::size_t index) {
                return m_ColumnLogicalIndices[index];
            }

            void setMatrix(const SquareMatrix<T>& matrix) {
                m_Matrix = matrix;
            }

            void setMatrix(SquareMatrix<T>&& matrix) {
                m_Matrix = std::move(matrix);
            }

        private:

            std::optional<size_t> getPhysicalIndex(const std::vector<size_t>& indices, size_t index) {
                auto it = std::find(indices.begin(),
                                    indices.end(), index);
                if(it != indices.end()) {
                    return std::distance(indices.begin(), it);
                }
                return std::nullopt;
            }

        private:
            SquareMatrix<T> m_Matrix;
            std::vector<std::size_t> m_RowLogicalIndices; // logical row header
            std::vector<std::size_t> m_ColumnLogicalIndices; // logical column header
        };

}
}