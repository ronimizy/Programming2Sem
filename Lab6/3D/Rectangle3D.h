//
// Created by Георгий Круглов on 22.03.2021.
//

#ifndef LAB6_RECTANGLE3D_H
#define LAB6_RECTANGLE3D_H

#include "Base/Object3D.h"

class Rectangle3D : public Object3D {
    Verticies points_;

    sf::ConvexShape shape;

public:
    explicit
    Rectangle3D(sf::Color color = {255, 255, 255},
                float width = 100, float height = 100,
                sf::Vector3f center = {0, 0, 0});

    void
    rotate(sf::Vector3f radians) override;

    void
    move(sf::Vector3f by) override;

    void
    render(const std::function<sf::Vector2f (sf::Vector3f)> &closure) override;

    void
    draw(sf::RenderTarget &target, sf::RenderStates states) const override { target.draw(shape, states); }
};


#endif //LAB6_RECTANGLE3D_H
