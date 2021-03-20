//
// Created by Георгий Круглов on 22.02.2021.
//

#include "../../RNGeometry.h"

//Default constructor
RNGeometry::Figures::Trapeze::Trapeze(const std::vector <RNGeometry::Point> &points) : RNGeometry::Figures::ConvexPolygon(points) {
    if (this->points.size() != 4 || !isTrapeze()) {
        throw std::invalid_argument("Given figure is not a Trapeze");
    }
}

//Move constructor
RNGeometry::Figures::Trapeze::Trapeze(Trapeze &&origin) noexcept : ConvexPolygon(origin) {};

//Copy constructor
RNGeometry::Figures::Trapeze::Trapeze(const Trapeze &origin) : ConvexPolygon(origin.points) {};

//Destructor
RNGeometry::Figures::Trapeze::~Trapeze()  {
    std::cout << "Trapeze object had been destructed\n";
}

//Invariant check
bool RNGeometry::Figures::Trapeze::isTrapeze() {
    std::vector<Point> directionals;
    std::vector<std::pair<int, int>> parallels;

    for (int i = 1; i < 4; i++) {
        directionals.push_back(points[i] - points[i - 1]);
    }
    directionals.push_back(points[0] - points[3]);

    for (int i = 0; i < directionals.size() - 1; i++) {
        for (int j = i + 1; j < directionals.size(); j++) {
            if (((directionals[i].x * directionals[j].x + directionals[i].y * directionals[j].y) /
                 directionals[i].distanceFromCenter() * directionals[j].distanceFromCenter()) == 0
                or ((directionals[i].x * directionals[j].x + directionals[i].y * directionals[j].y) /
                    directionals[i].distanceFromCenter() * directionals[j].distanceFromCenter()) == 180) {
                parallels.emplace_back(i, j);
            }
        }
    }

    if (parallels.size() != 1) {
        return false;
    }

    return true;
}