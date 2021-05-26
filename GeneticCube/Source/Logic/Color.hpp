//
// Created by Георгий Круглов on 11.05.2021.
//

#ifndef GENETICCUBE_COLOR_HPP
#define GENETICCUBE_COLOR_HPP

#include <iostream>

//@no-format
namespace Logic {
    class Color {
        int value;

    public:
        Color(int c = 0) : value(c) {}

        Color(const char &c) {
            switch (c) {
                case 'W':
                    value = 0;
                    break;
                case 'G':
                    value = 1;
                    break;
                case 'Y':
                    value = 2;
                    break;
                case 'B':
                    value = 3;
                    break;
                case 'O':
                    value = 4;
                    break;
                case 'R':
                    value = 5;
                    break;
                default:
                    throw std::invalid_argument("Invalid color code");
            }
        }

        inline friend bool operator==(const Color &lhs, const Color &rhs) { return lhs.value == rhs.value; }

        inline friend bool operator!=(const Color &lhs, const Color &rhs) { return lhs.value != rhs.value; }

        inline friend bool operator<(const Color &lhs, const int &rhs) { return lhs.value < rhs; }

        inline Color& operator++(int) { ++value; return *this; }

        inline Color operator++() { Color c(value); ++value; return c; }

        inline operator int() const { return value; }

        operator char() const {
            char v;
            switch (value) {
                case 0:
                    v = 'W';
                    break;
                case 1:
                    v = 'G';
                    break;
                case 2:
                    v = 'Y';
                    break;
                case 3:
                    v = 'B';
                    break;
                case 4:
                    v = 'O';
                    break;
                case 5:
                    v = 'R';
                    break;
                default:
                    throw std::invalid_argument("not a valid color");
            }

            return v;
        }

        inline friend std::ostream &operator<<(std::ostream &out, const Logic::Color &color) {
            return out << (char) color;
        }
    };
}
//@format

#endif //GENETICCUBE_COLOR_HPP
