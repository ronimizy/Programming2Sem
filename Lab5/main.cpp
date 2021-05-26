#include <iostream>
#include "CircularBuffer.hpp"

int main() {

    CircularBuffer<int> intBuffer;
    CircularBuffer<int> b;

    for (int i = 20; i > 0; --i) {
        intBuffer.push_back(i);
    }

    std::sort(intBuffer.begin(), intBuffer.end(), std::less());

    for (int i = 0; i < 20; ++i) {
        std::cout << intBuffer.pop_back() << ' ';
    }

//    intBuffer.push_back(66);
//
//    std::cout << '\n';
//
//    std::cout << intBuffer.front() << ' ' << intBuffer.back() << ' ' << intBuffer[5] << ' ' << intBuffer[39];
//    std::cout << '\n';
//    std::cout << "\n\t";
//
//    intBuffer.resize(0);
//    for (int i = 0; i < 40; ++i) {
//        intBuffer.push_back(i);
//    }
//
//    for (auto &it : intBuffer) {
//        std::cout << it << ' ';
//    }
//
//    std::cout << "\n\n\t";
//
//    for (int i = 0; i < 40; ++i) {
//        std::cout << intBuffer.pop_back() << ' ';
//    }


    return 0;
}
