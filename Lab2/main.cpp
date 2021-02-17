#include <iostream>
#include <utility>
#include <vector>
#include <cmath>

template <typename T>
class Poly {
    std::vector<T> coefficients;

public:
    int size() const {
        return coefficients.size();
    }

    T coefficient(unsigned int number) const {
        if (number < size()) {
            return coefficients[number];
        } else {
            return 0;
        }
    }

    void add(int number, T value) {
        if (number < size()) {
            coefficients[number] += value;
        } else {
            coefficients.resize(number + 1);
            coefficients[number] += value;
        }
    }

    T substitute(T x) const {
        T sum = 0;
        T degree = x/x;

        for (auto c : coefficients) {
            sum += c * degree;
            degree *= x;
        }

        return sum;
    }

    Poly(std::vector<T> c) : coefficients(std::move(c)) {};

    Poly(const Poly &origin) : coefficients(origin.coefficients) {};

    Poly() : coefficients(std::vector<T> ()) {};

    Poly(size_t size) :coefficients(std::vector<T>(size)) {}

    ~Poly() {
        coefficients.resize(0);
        std::cout << "Destructor's work have been done" << std::endl;
    }

    bool operator==(const Poly &rhs) const {
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

        return true;
    }
    bool operator!=(const Poly &rhs) const {
        return  !(*this == rhs);
    }

    Poly operator+(Poly &rhs) const {
        Poly buffer;

        if (coefficients.size() >= rhs.size()) {
            buffer = Poly(coefficients);

            for (int i = 0; i < rhs.size(); i++) {
                buffer.add(i, rhs.coefficient(i));
            }
        } else {
            buffer = Poly(rhs.coefficients);

            for (int i = 0; i < coefficients.size(); i++) {
                buffer.add(i, coefficients[i]);
            }
        }

        return buffer;
    }

    Poly operator-() const {
        Poly buffer(coefficients);

        for (int i = 0; i < size(); i++) {
            buffer.coefficients[i] = -coefficients[i];
        }

        return buffer;
    }

    Poly operator-(Poly &rhs) const {
        Poly buffer(-rhs);

        return *this + buffer;
    }

    Poly& operator +=(Poly &rhs) {
        *this = *this + rhs;

        return *this;
    }

    Poly& operator-=(Poly &rhs) {
        *this = *this - rhs;

        return *this;
    }

    Poly operator*(T &rhs) const {
        Poly buffer(coefficients);

        for (int i = 0; i < size(); i++) {
            buffer.coefficients[i] *= rhs;
        }

        return buffer;
    }
    Poly operator/(T &rhs) const {
        Poly buffer(coefficients);

        for (int i = 0; i < size(); i++) {
            buffer.coefficients[i] /= rhs;
        }

        return buffer;
    }

    Poly& operator*=(T &rhs) {
        for (int i = 0; i < size(); i++) {
            coefficients[i] *= rhs;
        }

        return *this;
    }
    Poly& operator/=(T &rhs) {
        for (int i = 0; i < size(); i++) {
            coefficients[i] /= rhs;
        }

        return *this;
    }

    T operator[](int i) const {
        return coefficient(i);
    }

    friend std::istream& operator>>(std::istream &in, Poly<T> &p) {
        for (auto &c : p.coefficients) {
            in >> c;
        }

        return in;
    }

    friend std::ostream& operator<<(std::ostream &out, const Poly<T> &p) {
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
};


int main() {
    Poly<int> pInt(std::vector<int> {-1, 2, 3, -4});
    Poly<int> pInt2(std::vector<int> {-1, 2, 3, -5});
    Poly<double> pD(std::vector<double> {-1, 2, 3, -4});


    std::cout << pInt << '\n';
//    std::cout << "Hello, World!" << std::endl;
    return 0;
}
