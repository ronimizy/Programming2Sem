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
        if (coefficients_[i].first_ != rhs.coefficients_[i].first_ ||
            coefficients_[i].second_ != rhs.coefficients_[i].second_) {
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
        it.second_ = -it.second_;
    }
    
    return buffer;
}

//MARK: Sum
template<typename T>
Poly<T> &Poly<T>::operator+=(Poly<T> &rhs) {
    for (auto &it : rhs.coefficients_) {
        this->add(it.first_, it.second_);
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
    for (auto &it : rhs.coefficients_) {
        this->add(it.first_, -it.second_);
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
    std::map<size_t, T> buffer {};
    
    for (auto &left : coefficients_) {
        for (auto &right : rhs.coefficients_) {
            buffer[left.first + right.first] += left.second * right.second;
        }
    }
    
    coefficients_ = buffer;
    size_ += rhs.size_ - 1;
    sizeToFit();
    
    return *this;
}
template<typename T>
Poly<T> &Poly<T>::operator*=(T &rhs) {
    for (auto &node : coefficients_) {
        node.second_ *= rhs;
    }
    
    return *this;
}

template<typename T>
Poly<T> Poly<T>::operator*(Poly &rhs) const {
    Poly buffer(*this);
    
    buffer *= rhs;
    
    return buffer;
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
    for (auto &node : coefficients_) {
        node.second_ /= rhs;
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
std::pair<const size_t, T> Poly<T>::operator[](size_t index) const {
    if (coefficients_.count(index)) {
        return coefficients_.at(index);
    } else {
        return std::pair<const size_t, T> {index, 0};
    }
}

template<typename T>
std::pair<const size_t, T> &Poly<T>::operator[](size_t index) {
    if (!coefficients_.count(index)) {
        coefficients_[index];
    }
    
    return coefficients_.at(index);
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
    for (const std::pair<const size_t, T> &it : p) {
        out << str(it) << ' ';
    }
    
    return out;
}

#endif
