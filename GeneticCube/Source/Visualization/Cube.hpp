//
// Created by Георгий Круглов on 28.03.2021.
//

#ifndef LAB6_CUBE_H
#define LAB6_CUBE_H

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Skybox.h>
#include <vector>
#include <tuple>


#include "RotationConfiguration.hpp"

namespace Visualization {
    class Cube {
    private:

        std::vector<std::vector<std::vector<Urho3D::Node *>>> cubes;
        Urho3D::Vector3 center_;

        RotationConfiguration configuration;

        const size_t CUBE_SIZE = 3;
        const float SCALE = 5.0f;
        const float SPACING = 0.6f;

        Urho3D::SharedPtr<Urho3D::Material> unselected_;
        Urho3D::SharedPtr<Urho3D::Material> selected_;

        void setSelectedMaterial(const Urho3D::SharedPtr<Urho3D::Material> &material);

        std::tuple<size_t, size_t, size_t> getCoordinates(size_t first, size_t second) const;

        [[nodiscard]]
        std::tuple<size_t, size_t, size_t>
        getRotatedCoordinates(size_t i, size_t j, size_t k) const;
    public:
        Cube(Urho3D::SharedPtr<Urho3D::Scene> &scene, Urho3D::ResourceCache *cache);

        void Rotate(float degree, bool sync);

        void MovePosition(long at);

        void FlipDimension();

        void FlipOrientation();

        void Deselect();

        void Select();

        inline RotationConfiguration &Configuration() { return configuration; }

        Urho3D::Vector3 Center() { return center_; }
    };
}

#endif //LAB6_CUBE_H
