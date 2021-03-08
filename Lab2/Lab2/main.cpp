#include <iostream>
#include <vector>

#include "Poly.cpp"


int main() {
    Poly<int> pInt;
    Poly<int> pInt2({-1, 2, 3, -5});
    Poly<double> pD({-1, 2, 3, -4});

    Poly<int> ol1({1, 2, 3});
    Poly<int> ol2({1, 2});
    Poly<int> ol3 = ol1 * ol2;

    std::cout << ol3 << '\n';
    return 0;
}
