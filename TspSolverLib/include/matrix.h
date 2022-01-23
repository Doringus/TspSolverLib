#pragma once

#include <concepts>
#include <cstddef>
#include <iterator>
#include <cassert>
#include <vector>
#include <functional>
#include <type_traits>
#include <ostream>

namespace tspsolver {

    template <typename Type, typename Allocator = std::allocator<Type>,
              std::enable_if_t<std::is_arithmetic_v<Type>, bool> = true>
    class SquareMatrix {
    public:

        class RowConstIterator {
        public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type        = const Type;
            using difference_type   = std::ptrdiff_t;
            using pointer           = const Type*;
            using reference         = const Type&;

            explicit RowConstIterator(Type* begin) : m_Ptr(begin) {

            }

            constexpr reference operator*() const {
                return *m_Ptr;
            }

            constexpr pointer operator->() const {
                return m_Ptr;
            }

            constexpr RowConstIterator& operator++() {
                ++m_Ptr;
                return *this;
            }

            constexpr RowConstIterator operator++(int) {
                RowConstIterator tmp = *this;
                ++m_Ptr;
                return tmp;
            }

            constexpr RowConstIterator& operator--() {
                m_Ptr--;
                return *this;
            }

            constexpr RowConstIterator operator--(int) {
                RowConstIterator tmp = *this;
                --m_Ptr;
                return tmp;
            }

            constexpr RowConstIterator operator+=(difference_type offset) noexcept {
                m_Ptr += offset;
                return *this;
            }

            constexpr RowConstIterator operator+(difference_type offset) const noexcept {
                RowConstIterator tmp = *this;
                tmp += offset;
                return tmp;
            }

            constexpr RowConstIterator operator-=(difference_type offset) noexcept {
                m_Ptr -= offset;
                return *this;
            }

            constexpr RowConstIterator operator-(difference_type offset) const noexcept {
                RowConstIterator tmp = *this;
                tmp -= offset;
                return tmp;
            }

            friend constexpr bool operator==(const RowConstIterator& lhs, const RowConstIterator& rhs) {
                return lhs.m_Ptr == rhs.m_Ptr;
            }
            friend constexpr bool operator!=(const RowConstIterator& lhs, const RowConstIterator& rhs) {
                return lhs.m_Ptr != rhs.m_Ptr;
            }
            friend constexpr size_t operator-(const RowConstIterator& lhs, const RowConstIterator& rhs) {
                return lhs.m_Ptr - rhs.m_Ptr;
            }

            Type* m_Ptr;
        };

        class RowIterator : public RowConstIterator {
        public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type        = Type;
            using difference_type   = std::ptrdiff_t;
            using pointer           = Type*;
            using reference         = Type&;

            explicit RowIterator(pointer begin) : RowConstIterator(begin) {

            }

            constexpr reference operator*() const {
                return const_cast<reference>(RowConstIterator::operator*());
            }

            constexpr pointer operator->() const {
                return this->m_Ptr;
            }

            constexpr RowIterator& operator++() {
                RowConstIterator::operator++();
                return *this;
            }

            constexpr RowIterator operator++(int) {
                RowIterator tmp = *this;
                RowConstIterator::operator++();
                return tmp;
            }

            constexpr RowIterator& operator--() {
                RowConstIterator::operator--();
                return *this;
            }

            constexpr RowIterator operator--(int) {
                RowIterator tmp = *this;
                RowConstIterator::operator--();
                return tmp;
            }

            constexpr RowIterator operator+=(difference_type offset) noexcept {
                RowConstIterator::operator+=(offset);
                return *this;
            }

            constexpr RowIterator operator+(difference_type offset) const noexcept {
                RowIterator tmp = *this;
                tmp += offset;
                return tmp;
            }

            constexpr RowIterator operator-=(difference_type offset) noexcept {
                RowConstIterator::operator-=(offset);
                return *this;
            }

