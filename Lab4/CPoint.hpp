//
// Created by Георгий Круглов on 24.02.2021.
//

#ifndef LAB4_CPOINT_HPP
#define LAB4_CPOINT_HPP

#include <cmath>

struct CPoint {
    double x_, y_;

    CPoint(double x, double y) : x_(x), y_(y) {};

    bool operator==(const CPoint &rhs) const {
        return x_ == rhs.x_ && y_ == rhs.y_;
    }

    bool operator!=(const CPoint &rhs) const {
        return !(*this == rhs);
    }

    bool operator>(const CPoint &rhs) const {
        return pow(pow(x_, 2) + pow(y_, 2), 0.5) > pow(pow(rhs.x_, 2) + pow(rhs.y_, 2), 0.5);
    }

    bool operator>=(const CPoint &rhs) const {
        return pow(pow(x_, 2) + pow(y_, 2), 0.5) >= pow(pow(rhs.x_, 2) + pow(rhs.y_, 2), 0.5);
    }

    bool operator<(const CPoint &rhs) const {
        return !(*this >= rhs);
    }

    bool operator<=(const CPoint &rhs) const {
        return !(*this > rhs);
    }
};

#endif //LAB4_CPOINT_HPP
