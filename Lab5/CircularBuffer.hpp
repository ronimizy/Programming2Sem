//
// Created by Георгий Круглов on 25.02.2021.
//

#ifndef LAB5_CIRCULARBUFFER_HPP
#define LAB5_CIRCULARBUFFER_HPP

#include <vector>

template<typename T>
class CircularBuffer {
    T **memory;
    size_t size_;

    size_t front_ = 0;
    size_t back_ = 0;


    void put_(T *value) {
        if (size_) {
            memory[back_] = value;
            back_ = (back_ + 1) % size_;
        }
    }

    T *get_(const size_t &index) {
        if (size_) {
            return memory[(front_ + index) % size_];
        } else {
            return nullptr;
        }
    }

    T *pop_() {
        if (size_) {
            T *value = memory[front_];
            front_ = (front_ + 1) % size_;

            return value;
        } else {
            return nullptr;
        }
    }

public:
    /** Life cycle **/
    explicit CircularBuffer(size_t size) : size_(size) {
        memory = new T *[size];
    }

    //Copy constructor
    CircularBuffer(const CircularBuffer &origin) : size_(origin.size_) {
        memory = new T *[origin.size_];

        for (size_t i = 0; i < size_; ++i) {
            *memory[i] = *origin.memory[i];
        }
    }

    //Move constructor
    CircularBuffer(CircularBuffer &&origin) noexcept: size_(origin.size_) {
        memory = origin.memory;
        origin.memory = nullptr;
    }

    CircularBuffer &operator=(const CircularBuffer &rhs) {
        if (this != &rhs) {
            size_ = rhs.size_;

            delete[] memory;
            memory = new T *[size_];

            for (int i = 0; i < size_; ++i) {
                memory[i] = rhs.memory[i];
            }
        }

        return *this;
    }

    ~CircularBuffer() {
        delete[] memory;
    }

    /** Iterator **/
    struct Iterator {
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;

        Iterator(CircularBuffer &buffer, const size_t &index) : buffer_(buffer), index_(index) {
            if (buffer.size_) {
                index_ %= buffer.size_;
            }
        };

        pointer operator->() { return buffer_.get_(index_); }


        Iterator &operator++() {
            if (buffer_.size_) {
                index_ = (index_ + 1) % buffer_.size_;
            }

            return *this;
        }

        Iterator operator++(Iterator) {
            Iterator temp = *this;

            ++*this;

            return temp;
        }

        Iterator &operator--() {
            if (buffer_.size_) {
                index_ = (index_ - 1 + buffer_.size_) % buffer_.size_;
            }

            return *this;
        }

        Iterator operator--(Iterator) {
            Iterator temp = *this;
            --*this;
            return temp;
        }

        bool operator!=(Iterator &rhs) {
            return index_ != rhs.index_;
        }

    private:
        CircularBuffer &buffer_;
        size_t index_;
    };

    /** Methods **/
    void append(T value) { put_(new T(value)); }

    void append(T *value) { put_(value); }

    T *pop() { return pop_(); }

    T *front() { return memory[front_]; }

    T *back() { return memory[back_]; }

    Iterator begin() { return Iterator(*this, front_); }

    Iterator end() { return Iterator(*this, back_ + 1); }

    void resize(size_t newSize) {
        if (size_ == newSize) { return; }

        T **n = new T *[newSize];

        for (size_t i = 0; i < ((size_ < newSize) ? size_ : newSize); ++i) {
            n[i] = new T(*memory[i]);
        }

        delete[] memory;

        memory = n;
        front_ = 0;
        back_ = size_ < newSize ? size_ - 1 : newSize - 1;
        size_ = newSize;
    }

    /** Operators **/
    T *operator[](const size_t &index) { return get_(index); }

    T *operator[](const size_t &&index) { return get_(index); }
};

#endif //LAB5_CIRCULARBUFFER_HPP
