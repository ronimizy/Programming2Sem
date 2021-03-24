//
// Created by Георгий Круглов on 23.03.2021.
//

#ifndef LAB6_LIGHT3D_H
#define LAB6_LIGHT3D_H


#include "../SFML3D.h"

class Light3D {
    sf::Vector3f position_;
    sf::Vector3f direction_;

public:
    Light3D(sf::Vector3f position, sf::Vector3f direction)
            : position_(position), direction_(direction) {};
};


#endif //LAB6_LIGHT3D_H
