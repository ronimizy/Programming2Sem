//
// Created by Георгий Круглов on 22.02.2021.
//

#include "../../RNGeometry.h"

//Default constructor
RNGeometry::Figures::AppropriatePolygon::AppropriatePolygon(const std::vector <Point> &points) : ConvexPolygon(points) {
    if (this->points.size() <= 2 || !isAppropriate()) {
        this->points.resize(0);
    }
}

//Copy constructor
RNGeometry::Figures::AppropriatePolygon::AppropriatePolygon(const AppropriatePolygon &origin)  : ConvexPolygon(origin.points) {}

//Destructor
RNGeometry::Figures::AppropriatePolygon::~AppropriatePolygon() {
    std::cout << "AppropriatePolygon object had been destructed\n";
}

//Invariant check
bool RNGeometry::Figures::AppropriatePolygon::isAppropriate() {
    double length = points[0].distanceFrom(points[1]);

    for (int i = 2; i < points.size(); i++) {
        if (points[i].distanceFrom(points[i - 1]) != length) {
            return false;
        }
    }

    double cos = ((double) points.size() - 2) * 180 / points.size();

    for (int i = 1; i < points.size() - 1; i++) {
        Point v1 = points[i - 1] - points[i], v2 = points[i + 1] - points[i];

        if (fmod(((v1.x * v2.x + v1.y * v2.y) / v1.distanceFromCenter() * v2.distanceFromCenter()), cos) != 0) {
            return false;
        }
    }
    Point v1 = points[points.size() - 1] - points[0], v2 = points[1] - points[0];

    if (fmod(((v1.x * v2.x + v1.y * v2.y) / v1.distanceFromCenter() * v2.distanceFromCenter()), cos) != 0) {
        return false;
    }

    return true;
}