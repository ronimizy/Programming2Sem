//
// Created by Георгий Круглов on 26.05.2021.
//

#ifndef GENETICCUBE_BOX_HPP
#define GENETICCUBE_BOX_HPP

#include "Urho3DAll.hpp"
#include "../Logic/Color.hpp"
#include "RotationConfiguration.hpp"

#include <vector>

namespace Visualization {
    class Box {
        static const int faceCount = 6;

        Urho3D::SharedPtr<Urho3D::Node> root;
        std::vector<Urho3D::SharedPtr<Urho3D::Node >> faces;

        float scale_ = 5.0f;

        std::vector<std::pair<Urho3D::Material *, Urho3D::Material *>> materials;
        std::vector<Logic::Color> colors;
        bool selected = false;

    public:
        Box(Urho3D::ResourceCache *cache, Urho3D::Scene *scene);

        void SetPosition(const Urho3D::Vector3 &position) { root->SetPosition(position); }

        void SetScale(float scale) { root->Scale(scale); }

        void SetColors(const std::vector<Logic::Color> &);

        std::vector<Logic::Color> &GetColors() { return colors; }

        void Select();

        void Deselect();

        void RotateAround(const Urho3D::Vector3 &, const Urho3D::Quaternion &, const Urho3D::TransformSpace &);

        void Sync(const RotationConfiguration &);
    };
}


#endif //GENETICCUBE_BOX_HPP
