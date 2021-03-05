//
// Created by Георгий Круглов on 22.02.2021.
//

#ifndef LAB2_POLY_H
#define LAB2_POLY_H

#include <vector>
#include <iostream>

template<typename T>
struct Node {
    size_t first_;
    T second_;

    Node(size_t first, size_t second) : first_(first), second_(second) {};
    Node() : first_(0), second_(0) {};
};

template<typename T>
class Poly;

template<typename T>
std::istream &operator>>(std::istream &in, Poly<T> &p);

template<typename T>
std::ostream &operator<<(std::ostream &out, const Poly<T> &p);

template<typename T>
class Poly {
    std::vector<Node<T>> coefficients;

    size_t get(size_t index) const;

public:
    int size() const;

    T coefficient(size_t index) const;

    Node<T> &add(size_t index, T value);

    T substitute(T x) const;

    void sizeToFit();

    Poly(std::vector<T> c);
    Poly(std::vector<Node<T>> c);

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

    T operator[](size_t index) const;

    T &operator[](size_t index);

    friend std::istream &operator>><>(std::istream &in, Poly<T> &p);

    friend std::ostream &operator
    <<<>(
    std::ostream &out,
    const Poly<T> &p
    );
};


#include "Poly/Constructors.hpp"
#include "Poly/Operators.hpp"
#include "Poly/Methods.hpp"

#endif //LAB2_POLY_H
