//
// Created by Георгий Круглов on 22.02.2021.
//

#include "../../RNGeometry.h"

//Default constructor
RNGeometry::Figures::Triangle::Triangle(const std::vector <Point> &points) : ConvexPolygon(points) {
    if (points.size() != 3) {
        this->points.resize(0);
    }
}

//Copy constructor
RNGeometry::Figures::Triangle::Triangle(const Triangle &origin)
        : ConvexPolygon(origin.points) {};

//Destructor
RNGeometry::Figures::Triangle::~Triangle() {
    std::cout << "Triangle object had been destructed\n";
}