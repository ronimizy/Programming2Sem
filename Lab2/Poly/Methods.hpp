//
// Created by Георгий Круглов on 22.02.2021.
//

#ifndef POLY_METHODS_HPP
#define POLY_METHODS_HPP

#include "../Poly.h"

//Max degree
template<typename T> int Poly<T>::size() const {
    return coefficients.size();
}

//Coefficient at index
template<typename T> T Poly<T>::coefficient(unsigned int number) const {
    if (number < size()) {
        return coefficients[number];
    } else {
        return 0;
    }
}

template<typename T> void Poly<T>::add(int number, T value)  {
    if (number < size()) {
        coefficients[number] += value;
    } else {
        coefficients.resize(number + 1);
        coefficients[number] += value;
    }
}

template<typename T> T Poly<T>::substitute(T x) const {
    T sum;
    T degree = x;

    for (int i = 0; i < coefficients.size(); i++) {
        if (i == 0) {
            sum += coefficients[0];
        } else {
            sum += coefficients[i] * degree;
            degree *= x;
        }
    }

    return sum;
}

#endif