//
// Created by Георгий Круглов on 24.02.2021.
//

#ifndef LAB4_RNALGORITHM_HPP
#define LAB4_RNALGORITHM_HPP

namespace RNAlgorithm {

    template<typename T, typename iterator = typename T::iterator, typename Predicate>
    bool all_of(iterator begin, iterator end, Predicate predicate) {
        for (auto it = begin; it != end; it++) {
            if (!predicate(*it)) {
                return false;
            }
        }

        return true;
    }

    template<typename T, typename iterator = typename T::iterator, typename Predicate>
    bool any_of(iterator begin, iterator end, Predicate predicate) {
        for (auto it = begin; it != end; it++) {
            if (predicate(*it)) {
                return true;
            }
        }

        return false;
    }

    template<typename T, typename iterator = typename T::iterator, typename Predicate>
    bool none_of(iterator begin, iterator end, Predicate predicate) {
        for (auto it = begin; it != end; it++) {
            if (predicate(*it)) {
                return false;
            }
        }

        return true;
    }

    template<typename T, typename iterator = typename T::iterator, typename Predicate>
    bool one_of(iterator begin, iterator end, Predicate predicate) {
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

    template<typename T, typename iterator = typename T::iterator, typename Predicate>
    bool is_sorted(iterator begin, iterator end, Predicate predicate) {
        if (begin == end) {
            return true;
        }

        auto prev = begin++;

        for (auto it = begin; it != end; it++) {
            if (!predicate(*prev, *it)) {
                return false;
            }

            prev = it;
        }

        return true;
    }

    template<typename T, typename iterator = typename T::iterator, typename Predicate>
    bool is_partitioned(iterator begin, iterator end, Predicate predicate) {
        if (begin == end) {
            return false;
        }

        bool last = predicate(*(begin++));

        bool flipped = false;
        size_t before;
        size_t after;

        for (auto it = begin; it != end; it++) {
            if (predicate(*it) != last) {
                if (flipped) {
                    return false;
                } else {
                    flipped = true;
                    last = !last;
                }
            } else {
                if (flipped) {
                    after++;
                } else {
                    before++;
                }
            }
        }

        return flipped && (before > 1 || after > 1);
    }

    template<typename T, typename iterator = typename T::iterator >
    T *find_not(iterator begin, iterator end, T &&nemesis) {
        for (auto it = begin; it != end; it++) {
            if (*it != nemesis) {
                return &(*it);
            }
        }

        return nullptr;
    }

    template<typename T, typename iterator = typename T::iterator >
    T *find_backward(iterator begin, iterator end, T &&ally) {
        for (auto it = end; it != begin; it--) {
            if (*it == ally) {
                return &(*it);
            }
        }

        return nullptr;
    }

    template<typename T, typename iterator = typename T::iterator >
    bool is_palindrome(iterator left, iterator right) {
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