            constexpr RowIterator operator-(difference_type offset) const noexcept {
                RowIterator tmp = *this;
                tmp -= offset;
                return tmp;
            }

            friend constexpr bool operator==(const RowIterator& lhs, const RowIterator& rhs) {
                return lhs.m_Ptr == rhs.m_Ptr;
            }
            friend constexpr bool operator!=(const RowIterator& lhs, const RowIterator& rhs) {
                return lhs.m_Ptr != rhs.m_Ptr;
            }
            friend constexpr size_t operator-(const RowIterator& lhs, const RowIterator& rhs) {
                return lhs.m_Ptr - rhs.m_Ptr;
            }
        };


        class ColumnConstIterator {
        public:

            using iterator_category = std::random_access_iterator_tag;
            using value_type        = const Type;
            using difference_type   = std::ptrdiff_t;
            using pointer           = const Type*;
            using reference         = const Type&;

            explicit ColumnConstIterator(Type* begin, std::size_t step) : m_Ptr(begin), m_Step(step) {

            }

            constexpr reference operator*() const {
                return *m_Ptr;
            }

            constexpr pointer operator->() const {
                return m_Ptr;
            }

            constexpr ColumnConstIterator& operator++() {
                m_Ptr += m_Step;
                return *this;
            }

            constexpr ColumnConstIterator operator++(int) {
                ColumnConstIterator tmp = *this;
                m_Ptr += m_Step;
                return tmp;
            }

            constexpr ColumnConstIterator& operator--() {
                m_Ptr -= m_Step;
                return *this;
            }

            constexpr ColumnConstIterator operator--(int) {
                ColumnConstIterator tmp = *this;
                m_Ptr -= m_Step;
                return tmp;
            }

            constexpr ColumnConstIterator operator+=(difference_type offset) noexcept {
                m_Ptr += offset * m_Step;
                return *this;
            }

            constexpr ColumnConstIterator operator+(difference_type offset) const noexcept {
                ColumnConstIterator tmp = *this;
                tmp += offset;
                return tmp;
            }

            constexpr ColumnConstIterator operator-=(difference_type offset) noexcept {
                m_Ptr -= offset * m_Step;
                return *this;
            }

            constexpr ColumnConstIterator operator-(difference_type offset) const noexcept {
                ColumnConstIterator tmp = *this;
                tmp -= offset;
                return tmp;
            }

            friend constexpr bool operator==(const ColumnConstIterator& lhs, const ColumnConstIterator& rhs) {
                return lhs.m_Ptr == rhs.m_Ptr;
            }
            friend constexpr bool operator!=(const ColumnConstIterator& lhs, const ColumnConstIterator& rhs) {
                return lhs.m_Ptr != rhs.m_Ptr;
            }
            friend constexpr size_t operator-(const ColumnConstIterator& lhs, const ColumnConstIterator& rhs) {
                return lhs.m_Ptr - rhs.m_Ptr;
            }

            Type* m_Ptr;
            std::size_t m_Step;
        };

        class ColumnIterator : public ColumnConstIterator {
        public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type        = Type;
            using difference_type   = std::ptrdiff_t;
            using pointer           = Type*;
            using reference         = Type&;

            explicit ColumnIterator(pointer begin, std::size_t step) : ColumnConstIterator(begin, step) {

            }

            constexpr reference operator*() const {
                return const_cast<reference>(ColumnConstIterator::operator*());
            }

            constexpr pointer operator->() const {
                return this->m_Ptr;
            }

            constexpr ColumnIterator& operator++() {
                ColumnConstIterator::operator++();
                return *this;
            }

            constexpr ColumnIterator operator++(int) {
                ColumnIterator tmp = *this;
                ColumnConstIterator::operator++();
                return tmp;
            }

            constexpr ColumnIterator& operator--() {
                ColumnConstIterator::operator--();
                return *this;
            }

            constexpr ColumnIterator operator--(int) {
                ColumnIterator tmp = *this;
                ColumnConstIterator::operator--();
                return tmp;
            }

