//
// Created by Георгий Круглов on 21.02.2021.
//

#include "../RNGeometry.h"

#include <cmath>
#include <iostream>

//Default constructor
RNGeometry::Point::Point(double x, double y)
        : x(x), y(y) {};

//Copy constructor
RNGeometry::Point::Point(const Point &origin)
        : x(origin.x), y(origin.y) {};

//Distance between two points
double RNGeometry::Point::distanceFrom(const Point &point) const {
    return sqrt(pow(x - point.x, 2) + pow(y - point.y, 2));
}

//Distance from center
double RNGeometry::Point::distanceFromCenter() const {
    return distanceFrom(Point());
}

//Normal vector to point's radius vector
RNGeometry::Point RNGeometry::Point::normal() const {
    return Point(y, -x);
}

//Operators
//
//
RNGeometry::Point RNGeometry::Point::operator+(const Point &rhs) const {
    return Point(x + rhs.x, y + rhs.y);
}

RNGeometry::Point RNGeometry::Point::operator-(const Point &rhs) const {
    return Point(x - rhs.x, y - rhs.y);
}

RNGeometry::Point RNGeometry::Point::operator-() const {
    return Point(-x, -y);
}

RNGeometry::Point &RNGeometry::Point::operator+=(const Point &rhs) {
    x += rhs.x;
    y += rhs.y;

    return *this;
}

RNGeometry::Point &RNGeometry::Point::operator-=(const Point &rhs) {
    x -= rhs.x;
    y -= rhs.y;

    return *this;
}

std::istream &operator>>(std::istream &in, RNGeometry::Point &point) {
    return in >> point.x >> point.y;
}

std::ostream &operator<<(std::ostream &out, const RNGeometry::Point &point) {
    return out << "x: " << point.x << ", y: " << point.y;
}
