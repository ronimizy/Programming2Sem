//
// Created by Георгий Круглов on 11.05.2021.
//

#ifndef GENETICCUBE_COLOR_H
#define GENETICCUBE_COLOR_H

#include <iostream>

namespace Logic {
    class Color {
        int value;

    public:
        Color(int c = 0) : value(c) {}

        inline friend bool operator==(const Color &lhs, const Color &rhs) { return lhs.value == rhs.value; }

        inline friend bool operator!=(const Color &lhs, const Color &rhs) { return lhs.value != rhs.value; }

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

        inline friend std::ostream &operator<<(std::ostream &out, const Logic::Color &color) { return out << (char) color; }
    };
}

#endif //GENETICCUBE_COLOR_H
