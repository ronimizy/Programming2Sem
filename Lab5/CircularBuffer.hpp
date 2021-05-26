//
// Created by Георгий Круглов on 25.02.2021.
//

#ifndef LAB5_CIRCULAR_BUFFER_HPP
#define LAB5_CIRCULAR_BUFFER_HPP


template<typename T, typename _Alloc = std::allocator<T> >
class CircularBuffer {
    using traits_ = std::allocator_traits<_Alloc>;

    size_t size_ = 0;
    size_t capacity_;

    size_t front_ = 0;
    size_t back_ = 1;

    _Alloc allocator_;
    T *memory_;


    /** PrivateMethods.hpp **/
    void increment(size_t &i, long by = 1, size_t mod = 1) const;

    size_t incremented(const size_t &i, long by = 1) const;

    void put_(const T &value, bool atBack);

    T &get_(const size_t &index) const;

    T pop_(bool atBack);

    void changeCapacity(size_t __capacity) {
        if (capacity_ == __capacity) { return; }

        T *newMemory = traits_::allocate(allocator_, __capacity);

        size_t i = 0;
        for (auto &it : *this) {
            if (i != __capacity) {
                traits_::construct(allocator_, &newMemory[1 + i++], it);
            }

            traits_::destroy(allocator_, &it);
        }

        traits_::deallocate(allocator_, memory_, capacity_);

        memory_ = newMemory;
        back_ = (capacity_ < __capacity) ? size_ + 1 : __capacity + 1;
        front_ = 0;

        capacity_ = __capacity;
        if (size_ > __capacity) {
            size_ = __capacity;
        }
    }

public:
    /** Life cycle **/
    //Default constructor
    CircularBuffer(size_t size = 0, T defaultValue = T(), _Alloc allocator = _Alloc())
            : capacity_(size ? 2 * size : 10),
              allocator_(allocator),
              memory_(traits_::allocate(allocator_, capacity_)) {
        for (size_t i = 0; i < size; ++i) {
            put_(defaultValue, true);
        }
    };

    //Copy constructor
    CircularBuffer(const CircularBuffer &origin)
            : capacity_(origin.capacity_),
              back_(origin.size_) {
        if (traits_::propagate_on_container_copy_assignment::value) {
            allocator_ = traits_::select_on_container_copy_construction(origin.allocator_);
        } else {
            allocator_ = _Alloc();
        }

        memory_ = traits_::allocate(allocator_, capacity_);

        for (size_t i = 0; i < origin.size_; ++i) {
            put_(origin.get_(i), true);
        }
    }

    //Move constructor
    CircularBuffer(CircularBuffer &&origin) noexcept
            : capacity_(std::move(origin.capacity)),
              back_(std::move(origin.size_)) {
        if (traits_::propagate_on_container_move_assignment::value) {
            allocator_ = std::move(origin.allocator_);
        } else {
            allocator_ = _Alloc();
        }

        memory_ = std::move(origin.memory_);
    }

    CircularBuffer &operator=(const CircularBuffer &rhs) {
        if (this != &rhs) {
            clean();

            if (traits_::propagate_on_container_copy_assignment::value) {
                allocator_ = traits_::select_on_container_copy_construction(rhs.allocator_);
            }

            capacity_ = rhs.capacity_;
            memory_ = std::allocator_traits<_Alloc>::allocate(allocator_, capacity_);
            front_ = 0;
            back_ = 0;

            for (auto &&it : rhs) {
                push_back(it);
            }
        }

        return *this;
    }

    ~CircularBuffer() { clean(); }


    /** Iterator.hpp **/
    struct iterator {
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;

        iterator(CircularBuffer *buffer, const size_t &index)
                : buffer_(buffer), index_(index) {};

        reference operator*() const { return buffer_->get_(index_); }

        pointer operator&() const { return &buffer_->get_(index_); }

        pointer operator->() const { return &buffer_->get_(index_); }

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

        bool operator==(const iterator &rhs) const {
            return index_ == rhs.index_;
        }

        bool operator!=(iterator &rhs) const {
            return index_ != rhs.index_;
        }

        reference operator[](size_t i) const {
            return buffer_->get_(buffer_->front_ + index_ + i);
        }

        iterator &operator+=(long n) {
            buffer_->increment(index_, n);

            return *this;
        }

        iterator &operator-=(long n) {
            buffer_->decrement(index_, n);

            return *this;
        }

