//
// Created by Георгий Круглов on 22.03.2021.
//

#ifndef LAB6_CAMERA3D_H
#define LAB6_CAMERA3D_H

class Camera3D {
    float distance_;

public:
    explicit
    Camera3D(float distance)
            : distance_(distance) {};

    [[nodiscard]] sf::Vector2f
    render(sf::Vector3f point) const {
        return sf::Vector2f ((distance_ * point.x) / (point.z ? point.z : 1.0f),
                             (distance_ * point.y) / (point.z ? point.z : 1.0f));
    }
};


#endif //LAB6_CAMERA3D_H
