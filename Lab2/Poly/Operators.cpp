//
// Created by Георгий Круглов on 22.02.2021.
//

#include "../Poly.h"

template<typename T>
bool Poly<T>::operator==(const Poly<T> &rhs) const {
    for (int i = 0; i < coefficients.size(); i++) {
        if (i < rhs.size()) {
            if (coefficients[i] != rhs.coefficients[i]) {
                return false;
            }
        } else {
            if (coefficients[i] != 0) {
                return false;
            }
        }
    }
    if (coefficients.size() < rhs.size()) {
        for (int i = coefficients.size(); i < rhs.size(); i++) {
            if (rhs[i] != 0) {
                return false;
            }
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

    for (int i = 0; i < size(); i++) {
        buffer.coefficients[i] = -coefficients[i];
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
    for (size_t i = 0; i < rhs.size(); i++) {
        this->add(i, rhs[i]);
    }

    return *this;
}

template<typename T>
Poly<T> &Poly<T>::operator-=(Poly<T> &rhs) {
    for (size_t i = 0; i < rhs.size(); i++) {
        this->add(i, -rhs[i]);
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

    for (int i = 0; i < size(); i++) {
        buffer.coefficients[i] *= rhs;
    }

    return buffer;
}

template<typename T>
Poly<T> Poly<T>::operator/(T &rhs) const {
    Poly buffer(coefficients);

    for (int i = 0; i < size(); i++) {
        buffer.coefficients[i] /= rhs;
    }

    return buffer;
}

template<typename T>
Poly<T> &Poly<T>::operator*=(Poly &rhs) {
    std::vector <T> buffer(size() + rhs.size() - 1);

    for (int i = size() - 1; i >= 0; i--) {
        for (int j = rhs.size() - 1; j >= 0; j--) {
            buffer[i + j] += coefficients[i] * coefficients[j];
        }
    }

    coefficients = buffer;
    return *this;
}

template<typename T>
Poly<T> &Poly<T>::operator*=(T &rhs) {
    for (int i = 0; i < size(); i++) {
        coefficients[i] *= rhs;
    }

    return *this;
}

template<typename T>
Poly<T> &Poly<T>::operator/=(T &rhs) {
    for (int i = 0; i < size(); i++) {
        coefficients[i] /= rhs;
    }

    return *this;
}

template<typename T>
T Poly<T>::operator[](int i) const {
    return coefficient(i);
}

template<typename T>
T& Poly<T>::operator[](int i) {
    return coefficient(i);
}

template<typename T>
std::istream &operator>>(std::istream &in, Poly<T> &p) {
    for (auto &c : p.coefficients) {
        in >> c;
    }

    return in;
}

template<typename T>
std::ostream &operator<<(std::ostream &out, const Poly<T> &p)  {
    for (int i = 0; i < p.size(); i++) {
        if (p[i] < 0) {
            if (i != 0) {
                out << "- ";
            } else {
                out << "-";
            }
        } else {
            if (i != 0) {
                out << "+ ";
            }
        }

        out << abs(p[i]);
        if (i > 0) {
            out << 'x';
            if (i > 1) {
                out << '^' << i;
            }
        }

        out << ' ';
    }

    return out;
}