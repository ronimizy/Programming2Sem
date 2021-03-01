//
// Created by Георгий Круглов on 25.02.2021.
//

#ifndef LAB5_CIRCULARBUFFER_HPP
#define LAB5_CIRCULARBUFFER_HPP

template<typename T>
class CircularBuffer {
    T **memory;
    size_t size_;

    size_t front_ = 0;
    size_t back_ = 0;

    void increment(size_t &i, long by = 1) {
        i = (i + by) % size_;
    }

    size_t incremented(size_t &i, long by = 1) const {
        return (i + by) % size_;
    }

    void decrement(size_t &i, long by = 1) {
        i = (i - by + size_) % size_;
    }

    size_t decremented(size_t &i, long by = 1) const {
        return (i - by + size_) % size_;
    }

    size_t span() const {
        if (back_ - front_ < 0) {
            return back_ - front_ + size_;
        } else {
            return back_ - front_;
        }
    }


    void put_(T *value, bool atBack) {
        if (size_) {
            if (atBack) {
                memory[back_] = value;

                if (incremented(back_) == front_) {
                    increment(front_);
                }

                increment(back_);
            } else {
                memory[front_] = value;

                if (decremented(front_) == back_) {
                    decrement(back_);
                }

                front_ = (front_ - 1 + size_) % size_;
            }
        }
    }

    T *get_(const size_t &index) const {
        if (size_) {
            return memory[(front_ + index) % size_];
        } else {
            return nullptr;
        }
    }

    T *pop_(bool atBack) {
        if (size_) {
            if (atBack) {
                T *value = memory[back_];

                if (decremented(back_) == front_) {
                    decrement(front_);
                }

                decrement(back_);

                return value;
            } else {
                T *value = memory[front_];

                if (incremented(front_) == back_) {
                    increment(back_);
                }

                increment(front_);

                return value;
            }
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
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;

        Iterator(CircularBuffer &buffer, const size_t &index) : buffer_(buffer), index_(index) {
            if (buffer.size_) {
                index_ %= buffer.size_;
            }
        };

        pointer operator->() const { return buffer_.get_(index_); }


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

        bool operator!=(Iterator &rhs) const {
            return index_ != rhs.index_;
        }

        pointer operator[](size_t i) const {
            return buffer_.get_(buffer_.front_ + index_ + i);
        }

        Iterator &operator+=(long n) {
            buffer_.increment(index_, n);

            return *this;
        }

        Iterator &operator-=(long n) {
            buffer_.decrement(index_, n);

            return *this;
        }

        Iterator &operator+(const long n) const {
            Iterator iterator(buffer_, index_);
            buffer_.increment(iterator.index_, n);

            return iterator;
        }

        friend Iterator &operator+(const long n, const Iterator &i) {
            Iterator iterator(i.buffer_, i.index_);
            i.buffer_.increment(iterator.index_, n);

            return iterator;
        }

        Iterator &operator-(const long n) const {
            Iterator iterator(buffer_, index_);
            buffer_.decrement(iterator.index_, n);

            return iterator;
        }

        friend Iterator &operator-(long n, const Iterator &i) {
            Iterator iterator(i.buffer_, i.index_);
            i.buffer_.decrement(iterator.index_, n);

            return iterator;
        }

        size_t operator-(const Iterator &rhs) const {
            if (buffer_.back_ - buffer_.front_ < 0) {
                return buffer_.size_ - abs(index_ - rhs.index_);
            } else {
                return abs(index_ - rhs.index_);
            }
        }

        bool operator<(const Iterator &rhs) const {
            if (buffer_.back_ - buffer_.front_ < 0) {
                return buffer_.size_ - index_ + rhs.index_ < 0;
            } else {
                return index_ - rhs.index_ < 0;
            }
        }

        bool operator>(const Iterator &rhs) const {
            if (buffer_.back_ - buffer_.front_ < 0) {
                return buffer_.size_ - index_ + rhs.index_ > 0;
            } else {
                return index_ - rhs.index_ > 0;
            }
        }

        bool operator<=(const Iterator &rhs) const {
            if (buffer_.back_ - buffer_.front_ < 0) {
                return buffer_.size_ - index_ + rhs.index_ <= 0;
            } else {
                return index_ - rhs.index_ <= 0;
            }
        }

        bool operator>=(const Iterator &rhs) const {
            if (buffer_.back_ - buffer_.front_ < 0) {
                return buffer_.size_ - index_ + rhs.index_ >= 0;
            } else {
                return index_ - rhs.index_ >= 0;
            }
        }


    private:
        CircularBuffer &buffer_;
        size_t index_;
    };

    /** Methods **/
    void push_back(T value) { put_(new T(value), true); }

    void push_back(T *value) { put_(value, true); }

    void push_front(T value) {
        T * a = new T;
        *a = value;
        put_(a, false); }

    void push_front(T *value) { put_(value, false); }

    T *pop_back() { return pop_(true); }
    T *pop_front() { return pop_(false); }

    T *front() const { return memory[front_]; }

    T *back() const { return memory[back_]; }

    Iterator begin() const { return Iterator(*this, front_); }

    Iterator end() const { return Iterator(*this, back_ + 1); }

    void resize(size_t newSize) {
        if (size_ == newSize) { return; }

        T **n = new T *[newSize];

        for (size_t i = 0; i < ((size_ < newSize) ? size_ : newSize); ++i) {
            n[i] = new T(*memory[i]);
        }

        delete[] memory;

        memory = n;
        front_ = 0;
        back_ = (size_ < newSize) ? (size_ - 1) : (newSize == 0 ? 0 : newSize - 1);
        size_ = newSize;
    }

    size_t capacity() const { return size_; }

    size_t size() const { return span(); }

    /** Operators **/
    T *operator[](const size_t &index) { return get_(index); }

    T *operator[](const size_t &&index) { return get_(index); }
};

#endif //LAB5_CIRCULARBUFFER_HPP
