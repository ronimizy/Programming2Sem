//
// Created by Георгий Круглов on 21.02.2021.
//

#include "../../RNGeometry.h"


//Default constructor
RNGeometry::Lines::PolygonalLine::PolygonalLine(std::vector <RNGeometry::Point> points)
        : points(std::move(points)) {};

//Copy constructor
RNGeometry::Lines::PolygonalLine::PolygonalLine(const PolygonalLine &origin)
        : points(origin.points) {};

//Iterator
std::vector<RNGeometry::Point>::iterator RNGeometry::Lines::PolygonalLine::begin() { return points.begin(); }

std::vector<RNGeometry::Point>::iterator RNGeometry::Lines::PolygonalLine::end() { return points.end(); }

//Length of the line
double RNGeometry::Lines::PolygonalLine::length() {
    double sum = 0;

    for (int i = 1; i < points.size(); i++) {
        sum += points[i].distanceFrom(points[i - 1]);
    }

    return sum;
}

//
long RNGeometry::Lines::PolygonalLine::vertexCount() const {
    return points.size();
}

//Operators
//
//
RNGeometry::Point &RNGeometry::Lines::PolygonalLine::operator[](const long &index) {
    return points[index];
}

const RNGeometry::Point &RNGeometry::Lines::PolygonalLine::operator[](const long &index) const {
    return points[index];
}

RNGeometry::Lines::PolygonalLine &RNGeometry::Lines::PolygonalLine::operator+=(const RNGeometry::Point &point) {
    points.push_back(point);

    return *this;
}

RNGeometry::Lines::PolygonalLine RNGeometry::Lines::PolygonalLine::operator+(const RNGeometry::Point &point) {
    PolygonalLine p(*this);

    return p += point;
}

std::ostream &operator<<(std::ostream &out, RNGeometry::Lines::PolygonalLine &line) {
    for (auto &point : line) {
        out << point << '\n';
    }

    return out;
}
