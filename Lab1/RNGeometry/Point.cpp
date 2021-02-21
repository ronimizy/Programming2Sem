//
// Created by Георгий Круглов on 21.02.2021.
//

#include "../RNGeometry.h"

#include <cmath>
#include <iostream>

using namespace RNGeometry;

//Default constructor
Point::Point(double x, double y)
        : x(x), y(y) {};

//Copy constructor
Point::Point(const Point &origin)
        : x(origin.x), y(origin.y) {};

//Distance between two points
double Point::distanceFrom(const Point &point) const {
    return sqrt(pow(x - point.x, 2) + pow(y - point.y, 2));
}

//Distance from center
double Point::distanceFromCenter() const {
    return distanceFrom(Point());
}

//Normal vector to point's radius vector
Point Point::normal() const {
    return Point(y, -x);
}

//Operators
//
//
Point Point::operator+(const Point &rhs) const {
    return Point(x + rhs.x, y + rhs.y);
}

Point Point::operator-(const Point &rhs) const {
    return Point(x - rhs.x, y - rhs.y);
}

Point Point::operator-() const {
    return Point(-x, -y);
}

Point &Point::operator+=(const Point &rhs) {
    x += rhs.x;
    y += rhs.y;

    return *this;
}

Point &Point::operator-=(const Point &rhs) {
    x -= rhs.x;
    y -= rhs.y;

    return *this;
}

std::istream &operator>>(std::istream &in, Point &point) {
    return in >> point.x >> point.y;
}

std::ostream &operator<<(std::ostream &out, const Point &point) {
    return out << "x: " << point.x << ", y: " << point.y;
}
