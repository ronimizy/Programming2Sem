#include <iostream>
#include <vector>

#include "Poly.h"


int main() {
    Poly<int> pInt(std::vector < int > {-1, 2, 3, -4});
    Poly<int> pInt2(std::vector < int > {-1, 2, 3, -5});
    Poly<double> pD(std::vector < double > {-1, 2, 3, -4});

    Poly<int> ol1(std::vector < int > {1, 2, 3});
    Poly<int> ol2(std::vector < int > {1, 2});

    std::cout << ol1 * ol2 << '\n';
    return 0;
}
