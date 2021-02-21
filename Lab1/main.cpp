#include <iostream>
#include <vector>

#include "RNGeometry.h"


int main() {
    RNGeometry::Point p1(1, 0);
    RNGeometry::Point p2(2, 0);

    RNGeometry::Lines::PolygonalLine line(std::vector<RNGeometry::Point>{p1, p2, RNGeometry::Point(3, 0)});

    std::cout << line.length() << ' ' << line.vertexCount() << '\n'; // 2 3

    RNGeometry::Figures::ConvexPolygon cp(std::vector<RNGeometry::Point>{RNGeometry::Point(),
                                                                         RNGeometry::Point(1, 0),
                                                                         RNGeometry::Point(1, 1),
                                                                         RNGeometry::Point(0, 1)});
    RNGeometry::Figures::ConvexPolygon np(std::vector<RNGeometry::Point>{RNGeometry::Point(),
                                                                         RNGeometry::Point(1, 0),
                                                                         RNGeometry::Point(0.5, 0.5),
                                                                         RNGeometry::Point(1, 1),
                                                                         RNGeometry::Point(0, 1)});

    std::cout << cp.area() << ' ' << cp.perimeter() << '\n'; // 1
    std::cout << np.area() << '\n'; // 0

    RNGeometry::Figures::Triangle cT(std::vector<RNGeometry::Point>{RNGeometry::Point(0, 1),
                                                                    RNGeometry::Point(),
                                                                    RNGeometry::Point(1, 0)});
    RNGeometry::Figures::Triangle nT(std::vector<RNGeometry::Point>{RNGeometry::Point(0, 1),
                                                                    RNGeometry::Point()});

    std::cout << cT.area() << ' ' << cT.perimeter() << '\n'; //0.5 3.41....
    std::cout << nT.area() << ' ' << nT.perimeter() << '\n'; // 0 0

    RNGeometry::Figures::Trapeze cTr(std::vector<RNGeometry::Point>{RNGeometry::Point(0, 1),
                                                                    RNGeometry::Point(-1, 0),
                                                                    RNGeometry::Point(2, 0),
                                                                    RNGeometry::Point(1, 1)});
    RNGeometry::Figures::Trapeze nTr(std::vector<RNGeometry::Point>{RNGeometry::Point(0, 1),
                                                                    RNGeometry::Point(),
                                                                    RNGeometry::Point(1, 0),
                                                                    RNGeometry::Point(1, 2)});

    std::cout << cTr.area() << ' ' << cTr.perimeter() << '\n'; //2 6.82...
    std::cout << nTr.area() << ' ' << nTr.perimeter() << '\n'; // 0 0

    RNGeometry::Figures::AppropriatePolygon cAp(std::vector<RNGeometry::Point>{RNGeometry::Point(),
                                                                               RNGeometry::Point(0, 1),
                                                                               RNGeometry::Point(1, 1),
                                                                               RNGeometry::Point(1, 0)});
    RNGeometry::Figures::AppropriatePolygon nAp(std::vector<RNGeometry::Point>{RNGeometry::Point(),
                                                                               RNGeometry::Point(0, 1),
                                                                               RNGeometry::Point(1, 1),
                                                                               RNGeometry::Point(0.9, 0)});

    std::cout << cAp.area() << ' ' << cAp.perimeter() << '\n'; //2 6.82...
    std::cout << nAp.area() << ' ' << nAp.perimeter() << '\n'; // 0 0

    RNGeometry::Figures::Figure* arr[] = { &cp, &cT, &cTr, &cAp };
    std::cout << '\n';
    for (auto & i : arr) {
        std::cout << i->perimeter() << '\n';
    }

    return 0;
}
