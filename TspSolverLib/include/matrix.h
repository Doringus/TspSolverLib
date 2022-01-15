#pragma once

#include <concepts>
#include <cstddef>
#include <iterator>
#include <cassert>
#include <vector>
#include <functional>

namespace tspsolver {

    template <typename T>
    concept Number = std::integral<T> || std::floating_point<T>;

    template <typename Type> requires Number<Type>
    class SquareMatrix {
    public:
        using RowIterator = typename std::vector<Type>::iterator;
        using RowCIterator = typename std::vector<Type>::const_iterator;
        using ColumnIterator = typename std::vector<std::reference_wrapper<Type>>::iterator;
        using ColumnCIterator = typename std::vector<std::reference_wrapper<Type>>::const_iterator;
        using RowsIterator = typename  std::vector<std::vector<Type>>::iterator;
        using RowsCIterator = typename  std::vector<std::vector<Type>>::const_iterator;
        using ColumnsIterator = typename std::vector<std::vector<std::reference_wrapper<Type>>>::iterator;
        using ColumnsCIterator = typename std::vector<std::vector<std::reference_wrapper<Type>>>::const_iterator;

        explicit SquareMatrix(std::size_t size) {
            m_Data.resize(size);
            m_Columns.resize(size);
            for(std::size_t i = 0; i < size; ++i) {
                m_Data[i].resize(size);
                for(std::size_t j = 0; j < size; ++j) {
                    m_Data[i][j] = 0;
                    m_Columns[j].push_back(m_Data[i][j]);
                }
            }
        }

        SquareMatrix(const SquareMatrix& matrix) : m_Data(matrix.m_Data) {
            std::size_t size = m_Data.size();
            m_Columns.resize(size);
            for(std::size_t i = 0; i < size; ++i) {
                for(std::size_t j = 0; j < size; ++j) {
                    m_Columns[j].push_back(m_Data[i][j]);
                }
            }
        }

        SquareMatrix(SquareMatrix&& matrix) noexcept: m_Data(std::exchange(matrix.m_Data, {})),
                        m_Columns(std::exchange(matrix.m_Columns, {})) {

        }

        constexpr std::size_t getSize() const noexcept {
            return m_Data.size();
        }

        constexpr Type& at(std::size_t rowIndex, std::size_t columnIndex) {
            return m_Data.at(rowIndex).at(columnIndex);
        }

        constexpr Type const& at(std::size_t rowIndex, std::size_t columnIndex) const {
            return m_Data[rowIndex][columnIndex];
        }

        constexpr RowIterator rowBegin(std::size_t rowIndex) {
            return m_Data[rowIndex].begin();
        }

        constexpr RowCIterator rowBegin(std::size_t rowIndex) const {
            return m_Data[rowIndex].cbegin();
        }

        constexpr RowIterator rowEnd(std::size_t rowIndex) {
            return m_Data[rowIndex].end();
        }

        constexpr RowCIterator rowEnd(std::size_t rowIndex) const {
            return m_Data[rowIndex].cend();
        }

        constexpr ColumnIterator columnBegin(std::size_t columnIndex) {
            return m_Columns[columnIndex].begin();
        }

        constexpr ColumnCIterator columnBegin(std::size_t columnIndex) const {
            return m_Columns[columnIndex].begin();
        }

        constexpr ColumnIterator columnEnd(std::size_t columnIndex) {
            return m_Columns[columnIndex].end();
        }

        constexpr ColumnCIterator columnEnd(std::size_t columnIndex) const {
            return m_Columns.at(columnIndex).cend();
        }

        constexpr RowsIterator rowsBegin(){
            return m_Data.begin();
        }

        constexpr RowsCIterator rowsBegin() const {
            return m_Data.cbegin();
        }

        constexpr ColumnsIterator columnsBegin() {
            return m_Columns.begin();
        }

        constexpr ColumnsCIterator columnsBegin() const {
            return m_Columns.cbegin();
        }

        std::vector<std::vector<Type>>& rows() {
            return m_Data;
        }

        std::vector<std::vector<Type>> const& rows() const {
            return m_Data;
        }

        std::vector<std::vector<std::reference_wrapper<Type>>>& columns() {
            return m_Columns;
        }

        std::vector<std::vector<std::reference_wrapper<Type>>> const& columns() const {
            return m_Columns;
        }

    private:
        std::vector<std::vector<Type>> m_Data;
        std::vector<std::vector<std::reference_wrapper<Type>>> m_Columns;
    };
}

