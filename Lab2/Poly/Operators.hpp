//
// Created by Георгий Круглов on 22.02.2021.
//

#ifndef POLY_OPERATORS_HPP
#define POLY_OPERATORS_HPP

#include "../Poly.h"

template<typename T>
bool Poly<T>::operator==(const Poly<T> &rhs) const {
    if (size() != rhs.size()) {
        return false;
    }

    for (size_t i = 0; i < size(); ++i) {
        if (coefficients[i].first_ != rhs.coefficients[i].first_ ||
            coefficients[i].second_ != rhs.coefficients[i].second_) {
            return false;
        }
    }

    return true;
}

template<typename T>
bool Poly<T>::operator!=(const Poly<T> &rhs) const {
    return !(*this == rhs);
}

template<typename T>
Poly<T> Poly<T>::operator+(Poly<T> &rhs) const {
    Poly buffer = Poly(*this);

    buffer += rhs;

    return buffer;
}

template<typename T>
Poly<T> Poly<T>::operator-() const {
    Poly buffer(*this);

    for (Node<T> &node : buffer.coefficients) {
        node.second_ = -node.second_;
    }

    return buffer;
}

template<typename T>
Poly<T> Poly<T>::operator-(Poly<T> &rhs) const {
    Poly buffer(-rhs);

    return *this + buffer;
}

template<typename T>
Poly<T> &Poly<T>::operator+=(Poly<T> &rhs) {
    for (Node<T> &node : rhs.coefficients) {
        this->add(node.first_, node.second_);
    }

    return *this;
}

template<typename T>
Poly<T> &Poly<T>::operator-=(Poly<T> &rhs) {
    for (Node<T> &node : rhs.coefficients) {
        this->add(node.first_, -node.second_);
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
    Poly buffer(coefficients);

    buffer *= rhs;

    return buffer;
}

template<typename T>
Poly<T> Poly<T>::operator/(T &rhs) const {
    Poly buffer(coefficients);

    buffer /= rhs;

    return buffer;
}

template<typename T>
Poly<T> &Poly<T>::operator*=(Poly &rhs) {
    std::vector<Node<T>> buffer;
    for (size_t i = 0; i < size() + rhs.size() - 1; i ++) {
        buffer.emplace_back(i, 0);
    }

    for (long i = coefficients.size() - 1; i >= 0; --i) {
        for (long j = rhs.coefficients.size() - 1; j >= 0; --j) {
            buffer[coefficients[i].first_ + rhs.coefficients[j].first_].second_ +=
                    coefficients[i].second_ * rhs.coefficients[j].second_;
        }
    }

    coefficients = buffer;
    sizeToFit();

    return *this;
}

template<typename T>
Poly<T> &Poly<T>::operator*=(T &rhs) {
    for (Node<T> &node : coefficients) {
        node.second_ *= rhs;
    }

    return *this;
}

template<typename T>
Poly<T> &Poly<T>::operator/=(T &rhs) {
    for (Node<T> &node : coefficients) {
        node.second_ /= rhs;
    }

    return *this;
}

template<typename T>
T Poly<T>::operator[](size_t index) const {
    size_t i = get(index);

    if (coefficients[i].first_ == index) {
        return coefficients[i].second_;
    }

    return 0;
}

template<typename T>
T &Poly<T>::operator[](size_t index) {
    size_t i = get(index);

    if (coefficients[i].first_ == index) {
        return coefficients[i].second_;
    }

    return add(index, 0);
}

template<typename T>
std::istream &operator>>(std::istream &in, Poly<T> &p) {
    for (auto &c : p.coefficients) {
        in >> c;
    }

    return in;
}

template<typename T>
std::ostream &operator<<(std::ostream &out, const Poly<T> &p) {
    for (const Node<T> &node : p.coefficients) {
        if (node.second_ < 0) {
            if (node.first_ != 0) {
                out << "- ";
            } else {
                out << "-";
            }
        } else {
            if (node.first_ != 0) {
                out << "+ ";
            }
        }

        out << abs(node.second_);
        if (node.first_ > 0) {
            out << 'x';
            if (node.first_ > 1) {
                out << '^' << node.first_;
            }
        }

        out << ' ';
    }

    return out;
}

#endif