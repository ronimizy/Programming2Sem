//
// Created by Георгий Круглов on 01.03.2021.
//

#ifndef LAB5_PRIVATEMETHODS_HPP
#define LAB5_PRIVATEMETHODS_HPP

#include "../CircularBuffer.hpp"

/** Index changes **/
//Increment
template<typename T, typename allocator_type>
void CircularBuffer<T, allocator_type>::increment(size_t &i, long by) {
    i = (i + by) % capacity_;
}

//Incremented
template<typename T, typename allocator_type>
size_t CircularBuffer<T, allocator_type>::incremented(const size_t &i, long by) const {
    return (i + by) % capacity_;
}

//Decrement
template<typename T, typename allocator_type>
void CircularBuffer<T, allocator_type>::decrement(size_t &i, long by) {
    i = (i - by + capacity_) % capacity_;
}

//Decremented
template<typename T, typename allocator_type>
size_t CircularBuffer<T, allocator_type>::decremented(const size_t &i, long by) const {
    return (i - by + capacity_) % capacity_;
}

/** Data Arrangement **/
//Put
template<typename T, typename allocator_type>
void CircularBuffer<T, allocator_type>::put_(T &value, bool atBack, bool copy) {
    if (capacity_) {
        if (size_ == capacity_) {
            resize(capacity_ + 1);
        }

        if (atBack) {
            if (copy) {
                std::allocator_traits<allocator_type>::construct(allocator_, &memory_[back_], value);
            } else {
                memory_[back_] = value;
            }

            increment(back_);
            size_++;
        } else {
            if (copy) {
                std::allocator_traits<allocator_type>::construct(allocator_, &memory_[front_], value);
            } else {
                memory_[front_] = value;
            }

            increment(front_);
            size_++;
        }
    }
}

//Get
template<typename T, typename allocator_type>
T& CircularBuffer<T, allocator_type>::get_(const size_t &index) const {
    if (capacity_ && size_) {
        return memory_[incremented(front_, index)];
    } else {
        throw std::out_of_range("Empty circular buffer");
    }
}

//Pop
template<typename T, typename allocator_type>
T& CircularBuffer<T, allocator_type>::pop_(bool atBack) {
    if (capacity_ && size_) {
        if (atBack) {
            decrement(back_);
            T &value = memory_[back_];
            size_--;

            return value;
        } else {
            increment(front_);
            T &value = memory_[front_];
            size_--;

            return value;
        }
    } else {
        throw std::out_of_range("Empty circular buffer");
    }
}


#endif //LAB5_PRIVATEMETHODS_HPP
