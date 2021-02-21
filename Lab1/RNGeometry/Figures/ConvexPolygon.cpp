//
// Created by Георгий Круглов on 21.02.2021.
//

#include "../../RNGeometry.h"

//Default constructor
RNGeometry::Figures::ConvexPolygon::ConvexPolygon(const std::vector <Point> &points) : Figure(points) {
    if (!isConvex()) {
        this->points.resize(0);
    }
}

//Copy constructor
RNGeometry::Figures::ConvexPolygon::ConvexPolygon(const ConvexPolygon &origin)
        : Figure(origin.points) {};

//Destructor
RNGeometry::Figures::ConvexPolygon::~ConvexPolygon() {
    std::cout << "ConvexPolygon object had been destructed\n";
}

double RNGeometry::Figures::ConvexPolygon::area() {
    double sum = 0;

    for (int i = 0; i < points.size(); i++) {
        sum += points[i].x * (points[(i + 1 + points.size()) % points.size()].y -
                              points[(i - 1 + points.size()) % points.size()].y);
    }

    return abs(sum) / 2;
}

//Invariant check
bool RNGeometry::Figures::ConvexPolygon::isConvex() {
    std::vector <std::pair<Point, Point>> edges;
    std::vector <Point> normals;

    edges.resize(0);
    normals.resize(0);

    for (int i = 1; i < points.size(); i++) {
        edges.emplace_back(points[i - 1], points[i]);
    }
    edges.emplace_back(points[points.size() - 1], points[0]);

    for (int i = 0; i < points.size(); i++) {
        normals.push_back((edges[i].second - edges[i].first).normal());
    }

    for (int i = 0; i < points.size(); i++) {
        double A = normals[i].x, B = normals[i].y, C = A * edges[i].first.x + B * edges[i].first.y;

        bool direction;

        for (auto &point : points) {
            if (A * point.x + B * point.y - C != 0) {
                direction = A * point.x + B * point.y - C > 0;
            }
        }

        for (auto &point : points) {
            if (A * point.x + B * point.y - C != 0 && (A * point.x + B * point.y - C > 0) != direction) {
                return false;
            }
        }
    }

    return true;
}