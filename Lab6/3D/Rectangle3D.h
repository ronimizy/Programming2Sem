//
// Created by Георгий Круглов on 22.03.2021.
//

#ifndef LAB6_RECTANGLE3D_H
#define LAB6_RECTANGLE3D_H

#include "SFML3D.h"

class Rectangle3D : public Object3D {
    sf::Vector3f points_[4];

    sf::ConvexShape shape;

    void
    updatePoints(sf::Vector3f points[4]);

public:
    explicit
    Rectangle3D(const Scene3D &scene,
                sf::Color color = {255, 255, 255},
                float width = 100, float height = 100,
                sf::Vector3f center = {0, 0, 0});

    void
    rotate(sf::Vector3f radians) override;

    void
    move(sf::Vector3f by) override;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override { target.draw(shape, states); }
};


#endif //LAB6_RECTANGLE3D_H
