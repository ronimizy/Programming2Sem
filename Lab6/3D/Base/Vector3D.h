//
// Created by Георгий Круглов on 24.03.2021.
//

#ifndef LAB6_VECTOR3D_H
#define LAB6_VECTOR3D_H

#include <SFML/Graphics.hpp>

sf::Vector3f operator*(const sf::Vector3f &lhs, const sf::Vector3f &rhs) {
    return {lhs.y * rhs.z - rhs.y * lhs.z,
            -(lhs.x * rhs.z - rhs.x * lhs.z),
            lhs.x * rhs.y - rhs.x * lhs.y};
}

float operator/(const sf::Vector3f &lhs, const sf::Vector3f &rhs) {
    bool a = lhs < rhs;
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

float operator()(const sf::Vector3f &uhs) {
    return (float) sqrt(pow(uhs.x, 2) + pow(uhs.y, 2) + pow(uhs.z, 2));
}



#endif //LAB6_VECTOR3D_H
