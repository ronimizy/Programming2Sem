//
// Created by Георгий Круглов on 22.03.2021.
//

#include "../Rectangle3D.h"

//Constructor
Rectangle3D::Rectangle3D(sf::Color color, float width, float height, sf::Vector3f center)
        : Object3D(center) {
    shape.setPointCount(4);
    shape.setFillColor(color);

    points_.resize(4);

    points_[0] = {center.x - width * 0.5f,
                  center.y - height * 0.5f,
                  center.z};
    points_[1] = {center.x + width * 0.5f,
                  center.y - height * 0.5f,
                  center.z};

    points_[2] = {center.x + width * 0.5f,
                  center.y + height * 0.5f,
                  center.z};
    points_[3] = {center.x - width * 0.5f,
                  center.y + height * 0.5f,
                  center.z};
}

void
Rectangle3D::render(const std::function<sf::Vector2f(sf::Vector3f)> &closure) {
    Verticies points(points_);

    for (auto &point : points) {
        point -= center_;

        point = {point.x,
                 point.y * cos(rotation_.x) + point.z * sin(rotation_.x),
                 -point.y * sin(rotation_.x) + point.z * cos(rotation_.x)};

        point = {point.x * cos(rotation_.y) - point.z * sin(rotation_.y),
                 point.y,
                 point.x * sin(rotation_.y) + point.z * cos(rotation_.y)};

        point = {point.x * cos(rotation_.z) + point.y * sin(rotation_.z),
                 -point.x * sin(rotation_.z) + point.y * cos(rotation_.z),
                 point.z};

        point += center_;
    }

    for (size_t i = 0; i < 4; ++i) {
        shape.setPoint(i, closure(points[i]));
    }
}

void
Rectangle3D::rotate(sf::Vector3f radians) {
    rotation_ = {fmod(rotation_.x + radians.x, 2 * (float) M_PI),
                 fmod(rotation_.y + radians.y, 2 * (float) M_PI),
                 fmod(rotation_.z + radians.z, 2 * (float) M_PI)};
}

void
Rectangle3D::move(sf::Vector3f by) {
    for (auto &point : points_) {
        point += by;
    }

    rotate(rotation_);
}
