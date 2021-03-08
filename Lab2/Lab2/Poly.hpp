//
// Created by Георгий Круглов on 22.02.2021.
//

#ifndef LAB2_POLY_H
#define LAB2_POLY_H

#include <vector>
#include <map>
#include <iostream>

#include "Poly/ToString.hpp"

//MARK: - class Poly
template<typename T>
class Poly;

//MARK: IO Declaration
template<typename T>
std::istream &operator>>(std::istream &in, Poly<T> &p);

template<typename T>
std::ostream &operator<<(std::ostream &out, const Poly<T> &p);

template<typename T>
class Poly {
    //MARK: Values
    std::map<size_t, T> coefficients_;
    size_t size_ = 0;


public:
    //MARK: Methods
    size_t size() const;

    //
    //returns a number of deleted objects
    size_t sizeToFit();
    
    typename std::map<size_t, T>::iterator begin() { return coefficients_.begin(); }
    typename std::map<size_t, T>::iterator end() { return coefficients_.end(); }

    //MARK: Life cycle
    //Size constructor
    Poly(size_t size) : size_(std::move(size)) {
        for (size_t i = 0; i < size_; ++i) {
            coefficients_[i] = 0;
        }
    }
    
    //Default constructor
    Poly(std::initializer_list<std::pair<const size_t, T>> init = {})
        : coefficients_(init) {
            for (auto &it : init) {
                if (it.first + 1 > size_) {
                    size_ = it.first + 1;
                }
            }
        };
    
    //
    Poly(std::initializer_list<T> &&init)
        : size_(init.size()) {
        size_t i = 0;
        
        for (auto &it : init) {
            coefficients_[i++] = it;
        }
    }

    //Map constructor
    Poly(std::map<size_t, T> coefficients)
        : coefficients_(coefficients) {
            for (auto &it : coefficients) {
                if (it.first + 1 > size_) {
                    size_ = it.first + 1;
                }
            }
        };

    //Copy constructor
    Poly(const Poly &origin)
        : coefficients_(origin.coefficients_), size_(origin.size_) {};

    //Move constructor
    Poly(Poly &&origin) noexcept
        : coefficients_(std::move(origin.coefficients_)), size_(std::move(origin.size_)) {};

    //Destructor
    ~Poly() {
        std::clog << "Poly object has been destructed\n";
    };

    //MARK: Operators

    //Substitute
    T operator()(const T &x) const;
    
        //MARK: Comparison
    bool operator==(const Poly &rhs) const;

    bool operator!=(const Poly &rhs) const;

        //MARK: Math
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

        //MARK: Subscript
    std::pair<const size_t, T> operator[](size_t index) const;

    std::pair<const size_t, T> &operator[](size_t index);

        //MARK: IO
    friend std::istream &operator>> <>(std::istream &in, Poly<T> &p);

    friend std::ostream &operator<< <>(std::ostream &out,const Poly<T> &p);
};


#include "Poly/Operators.hpp"
#include "Poly/Methods.hpp"

#endif //LAB2_POLY_H
