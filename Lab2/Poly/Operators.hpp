//
// Created by Георгий Круглов on 22.02.2021.
//

#ifndef POLY_OPERATORS_HPP
#define POLY_OPERATORS_HPP

#include "../Poly.hpp"

//MARK: Substitute
template<typename T>
T Poly<T>::operator()(const T &x) const {
    T sum;
    size_t degree;
    T multiplier = x;
    
    for (auto &node : coefficients_) {
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

//MARK: - Comparison
template<typename T>
bool Poly<T>::operator==(const Poly<T> &rhs) const {
    if (size_ != rhs.size_) {
        return false;
    }
    
    for (size_t i = 0; i < size(); ++i) {
        if (coefficients_.contains(i) != rhs.coefficients_.contains(i)
            || coefficients_[i] != rhs.coefficients_[i]) {
            return false;
        }
    }
    
    return true;
}

template<typename T>
bool Poly<T>::operator!=(const Poly<T> &rhs) const {
    return !(*this == rhs);
}

//MARK: - Math
template<typename T>
Poly<T> Poly<T>::operator-() const {
    Poly buffer(*this);
    
    for (auto &it : buffer.coefficients_) {
        it = -it;
    }
    
    return buffer;
}

//MARK: Sum
template<typename T>
Poly<T> &Poly<T>::operator+=(Poly<T> &rhs) {
    for (const auto &[key, value] : rhs.coefficients_) {
        coefficients_[key] += value;
    }
    
    return *this;
}

template<typename T>
Poly<T> Poly<T>::operator+(Poly<T> &rhs) const {
    Poly buffer = Poly(*this);
    
    buffer += rhs;
    
    return buffer;
}

//MARK: Subtraction
template<typename T>
Poly<T> &Poly<T>::operator-=(Poly<T> &rhs) {
    for (const auto &[key, value] : rhs.coefficients_) {
        coefficients_[key] -= value;
    }
    
    return *this;
}
template<typename T>
Poly<T> Poly<T>::operator-(Poly<T> &rhs) const {
    Poly buffer(-rhs);
    
    return *this + buffer;
}


//MARK: Multiplying
template<typename T>
Poly<T> &Poly<T>::operator*=(Poly &rhs) {
    Poly p = *this * rhs;
    coefficients_ = p.coefficients_;
    size_ = p.size_;
    
    return *this;
}
template<typename T>
Poly<T> &Poly<T>::operator*=(T &rhs) {
    for (const auto &[key, _] : coefficients_) {
        coefficients_[key] *= rhs;
    }
    
    return *this;
}

template<typename T>
Poly<T> Poly<T>::operator*(Poly &rhs) const {
    mapType map;

    for (const auto &[fKey, fValue] : coefficients_) {
        for (const auto &[sKey, sValue] : rhs.coefficients_) {
            map[fKey + sKey] += fValue * sValue;
        }
    }

    Poly p{map};
    p.sizeToFit();

    return p;
}
template<typename T>
Poly<T> Poly<T>::operator*(T &rhs) const {
    Poly buffer(coefficients_);
    
    buffer *= rhs;
    
    return buffer;
}

    //MARK: Dividing
template<typename T>
Poly<T> &Poly<T>::operator/=(T &rhs) {
    for (const auto &[key, _] : coefficients_) {
        coefficients_[key] /= rhs;
    }
    
    return *this;
}
template<typename T>
Poly<T> Poly<T>::operator/(T &rhs) const {
    Poly buffer(coefficients_);
    
    buffer /= rhs;
    
    return buffer;
}


//MARK: - Subscript
template<typename T>
T Poly<T>::operator[](size_t index) const {
    if (coefficients_.contains(index)) {
        return coefficients_[index];
    } else {
        return 0;
    }
}

template<typename T>
T &Poly<T>::operator[](size_t index) {
    return coefficients_[index];
}

//MARK: - IO
template<typename T>
std::istream &operator>>(std::istream &in, Poly<T> &p) {
    for (size_t i = 0; i < p.size(); ++i) {
        in >> p[i];
    }
    
    return in;
}

template<typename T>
std::ostream &operator<<(std::ostream &out, Poly<T> &p) {
    bool sthIsOut = false;
    
    for (size_t i = 0; i < p.size(); ++i) {
        if (p.contains(i)) {
            out << str(std::pair<const size_t, T> {i, p[i]}, !sthIsOut) << ' ';
            sthIsOut = true;
        }
    }
    
    return out;
}

#endif
