//
// Created by Георгий Круглов on 26.05.2021.
//

#include "../Box.hpp"

#include <queue>

using namespace Visualization;

Box::Box(Urho3D::ResourceCache *cache, Urho3D::Scene *scene) {
    materials = {
            {cache->GetResource<Urho3D::Material>("Rubik/Materials/White.xml"),
                    cache->GetResource<Urho3D::Material>("Rubik/Materials/WhiteSelected.xml")},
            {cache->GetResource<Urho3D::Material>("Rubik/Materials/Green.xml"),
                    cache->GetResource<Urho3D::Material>("Rubik/Materials/GreenSelected.xml")},
            {cache->GetResource<Urho3D::Material>("Rubik/Materials/Yellow.xml"),
                    cache->GetResource<Urho3D::Material>("Rubik/Materials/YellowSelected.xml")},
            {cache->GetResource<Urho3D::Material>("Rubik/Materials/Blue.xml"),
                    cache->GetResource<Urho3D::Material>("Rubik/Materials/BlueSelected.xml")},
            {cache->GetResource<Urho3D::Material>("Rubik/Materials/Orange.xml"),
                    cache->GetResource<Urho3D::Material>("Rubik/Materials/OrangeSelected.xml")},
            {cache->GetResource<Urho3D::Material>("Rubik/Materials/Red.xml"),
                    cache->GetResource<Urho3D::Material>("Rubik/Materials/RedSelected.xml")}
    };

    static const std::vector<std::vector<Urho3D::Quaternion>> rotations {
            {{}},
            {{-90, {1, 0, 0}}},
            {{180, {1, 0, 0}}},
            {{90,  {1, 0, 0}}},
            {{90,  {0, 0, 1}}},
            {{-90, {0, 0, 1}}}
    };
    static const std::vector<Urho3D::Vector3> positions {
            {0,    0.5,  0},
            {0,    0,    -0.5},
            {0,    -0.5, 0},
            {0,    0,    0.5},
            {-0.5, 0,    0},
            {0.5,  0,    0}
    };

    root = scene->CreateChild("BoxRoot");

    faces.resize(faceCount);

    for (int i = 0; i < faceCount; ++i) {
        faces[i] = root->CreateChild("Plane");
        faces[i]->SetScale(1);
        faces[i]->SetPosition(positions[i]);
        for (const auto &rotation : rotations[i])
            faces[i]->Rotate(rotation);
        auto *model = faces[i]->CreateComponent<Urho3D::StaticModel>();
        model->SetModel(cache->GetResource<Urho3D::Model>("Models/Plane.mdl"));
        model->SetCastShadows(true);
    }

    SetColors({0, 1, 2, 3, 4, 5});
}

void Box::SetColors(const std::vector<Logic::Color> &colors_) {
    colors = colors_;

    for (int i = 0; i < faceCount; ++i) {
        faces[i]->GetComponent<Urho3D::StaticModel>()->SetMaterial(
                selected ? materials[(int) colors[i]].second : materials[(int) colors[i]].first);
    }
}

void Box::Select() {
    selected = true;
    SetColors(colors);
}

void Box::Deselect() {
    selected = false;
    SetColors(colors);
}

void
Box::RotateAround(const Urho3D::Vector3 &point, const Urho3D::Quaternion &delta, const Urho3D::TransformSpace &space) {
    root->RotateAround(point, delta, space);
}

void Box::Sync(const RotationConfiguration &configuration) {
    //Vertical Front - 0
    //Top to Back - 0
    //Top to Front - 1

    //Vertical Side - 1
    //Top to Left - 0
    //Top to Right - 1

    //Horizontal - 2
    //Front to Left - 0
    //Front to Right - 1

    static const std::vector<int> begins {0, 0, 1};
    static const std::vector<std::vector<std::vector<int>>> sequences {
            {
                    {3, 2, 1, 0}, {1, 2, 3, 0}
            },
            {
                    {4, 2, 5, 0}, {5, 2, 4, 0}
            },
            {
                    {4, 3, 5, 1}, {5, 3, 4, 1}
            }
    };

    int dimension;
    int direction;

    if (configuration.dimension == RotationConfiguration::Horizontal) {
        dimension = 2;
    } else if (configuration.orientation == RotationConfiguration::Front)
        dimension = 0;
    else
        dimension = 1;

    if (configuration.direction > 0)
        direction = 0;
    else
        direction = 1;

    for (int i = 0; i < abs(configuration.direction / 90); ++i) {
        std::queue<std::pair<Urho3D::Node *, int>> queue;

        queue.push({faces[begins[dimension]], colors[begins[dimension]]});

        for (int j : sequences[dimension][direction]) {
            queue.push({faces[j], colors[j]});
            auto pair = queue.front();
            queue.pop();

            faces[j] = pair.first;
            colors[j] = pair.second;
        }
    }
}