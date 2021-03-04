//
// Created by Георгий Круглов on 25.02.2021.
//

#ifndef LAB5_CIRCULARBUFFER_HPP
#define LAB5_CIRCULARBUFFER_HPP

template<typename T, typename _Alloc = std::allocator<T> >
class CircularBuffer {
    _Alloc allocator_ = _Alloc();

    T *memory_;

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
        memory_ = std::allocator_traits<_Alloc>::allocate(allocator_, capacity);
    }

    //Copy constructor
    CircularBuffer(const CircularBuffer &origin) : capacity_(origin.capacity_) {
        memory_ = std::allocator_traits<_Alloc>::allocate(allocator_, origin.capacity_);

        for (auto &it : origin) {
            push_back(it);
        }
    }

    //Copy insert constructor
    CircularBuffer(const CircularBuffer &origin, _Alloc &alloc) : CircularBuffer(origin) {
        allocator_ = alloc;
    }

    //Move constructor
    CircularBuffer(CircularBuffer &&origin) noexcept: capacity_(origin.capacity_) {
        memory_ = origin.memory_;
        origin.memory_ = nullptr;
    }

    //Move insert constructor
    CircularBuffer(CircularBuffer &&origin, _Alloc &&alloc) noexcept : CircularBuffer(origin) {
        allocator_ = alloc;
    }

    CircularBuffer &operator=(const CircularBuffer &rhs) {
        if (this != &rhs) {
            allocator_ = rhs.get_allocator();

            std::allocator_traits<_Alloc>::deallocate(allocator_, capacity_, memory_);

            capacity_ = rhs.capacity_;
            memory_ = std::allocator_traits<_Alloc>::allocate(allocator_, capacity_);
            front_ = 0;
            back_ = 0;

            for (auto &it : rhs) {
                push_back(it);
            }
        }

        return *this;
    }

    ~CircularBuffer() {
        std::allocator_traits<_Alloc>::deallocate(allocator_, capacity_, memory_);
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

    T &front() const { return memory_[front_]; }

    T &back() const { return memory_[decremented(back_)]; }

    iterator begin() { return iterator(*this, 0); }

    iterator end() { return iterator(*this, size_); }

    void resize(size_t newSize) {
        if (capacity_ == newSize) { return; }

        T *newMemory = std::allocator_traits<_Alloc>::allocate(allocator_, newSize);

        size_t i = 0;
        for (auto &it : *this) {
            if (i == newSize) { break; }

            newMemory[i++] = it;
        }

        std::allocator_traits<_Alloc>::deallocate(allocator_, capacity_, memory_);

        memory_ = newMemory;
        front_ = 0;
        back_ = (capacity_ < newSize) ? (capacity_) : (newSize == 0 ? 0 : newSize);

        capacity_ = newSize;
        if (size_ > newSize) {
            size_ = newSize;
        }
    }

    void sizeToFit() {
        memory_ = std::allocator_traits<_Alloc>::allocate(allocator_, front_, memory_);
        memory_ = std::allocator_traits<_Alloc>::allocate(allocator_, capacity_ - back_ - 1, memory_ + back_);

        memory_ += front_;
    }

    void swap(CircularBuffer &rhs) {
        T* tmpMem = memory_;
        size_t  tmpCap = capacity_,
                tmpFront = front_,
                tmpBack = back_,
                tmpSize = size_;

        memory_ = rhs.memory_;
        capacity_ = rhs.capacity_;
        front_ = rhs.front_;
        back_ = rhs.back_;
        size_ = rhs.size_;

        rhs.memory_ = tmpMem;
        rhs.capacity_ = tmpCap;
        rhs.front_ = tmpFront;
        rhs.back_ = tmpBack;
        rhs.size_ = tmpSize;
    }

    _Alloc get_allocator() { return allocator_; }

    using allocator_type = _Alloc;
    using value_type = T;

    const size_t &capacity() const { return capacity_; }

    const size_t &size() const { return size_; }

    bool isEmpty() const { return size_ == 0; }

    /** Operators **/
    T &operator[](const size_t &index) { return get_(index); }

    T &operator[](const size_t &&index) { return get_(index); }
};

#include "CircularBuffer/PrivateMethods.hpp"

#endif //LAB5_CIRCULARBUFFER_HPP
