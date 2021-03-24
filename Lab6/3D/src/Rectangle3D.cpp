//
// Created by Георгий Круглов on 22.03.2021.
//

#include "../Rectangle3D.h"

//Constructor
Rectangle3D::Rectangle3D(const Scene3D &scene, sf::Color color, float width, float height, sf::Vector3f center)
        : Object3D(center, scene) {
    shape.setPointCount(4);
    shape.setFillColor(color);

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

    updatePoints(points_);
}

void Rectangle3D::updatePoints(sf::Vector3f points[4]) {
    for (size_t i = 0; i < 4; ++i) {
        shape.setPoint(i, scene_.camera_.render(points[i]));
    }
}


void
Rectangle3D::rotate(sf::Vector3f radians) {
    rotation_ = {fmod(rotation_.x + radians.x, 2 * (float) M_PI),
                 fmod(rotation_.y + radians.y, 2 * (float) M_PI),
                 fmod(rotation_.z + radians.z, 2 * (float) M_PI)};

    sf::Vector3f points[4];
    for (size_t i = 0; i < 4; ++i) {
        points[i] = points_[i];
    }

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

    updatePoints(points);
}

void
Rectangle3D::move(sf::Vector3f by) {
    for (auto &point : points_) {
        point += by;
    }

    rotate(rotation_);
}
