#pragma once

#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <utility>

template <typename T>
class Array {
private:
    std::size_t size;
    T* data;

public:
    explicit Array(std::size_t size = 0)
        : size(size), data(size ? new T[size]{} : nullptr) {}

    Array(std::initializer_list<T> values) : Array(values.size()) {
        std::size_t i = 0;
        for (const T& value : values) data[i++] = value;
    }

    Array(const Array& other) : Array(other.size) {
        for (std::size_t i = 0; i < size; ++i) data[i] = other.data[i];
    }

    Array(Array&& other) noexcept
        : size(std::exchange(other.size, 0)), data(std::exchange(other.data, nullptr)) {}

    Array& operator=(Array other) noexcept {
        std::swap(size, other.size);
        std::swap(data, other.data);
        return *this;
    }

    ~Array() {
        delete[] data;
    }

    std::size_t countAfterLastMax() const {
        if (size == 0) return 0;
        std::size_t lastMax = 0;
        for (std::size_t i = 1; i < size; ++i) {
            if (data[i] >= data[lastMax]) lastMax = i;
        }
        return size - lastMax - 1;
    }

    friend std::istream& operator>>(std::istream& input, Array& array) {
        for (std::size_t i = 0; i < array.size; ++i) {
            if (!(input >> array.data[i])) break;
        }
        return input;
    }

    friend std::ostream& operator<<(std::ostream& output, const Array& array) {
        for (std::size_t i = 0; i < array.size; ++i) output << array.data[i] << '\n';
        return output;
    }
};
