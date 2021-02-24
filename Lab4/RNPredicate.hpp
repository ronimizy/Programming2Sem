//
// Created by Георгий Круглов on 24.02.2021.
//

#ifndef LAB4_RNPREDICATE_HPP
#define LAB4_RNPREDICATE_HPP

namespace RNPredicate {
    /** Comparison Predicates **/
    template<typename T>
    struct MoreThan {
        MoreThan(T &&value) : superior(value) {};

        bool operator()(T &lhs) {
            return lhs > superior;
        }

    private:
        T superior;
    };
    template<typename T>
    struct LessThan {
        LessThan(T &&value) : superior(value) {};

        bool operator()(T &lhs) {
            return lhs < superior;
        }

    private:
        T superior;
    };
    template<typename T>
    struct EqualTo {
        EqualTo(T value) : icon(value) {};

        bool operator()(T &lhs) {
            return lhs == icon;
        }

    private:
        T icon;
    };

    /** Sorting Predicates **/
    template<typename T>
    struct Increasing {
        bool operator()(T &lhs, T &rhs) {
            return lhs <= rhs;
        }
    };

    template<typename T>
    struct Decreasing {
        bool operator()(T &lhs, T &rhs) {
            return lhs >= rhs;
        }
    };
}

#endif //LAB4_RNPREDICATE_HPP
