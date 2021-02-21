//
// Created by Георгий Круглов on 21.02.2021.
//

#include "../../RNGeometry.h"

//Default constructor
RNGeometry::Lines::ClosedPolygonalLine::ClosedPolygonalLine(const std::vector <RNGeometry::Point> &points)
        : PolygonalLine(points) {};

//Copy constructor
RNGeometry::Lines::ClosedPolygonalLine::ClosedPolygonalLine(const ClosedPolygonalLine &origin)
        : PolygonalLine(origin.points) {};

//Iterator
//
std::vector<RNGeometry::Point>::iterator RNGeometry::Lines::ClosedPolygonalLine::begin() { return points.begin(); }

std::vector<RNGeometry::Point>::iterator RNGeometry::Lines::ClosedPolygonalLine::end() { return points.end(); }

double RNGeometry::Lines::ClosedPolygonalLine::length() {
    double sum = 0;

    for (int i = 0; i < points.size(); i++) {
        sum += points[i].distanceFrom(points[(i - 1 + points.size()) % points.size()]);
    }

    return sum;
}