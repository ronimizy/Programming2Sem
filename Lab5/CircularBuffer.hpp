//
// Created by Георгий Круглов on 25.02.2021.
//

#ifndef LAB5_CIRCULARBUFFER_HPP
#define LAB5_CIRCULARBUFFER_HPP

template<typename T, typename allocator_type = std::allocator<T> >
class CircularBuffer {
    allocator_type allocator_ = allocator_type();
    allocator_type get_allocator() { return allocator_; }

    T *memory;

    size_t capacity_;
    size_t size_ = 0;

    size_t front_ = 0;
    size_t back_ = 0;

    /** PrivateMethods.hpp **/
    void increment(size_t &i, long by = 1);

    size_t incremented(const size_t &i, long by = 1) const;

    void decrement(size_t &i, long by = 1);

    size_t decremented(const size_t &i, long by = 1) const;

    void put_(T &value, bool atBack, bool copy = true);

    T &get_(const size_t &index) const;

    T &pop_(bool atBack);

public:
    /** Life cycle **/
    CircularBuffer(size_t capacity = 0) : capacity_(capacity) {
        memory = allocator_.allocate(capacity);
    }

    //Copy constructor
    CircularBuffer(const CircularBuffer &origin) : capacity_(origin.capacity_) {
        memory = allocator_.allocate(origin.capacity_);

        for (auto &it : origin) {
            push_back(it);
        }
    }

    //Copy insert constructor
    CircularBuffer(const CircularBuffer &origin, allocator_type &alloc) : CircularBuffer(origin) {
        allocator_ = alloc;
    }

    //Move constructor
    CircularBuffer(CircularBuffer &&origin) noexcept: capacity_(origin.capacity_) {
        memory = origin.memory;
        origin.memory = nullptr;
    }

    //Move insert constructor
    CircularBuffer(CircularBuffer &&origin, allocator_type &&alloc) noexcept : CircularBuffer(origin) {
        allocator_ = alloc;
    }

    CircularBuffer &operator=(const CircularBuffer &rhs) {
        if (this != &rhs) {
            allocator_.deallocate(memory, capacity_);

            capacity_ = rhs.capacity_;
            memory = allocator_.allocate(capacity_);
            front_ = 0;
            back_ = 0;

            for (auto &it : rhs) {
                push_back(it);
            }
        }

        return *this;
    }

    ~CircularBuffer() {
        allocator_.deallocate(memory, capacity_);
    }

    /** Iterator.hpp **/
    struct iterator {
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;

        iterator(CircularBuffer &buffer, const size_t &index)
                : buffer_(buffer), index_(index) {};

        reference operator*() const { return buffer_.get_(index_); }

        pointer operator->() const { return &buffer_.get_(index_); }

        iterator &operator++() {
            index_++;
            return *this;
        }

        iterator operator++(T) {
            iterator temp = *this;

            ++*this;

            return temp;
        }

        iterator &operator--() {
            index_--;
            return *this;
        }

        iterator operator--(T) {
            iterator temp = *this;
            --*this;
            return temp;
        }

        bool operator!=(iterator &rhs) const {
            return index_ != rhs.index_;
        }

        reference operator[](size_t i) const {
            return buffer_.get_(buffer_.front_ + index_ + i);
        }

        iterator &operator+=(long n) {
            buffer_.increment(index_, n);

            return *this;
        }

        iterator &operator-=(long n) {
            buffer_.decrement(index_, n);

            return *this;
        }

        iterator &operator+(const long n) const {
            iterator iterator(buffer_, index_);
            buffer_.increment(iterator.index_, n);

            return iterator;
        }

        friend iterator &operator+(const long n, const iterator &i) {
            iterator iterator(i.buffer_, i.index_);
            i.buffer_.increment(iterator.index_, n);

            return iterator;
        }

        iterator &operator-(const long n) const {
            iterator iterator(buffer_, index_);
            buffer_.decrement(iterator.index_, n);

            return iterator;
        }

        friend iterator &operator-(long n, const iterator &i) {
            iterator iterator(i.buffer_, i.index_);
            i.buffer_.decrement(iterator.index_, n);

            return iterator;
        }

        size_t operator-(const iterator &rhs) const {
            if (buffer_.back_ - buffer_.front_ < 0) {
                return buffer_.capacity_ - abs(index_ - rhs.index_);
            } else {
                return abs(index_ - rhs.index_);
            }
        }

        bool operator<(const iterator &rhs) const {
            if (buffer_.back_ - buffer_.front_ < 0) {
                return buffer_.capacity_ - index_ + rhs.index_ < 0;
            } else {
                return index_ - rhs.index_ < 0;
            }
        }

        bool operator>(const iterator &rhs) const {
            if (buffer_.back_ - buffer_.front_ < 0) {
                return buffer_.capacity_ - index_ + rhs.index_ > 0;
            } else {
                return index_ - rhs.index_ > 0;
            }
        }

        bool operator<=(const iterator &rhs) const {
            if (buffer_.back_ - buffer_.front_ < 0) {
                return buffer_.capacity_ - index_ + rhs.index_ <= 0;
            } else {
                return index_ - rhs.index_ <= 0;
            }
        }

        bool operator>=(const iterator &rhs) const {
            if (buffer_.back_ - buffer_.front_ < 0) {
                return buffer_.capacity_ - index_ + rhs.index_ >= 0;
            } else {
                return index_ - rhs.index_ >= 0;
            }
        }

    private:
        CircularBuffer &buffer_;
        size_t index_;
    };

    /** Public methods **/
    void push_back(T &&value) { put_(value, true); }

    void push_back(T &value) { put_(value, true, false); }

    void push_front(T &&value) { put_(value, false); }

    void push_front(T &value) { put_(value, false, false); }

    T &pop_back() { return pop_(true); }

    T &pop_front() { return pop_(false); }

    T &front() const { return memory[front_]; }

    T &back() const { return memory[decremented(back_)]; }

    iterator begin() { return iterator(*this, 0); }

    iterator end() { return iterator(*this, size_); }

    void resize(size_t newSize) {
        if (capacity_ == newSize) { return; }

        T *newMemory = allocator_.allocate(newSize);

        size_t i = 0;
        for (auto &it : *this) {
            if (i == newSize) { break; }

            newMemory[i++] = it;
        }

        allocator_.deallocate(memory, capacity_);

        memory = newMemory;
        front_ = 0;
        back_ = (capacity_ < newSize) ? (capacity_) : (newSize == 0 ? 0 : newSize);
        capacity_ = newSize;
        if (size_ > newSize) {
            size_ = newSize;
        }
    }

    void sizeToFit() {
        allocator_.deallocate(front_, memory);
        allocator_.deallocate(capacity_ - back_ - 1, memory + back_);

        memory += front_;
    }

    const size_t &capacity() const { return capacity_; }

    const size_t &size() const { return size_; }

    bool isEmpty() const { return size_ == 0; }

    /** Operators **/
    T &operator[](const size_t &index) { return get_(index); }

    T &operator[](const size_t &&index) { return get_(index); }
};

#include "CircularBuffer/PrivateMethods.hpp"

#endif //LAB5_CIRCULARBUFFER_HPP
