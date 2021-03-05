//
// Created by Георгий Круглов on 22.02.2021.
//

#ifndef POLY_METHODS_HPP
#define POLY_METHODS_HPP

#include "../Poly.h"

//Max degree
template<typename T>
int Poly<T>::size() const {
    return coefficients.back().first_ + 1;
}

template<typename T>
size_t Poly<T>::get(size_t index) const {
    size_t left = 0, right = coefficients.size(), mid;

    while (right - left > 1) {
        mid = (left + right) / 2;

        if (coefficients[mid].first_ == index) {
            break;
        }

        if (coefficients[mid].first_ < index) {
            left = mid;
        } else {
            right = mid;
        }
    }

    return mid;
}

template<typename T>
void Poly<T>::sizeToFit() {
    size_t i = 0;

    while (true) {
        while (i < coefficients.size() - 1 && coefficients[i + 1].second_ == 0) {
            coefficients.erase(coefficients.begin() + i + 1);
        }

        if (i + 1 >= coefficients.size()) {
            break;
        }

        ++i;
    }
}

//Coefficient at index
template<typename T>
T Poly<T>::coefficient(size_t index) const {
    if (index < size()) {
        size_t i = get(index);

        if (coefficients[i].first_ == index) {
            return coefficients[i].second_;
        }
    }

    return 0;
}

template<typename T>
Node<T> &Poly<T>::add(size_t index, T value) {
    if (index < size()) {
        size_t i = get(index);

        if (coefficients[i].first_ == index) {
            coefficients[i].second_ += value;
        } else {
            coefficients.insert(i, Node(index, value));
        }

        return coefficients[i];
    } else {
        coefficients.emplace_back(index, value);

        return coefficients.back();
    }
}

template<typename T>
T Poly<T>::substitute(T x) const {
    T sum;
    size_t degree;
    T multiplier = x;

    for (Node<T> &node : coefficients) {
        if (node.first_ == 0) {
            sum += node.second_;
        } else {
            for (; degree < node.first_; ++degree) {
                multiplier *= x;
            }

            sum += node.second_ * multiplier;
        }
    }

    return sum;
}

#endif