            constexpr ColumnIterator operator+=(difference_type offset) noexcept {
                ColumnConstIterator::operator+=(offset);
                return *this;
            }

            constexpr ColumnIterator operator+(difference_type offset) const noexcept {
                ColumnIterator tmp = *this;
                tmp += offset;
                return tmp;
            }

            constexpr ColumnIterator operator-=(difference_type offset) noexcept {
                ColumnConstIterator::operator-=(offset);
                return *this;
            }

            constexpr ColumnIterator operator-(difference_type offset) const noexcept {
                ColumnIterator tmp = *this;
                tmp -= offset;
                return tmp;
            }

            friend constexpr bool operator==(const ColumnIterator& lhs, const ColumnIterator& rhs) {
                return lhs.m_Ptr == rhs.m_Ptr;
            }
            friend constexpr bool operator!=(const ColumnIterator& lhs, const ColumnIterator& rhs) {
                return lhs.m_Ptr != rhs.m_Ptr;
            }
            friend constexpr size_t operator-(const ColumnIterator& lhs, const ColumnIterator& rhs) {
                return lhs.m_Ptr - rhs.m_Ptr;
            }
        };

    public:

        using ElementsIterator = typename std::vector<Type>::iterator;
        using ElementsCIterator = typename std::vector<Type>::const_iterator;


        explicit constexpr SquareMatrix(std::size_t size) : m_Size(size) {
            m_Data.resize(size * size);
        }


        virtual constexpr Type& at(std::size_t rowIndex, std::size_t columnIndex) {
            return m_Data[rowIndex * m_Size + columnIndex];
        }

        virtual constexpr Type const& at(std::size_t rowIndex, std::size_t columnIndex) const {
            return m_Data.at(rowIndex * m_Size + columnIndex);
        }

        constexpr std::size_t size() const noexcept {
            return m_Size;
        }

        constexpr ElementsIterator begin() noexcept {
            return m_Data.begin();
        }

        constexpr ElementsCIterator begin() const noexcept {
            return m_Data.begin();
        }

        constexpr ElementsIterator end() noexcept {
            return m_Data.end();
        }

        constexpr ElementsCIterator end() const noexcept {
            return m_Data.end();
        }

        constexpr RowConstIterator rowBegin(std::size_t rowIndex) const noexcept {
            return RowConstIterator(const_cast<Type*>(m_Data.data()) + rowIndex * m_Size);
        }

        constexpr RowConstIterator rowEnd(std::size_t rowIndex) const noexcept {
            return RowConstIterator(const_cast<Type*>(m_Data.data()) + rowIndex * m_Size + m_Size);
        }

        constexpr RowIterator rowBegin(std::size_t rowIndex) noexcept {
            return RowIterator(m_Data.data() + rowIndex * m_Size);
        }

        constexpr RowIterator rowEnd(std::size_t rowIndex) noexcept {
            return RowIterator(m_Data.data() + rowIndex * m_Size + m_Size);
        }

        constexpr ColumnConstIterator columnBegin(std::size_t columnIndex) const noexcept {
            return ColumnConstIterator(const_cast<Type*>(m_Data.data()) + columnIndex, m_Size);
        }

        constexpr ColumnConstIterator columnEnd(std::size_t columnIndex) const noexcept {
            return ColumnConstIterator(const_cast<Type*>(m_Data.data()) + columnIndex + m_Size * m_Size, m_Size);
        }

        constexpr ColumnIterator columnBegin(std::size_t columnIndex) noexcept {
            return ColumnIterator(m_Data.data() + columnIndex, m_Size);
        }

        constexpr ColumnIterator columnEnd(std::size_t columnIndex) noexcept {
            return ColumnIterator(m_Data.data() + columnIndex + m_Size * m_Size, m_Size);
        }


    protected:
        std::vector<Type, Allocator> m_Data;
        std::size_t m_Size;
    };
}