        iterator operator+(const long n) const {
            iterator iterator(buffer_, index_);
            buffer_->increment(iterator.index_, n);

            return iterator;
        }

        friend iterator &operator+(const long n, const iterator &i) {
            iterator iterator(i.buffer_, i.index_);
            i.buffer_->increment(iterator.index_, n);

            return iterator;
        }

        iterator &operator-(const long n) const {
            iterator iterator(buffer_, index_);
            buffer_->decrement(iterator.index_, n);

            return iterator;
        }

        friend iterator &operator-(long n, const iterator &i) {
            iterator iterator(i.buffer_, i.index_);
            i.buffer_->decrement(iterator.index_, n);

            return iterator;
        }

        size_t operator-(const iterator &rhs) const {
            if (buffer_->back_ - buffer_->front_ < 0) {
                return buffer_->capacity_ - index_ + rhs.index_;
            } else {
                return index_ - rhs.index_;
            }
        }

        bool operator<(const iterator &rhs) const {
            if (buffer_->back_ - buffer_->front_ < 0) {
                return buffer_->capacity_ - index_ + rhs.index_ < 0;
            } else {
                return index_ - rhs.index_ < 0;
            }
        }

        bool operator>(const iterator &rhs) const {
            if (buffer_->back_ - buffer_->front_ < 0) {
                return buffer_->capacity_ - index_ + rhs.index_ > 0;
            } else {
                return index_ - rhs.index_ > 0;
            }
        }

        bool operator<=(const iterator &rhs) const {
            if (buffer_->back_ - buffer_->front_ < 0) {
                return buffer_->capacity_ - index_ + rhs.index_ <= 0;
            } else {
                return index_ - rhs.index_ <= 0;
            }
        }

        bool operator>=(const iterator &rhs) const {
            if (buffer_->back_ - buffer_->front_ < 0) {
                return buffer_->capacity_ - index_ + rhs.index_ >= 0;
            } else {
                return index_ - rhs.index_ >= 0;
            }
        }

    private:
        CircularBuffer *buffer_;
        size_t index_;
    };


    /** Public methods **/
    using allocator_type = _Alloc;
    using value_type = T;

    const size_t &capacity() const { return capacity_; }

    const size_t &size() const { return size_; }

    bool isEmpty() const { return size_ == 0; }

    T &front() const { return get_(front_); }

    T &back() const { return get_(back_ - 2); }

//Uses passed index as offset from front_
    iterator begin() { return iterator(this, 0); }

    iterator end() { return iterator(this, size_); }

    /** Inserting **/
    void push_back(T &&value) { put_(value, true); }

    void push_back(const T &value) { put_(value, true); }

    void push_front(T &&value) { put_(value, false); }

    void push_front(const T &value) { put_(value, false); }

//    template<typename... Args>
//    void emplace_back(Args &&... init) { put_(T(init...), true); };
//
//    template<typename... Args>
//    void emplace_front(Args &&... init) { put_(T(init...), false); };

    /** Ejecting **/
    T pop_back() { return pop_(true); }

    T pop_front() { return pop_(false); }


    /** Memory **/
    void resize(size_t size, T defaultValue = T()) {
        if (size_ == size)
            return;

        if (size > capacity_) {
            changeCapacity(2 * size);
        }

        for (size_t i = size; i < size_; ++i) {
            traits_::destroy(allocator_, &get_(i));
        }

        for (size_t i = size_; i < size; ++i) {
            traits_::construct(allocator_, &((*this)[i]), defaultValue);
        }

        back_ -= size_ - size;
        size_ = size;
    }

    void sizeToFit() { changeCapacity(size_); }

    void clean() { resize(0); }

    void swap(CircularBuffer &rhs) noexcept {
        std::swap(memory_, rhs.memory_);
        std::swap(capacity_, rhs.capacity_);
        std::swap(front_, rhs.front_);
        std::swap(back_, rhs.back_);
        std::swap(size_, rhs.size_);

        if (std::allocator_traits<allocator_type>::propagate_on_container_swap::value) {
            std::swap(allocator_, rhs.allocator_);
        }
    }

    _Alloc &get_allocator() { return allocator_; }


    /** Operators **/
    T &operator[](const size_t &index) { return get_(index); }

    T &operator[](const size_t &&index) { return get_(index); }
};

#include "CircularBuffer/PrivateMethods.hpp"

#endif //LAB5_CIRCULAR_BUFFER_HPP
