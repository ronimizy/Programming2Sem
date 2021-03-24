//
// Created by Георгий Круглов on 23.03.2021.
//

#ifndef LAB6_SCENE3D_H
#define LAB6_SCENE3D_H

#include <vector>

#include "Object3D.h"


class Scene3D {
    std::vector<Object3D> objects_;

public:
    Camera3D camera_;
    Light3D light_;

    Scene3D(Camera3D camera, Light3D light);

    template<typename T, typename... Args> void
    addObject(Args &&... args);
};


#endif //LAB6_SCENE3D_H
