//
// Created by Георгий Круглов on 21.02.2021.
//

#include <utility>

#include "../../RNGeometry.h"


double RNGeometry::Figures::Figure::perimeter() const {
    double sum = 0;

    for (int i = 0; i < points.size(); i++) {
        sum += points[i].distanceFrom(points[(i - 1 + points.size()) % points.size()]);
    }

    return sum;
}


RNGeometry::Figures::Figure::Figure(std::vector <Point> points) : points(std::move(points)) {};
RNGeometry::Figures::Figure::~Figure() {
    std::cout << "Figure object had been destructed\n";
}

std::vector<RNGeometry::Point>::iterator RNGeometry::Figures::Figure::begin() { return points.begin(); }
std::vector<RNGeometry::Point>::iterator RNGeometry::Figures::Figure::end() { return points.end(); }