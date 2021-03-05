//
// Created by Георгий Круглов on 22.02.2021.
//

#ifndef POLY_CONSTRUCTORS_HPP
#define POLY_CONSTRUCTORS_HPP

#include "../Poly.h"

//Default constructor
template<typename T> Poly<T>::Poly(std::vector <T> c) {
    for (size_t i = 0; i < c.size(); ++i) {
        coefficients.emplace_back(i, c[i]);
    }
};

template<typename T> Poly<T>::Poly(std::vector <Node<T>> c) : coefficients(std::move(c)) {};

//Copy constructor
template<typename T> Poly<T>::Poly(const Poly<T> &origin) : coefficients(origin.coefficients) {};

//Move constructor
template<typename T> Poly<T>::Poly(Poly<T> &&origin) noexcept : coefficients(std::move(origin.coefficients)) {};

//Blank constructor
template<typename T> Poly<T>::Poly()  : coefficients(std::vector<T>()) {};

//Size constructor
template<typename T> Poly<T>::Poly(size_t size)  : coefficients(std::vector<T>(size)) {};

//Destructor
template<typename T> Poly<T>::~Poly() {
//    coefficients.resize(0);
    std::cout << "Destructor's work have been done" << std::endl;
}

#endif