#include <iostream>
#include <vector>

#include "RNGeometry.h"


int main() {
    RNGeometry::Point p1(1, 0);
    RNGeometry::Point p2(2, 0);

    RNGeometry::Lines::PolygonalLine line({p1, p2, RNGeometry::Point(3, 0)});

    std::cout << line.length() << ' ' << line.vertexCount() << '\n'; // 2 3

    RNGeometry::Figures::ConvexPolygon cp({RNGeometry::Point(),
                                           RNGeometry::Point(1, 0),
                                           RNGeometry::Point(1, 1),
                                           RNGeometry::Point(0, 1)});
    try {
        RNGeometry::Figures::ConvexPolygon np({RNGeometry::Point(),
                                               RNGeometry::Point(1, 0),
                                               RNGeometry::Point(0.5, 0.5),
                                               RNGeometry::Point(1, 1),
                                               RNGeometry::Point(0, 1)});
    } catch (std::exception &e) {
        std::cerr << e.what() << '\n';
    }

    std::cout << cp.area() << ' ' << cp.perimeter() << '\n'; // 1

    RNGeometry::Figures::Triangle cT({RNGeometry::Point(0, 1),
                                      RNGeometry::Point(),
                                      RNGeometry::Point(1, 0)});
    try {
        RNGeometry::Figures::Triangle nT({RNGeometry::Point(0, 1),
                                          RNGeometry::Point()});
    } catch (std::exception &e) {
        std::cerr << e.what() << '\n';
    }


    std::cout << cT.area() << ' ' << cT.perimeter() << '\n'; //0.5 3.41....

    RNGeometry::Figures::Trapeze cTr({RNGeometry::Point(0, 1),
                                      RNGeometry::Point(-1, 0),
                                      RNGeometry::Point(2, 0),
                                      RNGeometry::Point(1, 1)});
    try {
        RNGeometry::Figures::Trapeze nTr({RNGeometry::Point(0, 1),
                                          RNGeometry::Point(),
                                          RNGeometry::Point(1, 0),
                                          RNGeometry::Point(1, 2)});
    } catch (std::exception &e) {
        std::cerr << e.what() << '\n';
    }

    std::cout << cTr.area() << ' ' << cTr.perimeter() << '\n'; //2 6.82...

    RNGeometry::Figures::AppropriatePolygon cAp({RNGeometry::Point(),
                                                 RNGeometry::Point(0, 1),
                                                 RNGeometry::Point(1, 1),
                                                 RNGeometry::Point(1, 0)});
    try {
        RNGeometry::Figures::AppropriatePolygon nAp({RNGeometry::Point(),
                                                     RNGeometry::Point(0, 1),
                                                     RNGeometry::Point(1, 1),
                                                     RNGeometry::Point(0.9, 0)});
    } catch (std::exception &e) {
        std::cerr << e.what() << '\n';
    }

    std::cout << cAp.area() << ' ' << cAp.perimeter() << '\n'; //2 6.82...

    RNGeometry::Figures::Figure *arr[] = {&cp, &cT, &cTr, &cAp};
    std::cout << '\n';
    for (auto &i : arr) {
        std::cout << i->perimeter() << '\n';
    }

    return 0;
}
