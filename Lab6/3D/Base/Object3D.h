//
// Created by Георгий Круглов on 22.03.2021.
//

#ifndef LAB6_OBJECT3D_H
#define LAB6_OBJECT3D_H

#include <compare>
#include <functional>
#include <vector>

#include <SFML/Graphics.hpp>

using Verticies = std::vector<sf::Vector3f >;

class Object3D : public sf::Drawable,
                 public sf::Transformable {
protected:
    float zIndex;
    sf::Vector3f center_;
    sf::Vector3f rotation_ = {0, 0, 0};

public:
    explicit
    Object3D(sf::Vector3f center)
            : zIndex(center.z), center_(center) {};


    constexpr auto
    operator<=>(const Object3D &rhs) const {
        return zIndex <=> rhs.zIndex;
    }


    virtual void
    rotate(sf::Vector3f radians) = 0;

    virtual void
    move(sf::Vector3f by) = 0;

    virtual void
    render(const std::function<sf::Vector2f(sf::Vector3f)> &closure) = 0;
};


#endif //LAB6_OBJECT3D_H
