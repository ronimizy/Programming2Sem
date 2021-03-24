//
// Created by Георгий Круглов on 23.03.2021.
//

#include "../Base/Scene3D.h"

Scene3D::Scene3D(Camera3D camera, Light3D light)
        :camera_(camera), light_(light) {};

template <typename T, typename... Args> void
Scene3D::addObject(Args &&... args) {
    objects_.push_back(T(*this, args...));
}