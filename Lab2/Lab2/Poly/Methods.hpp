//
// Created by Георгий Круглов on 22.02.2021.
//

#ifndef POLY_METHODS_HPP
#define POLY_METHODS_HPP

#include "../Poly.hpp"

template<typename T>
size_t Poly<T>::size() const {
    return size_;
}

template<typename T>
size_t Poly<T>::sizeToFit() {
    size_t i = 0, deleted = 0;

    while (true) {
        while (i < coefficients_.size() - 1 && coefficients_[i + 1] == 0) {
            coefficients_.erase(i + 1);
            ++deleted;
        }

        if (i + 1 >= coefficients_.size()) {
            break;
        }

        ++i;
    }
    
    return deleted;
}

#endif
