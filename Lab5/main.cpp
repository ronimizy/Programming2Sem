#include <iostream>
#include "CircularBuffer.hpp"

int main() {
    CircularBuffer<int> intBuffer(10);
    CircularBuffer<int> b(0);

    for (int i = 0; i < 20; ++i) {
        intBuffer.append(i);
    }

    for (int i = 0; i < 10; ++i) {
        std::cout << *intBuffer.pop() << ' ';
    }

    intBuffer.append(66);

    std::cout << '\n';

    std::cout << *intBuffer.front() << ' ' << *intBuffer.back() << ' ' << *intBuffer[5] << ' ' << *intBuffer[39];
    std::cout << '\n';
    std::cout << "\n\t";

    intBuffer.resize(40);
    for (int i = 0; i < 40; ++i) {
        intBuffer.append(i);
    }

    for (int i = 0; i < 40; ++i) {
        std::cout << *intBuffer.pop() << ' ';
        if (i != 0 && i % 10 == 0) { std::cout << "\n\t"; }
    }


    return 0;
}
