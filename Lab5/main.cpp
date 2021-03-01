#include <iostream>
#include "CircularBuffer.hpp"

int main() {

    CircularBuffer<int> intBuffer(10);
    CircularBuffer<int> b(0);

    for (int i = 0; i < 20; ++i) {
        intBuffer.push_back(i);
    }

    for (int i = 0; i < 10; ++i) {
        std::cout << intBuffer.pop_back() << ' ';
    }

    intBuffer.push_back(66);

    std::cout << '\n';

    std::cout << intBuffer.front() << ' ' << intBuffer.back() << ' ' << intBuffer[5] << ' ' << intBuffer[39];
    std::cout << '\n';
    std::cout << "\n\t";

    intBuffer.resize(0);
    intBuffer.resize(40);
    for (int i = 0; i < 40; ++i) {
        intBuffer.push_back(i);
    }

    for (auto &it : intBuffer) {
        std::cout << it << ' ';
    }

    std::cout << "\n\n\t";

    for (int i = 0; i < 40; ++i) {
        std::cout << intBuffer.pop_back() << ' ';
    }


    return 0;
}
