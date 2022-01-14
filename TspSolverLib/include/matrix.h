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
        using ColumnIterator = typename std::vector<std::reference_wrapper<Type>>::iterator;

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

        SquareMatrix(SquareMatrix&& matrix) : m_Data(std::exchange(matrix.m_Data, {})),
                        m_Columns(std::exchange(matrix.m_Columns, {})) {

        }

        constexpr Type& at(std::size_t rowIndex, std::size_t columnIndex) {
            return m_Data.at(rowIndex).at(columnIndex);
        }

        constexpr Type const& at(std::size_t rowIndex, std::size_t columnIndex) const {
            return m_Data[rowIndex][columnIndex];
        }

        RowIterator rowBegin(std::size_t rowIndex) {
            return m_Data[rowIndex].begin();
        }

        RowIterator rowEnd(std::size_t rowIndex) {
            return m_Data[rowIndex].end();
        }

        ColumnIterator columnBegin(std::size_t columnIndex) {
            return m_Columns[columnIndex].begin();
        }

        ColumnIterator columnEnd(std::size_t columnIndex) {
            return m_Columns[columnIndex].end();
        }

    private:
        std::vector<std::vector<Type>> m_Data;
        std::vector<std::vector<std::reference_wrapper<Type>>> m_Columns;
    };
}

