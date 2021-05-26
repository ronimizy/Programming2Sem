//
// Created by Георгий Круглов on 11.05.2021.
//

#ifndef GENETICCUBE_COLOR_HPP
#define GENETICCUBE_COLOR_HPP

#include <iostream>
#include <compare>
#include <vector>

//@no-format
namespace Logic {
    class Color {
        int value;

    public:
        Color(int v) : value(v) {};

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

        inline Color& operator++() { ++value; return *this; }

        inline Color operator++(int) { Color c(value); ++value; return c; }

        inline operator int() const { return value; }

        operator char() const {
            static const std::vector<char> array {'W', 'G', 'Y', 'B', 'O', 'R'};

            try {
                return array.at(value);
            } catch (std::exception &) {
                throw std::invalid_argument("Invalid color was given");
            }
        }

        inline friend std::ostream &operator<<(std::ostream &out, const Logic::Color &color) {
            return out << (char) color;
        }
    };
}
//@format

#endif //GENETICCUBE_COLOR_HPP
