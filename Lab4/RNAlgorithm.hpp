//
// Created by Георгий Круглов on 24.02.2021.
//

#ifndef LAB4_RNALGORITHM_HPP
#define LAB4_RNALGORITHM_HPP

namespace RNAlgorithm {

    template<typename T, typename Allocator = std::allocator<T>, typename Predicate>
    bool all_of(Allocator begin, Allocator end, Predicate predicate) {
        for (auto it = begin; it != end; it++) {
            if (!predicate(*it)) {
                return false;
            }
        }

        return true;
    }

    template<typename T, typename Allocator = std::allocator<T>, typename Predicate>
    bool any_of(Allocator begin, Allocator end, Predicate predicate) {
        for (auto it = begin; it != end; it++) {
            if (predicate(*it)) {
                return true;
            }
        }

        return false;
    }

    template<typename T, typename Allocator = std::allocator<T>, typename Predicate>
    bool none_of(Allocator begin, Allocator end, Predicate predicate) {
        for (auto it = begin; it != end; it++) {
            if (predicate(*it)) {
                return false;
            }
        }

        return true;
    }

    template<typename T, typename Allocator = std::allocator<T>, typename Predicate>
    bool one_of(Allocator begin, Allocator end, Predicate predicate) {
        bool satisfied = false;

        for (auto it = begin; it != end; it++) {
            if (predicate(*it)) {
                if (satisfied) {
                    return false;
                } else {
                    satisfied = true;
                }
            }
        }

        return satisfied;
    }

    template<typename T, typename Allocator = std::allocator<T>, typename Predicate>
    bool is_sorted(Allocator begin, Allocator end, Predicate predicate) {
        auto prev = begin++;

        for (auto it = begin; it != end; it++) {
            if (!predicate(*prev, *it)) {
                return false;
            }

            prev = it;
        }

        return true;
    }

    //ToDo – is_partitioned
    template<typename T, typename Allocator = std::allocator<T>, typename Predicate>
    bool is_partitioned(Allocator begin, Allocator end, Predicate predicate) {
        for (auto it = begin; it != end; it++) {
            if (!predicate(*begin)) {
                return false;
            }
        }

        return true;
    }

    template<typename T, typename Allocator = std::allocator<T> >
    T *find_not(Allocator begin, Allocator end, T&& nemesis) {
        for (auto it = begin; it != end; it++) {
            if (*it != nemesis) {
                return &(*it);
            }
        }

        return nullptr;
    }

    template<typename T, typename Allocator = std::allocator<T> >
    T *find_backward(Allocator begin, Allocator end, T&& ally) {
        for (auto it = end; it != begin; it--) {
            if (*it == ally) {
                return &(*it);
            }
        }

        return nullptr;
    }

    template<typename T, typename Allocator = std::allocator<T> >
    bool is_palindrome(Allocator left, Allocator right) {
        right--;

        while (left != right) {
            if (*left != *right) {
                return false;
            }

            if (right == ++left) {
                return true;
            } else {
                right--;
            }
        }

        return true;
    }
}

#endif //LAB4_RNALGORITHM_HPP
