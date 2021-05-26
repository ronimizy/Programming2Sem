//
// Created by Георгий Круглов on 01.03.2021.
//

#ifndef LAB5_PRIVATE_METHODS_HPP
#define LAB5_PRIVATE_METHODS_HPP

#include "../CircularBuffer.hpp"

/** Index changes **/
//Increment
template<typename T, typename allocator_type>
void CircularBuffer<T, allocator_type>::increment(size_t &i, long by, size_t mod) const {
    if ((long) i + by < 0) {
        by = (capacity_ + by) % capacity_;
    }
    i += by;
    i %= capacity_;
}

//Incremented
template<typename T, typename allocator_type>
size_t CircularBuffer<T, allocator_type>::incremented(const size_t &i, long by) const {
    size_t result = i;
    increment(result, by);

    return result;
}

/** Data Arrangement **/
//Put
template<typename T, typename allocator_type>
void CircularBuffer<T, allocator_type>::put_(const T &value, bool atBack) {
    if (capacity_) {
        if (size_ == capacity_) {
            changeCapacity(2 * capacity_);
        }

        std::allocator_traits<allocator_type>::construct(allocator_,
                                                         &memory_[atBack ? back_ : front_], value);
        increment(atBack ? back_ : front_,
                  atBack ? 1 : -1);

        size_++;
    }
}

//Get
template<typename T, typename allocator_type>
T &CircularBuffer<T, allocator_type>::get_(const size_t &index) const {
    if (capacity_ && size_) {
        return memory_[incremented(front_, index + 1)];
    } else {
        throw std::out_of_range("Empty circular buffer");
    }
}

//Pop
template<typename T, typename allocator_type>
T CircularBuffer<T, allocator_type>::pop_(bool atBack) {
    if (capacity_ && size_) {
        increment(atBack ? back_ : front_,
                  atBack ? - 1 : 1);
        T value(memory_[atBack ? back_ : front_]);
        traits_::destroy(allocator_, &memory_[atBack ? back_ : front_]);

        --size_;

        return value;
    } else {
        throw std::out_of_range("Empty circular buffer");
    }
}


#endif //LAB5_PRIVATE_METHODS_HPP
