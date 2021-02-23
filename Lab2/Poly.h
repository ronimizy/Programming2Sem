//
// Created by Георгий Круглов on 22.02.2021.
//

#ifndef LAB2_POLY_H
#define LAB2_POLY_H

#include <vector>
#include <iostream>

template<typename T>
class Poly {
    std::vector <T> coefficients;

public:
    int size() const;

    T coefficient(unsigned int number) const;

    void add(int number, T value);

    T substitute(T x) const;

    Poly(std::vector <T> c);

    Poly(const Poly &origin);

    Poly(Poly &&origin) noexcept;

    Poly();

    Poly(size_t size);

    ~Poly();

    bool operator==(const Poly &rhs) const;

    bool operator!=(const Poly &rhs) const;

    Poly operator+(Poly &rhs) const;

    Poly operator-() const;

    Poly operator-(Poly &rhs) const;

    Poly &operator+=(Poly &rhs);

    Poly &operator-=(Poly &rhs);

    Poly operator*(Poly &rhs) const;

    Poly operator*(T &rhs) const;

    Poly operator/(T &rhs) const;

    Poly &operator*=(Poly &rhs);

    Poly &operator*=(T &rhs);

    Poly &operator/=(T &rhs);

    T operator[](int i) const;

    T& operator[](int i);

    friend std::istream &operator>>(std::istream &in, Poly<T> &p);

    friend std::ostream &operator<<(std::ostream &out, const Poly<T> &p);
};


#include "Poly/Constructors.hpp"
#include "Poly/Operators.hpp"
#include "Poly/Methods.hpp"

#endif //LAB2_POLY_H
