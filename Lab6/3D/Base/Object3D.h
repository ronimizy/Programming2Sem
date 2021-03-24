//
// Created by Георгий Круглов on 22.03.2021.
//

#ifndef LAB6_OBJECT3D_H
#define LAB6_OBJECT3D_H

#include <compare>

#include "Scene3D.h"

class Object3D : public sf::Drawable,
                 public sf::Transformable {
protected:
    float zIndex;
    sf::Vector3f center_;
    sf::Vector3f rotation_ = {0, 0, 0};
    const Scene3D &scene_;

public:
    explicit
    Object3D(sf::Vector3f center, const Scene3D &scene)
            : zIndex(center.z), center_(center), scene_(scene) {};


    constexpr auto
    operator<=>(const Object3D &rhs) const {
        return zIndex <=> rhs.zIndex;
    }


    virtual void
    rotate(sf::Vector3f radians) = 0;

    virtual void
    move(sf::Vector3f by) = 0;
};


#endif //LAB6_OBJECT3D_H
