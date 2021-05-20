//
// Created by Георгий Круглов on 28.03.2021.
//

#include "../Cube.hpp"

#include <cmath>
#include <sstream>
#include <iostream>
#include <random>

using namespace Visualization;

Cube::Cube(Urho3D::SharedPtr<Urho3D::Scene> &scene, Urho3D::ResourceCache *cache) {
    center_ = {
            (SCALE / (float) (CUBE_SIZE * CUBE_SIZE) + SPACING),
            (SCALE / (float) (CUBE_SIZE * CUBE_SIZE) + SPACING),
            (SCALE / (float) (CUBE_SIZE * CUBE_SIZE) + SPACING)
    };

    unselected_ = cache->GetResource<Urho3D::Material>("Materials/RubikUnselected.xml");
    selected_ = cache->GetResource<Urho3D::Material>("Materials/RubikSelected.xml");

    cubes.resize(CUBE_SIZE);
    for (size_t i = 0; i < CUBE_SIZE; ++i) {
        cubes[i].resize(CUBE_SIZE);

        for (size_t j = 0; j < CUBE_SIZE; ++j) {
            cubes[i][j].resize(CUBE_SIZE);

            for (size_t k = 0; k < CUBE_SIZE; ++k) {
                if (i == CUBE_SIZE / 2 && j == CUBE_SIZE / 2 && k == CUBE_SIZE / 2) {
                    cubes[i][j][k] = nullptr;
                    continue;
                }

                std::ostringstream s;
                s << i << ' ' << j << ' ' << k;


                auto node = scene->CreateChild(s.str().c_str());
                node->SetPosition({
                                          (SCALE / (float) (CUBE_SIZE * CUBE_SIZE) + SPACING) * (float) j,

                                          (SCALE / (float) (CUBE_SIZE * CUBE_SIZE) + SPACING) * (float) i,

                                          (SCALE / (float) (CUBE_SIZE * CUBE_SIZE) + SPACING) * (float) k});

                node->Scale(SCALE / (float) (CUBE_SIZE * CUBE_SIZE));

                auto model = node->CreateComponent<Urho3D::StaticModel>();
                model->SetModel(cache->GetResource<Urho3D::Model>("Models/Cube.mdl"));
                model->SetMaterial(unselected_);

                model->SetCastShadows(true);

                cubes[i][j][k] = node;
            }
        }
    }

    setSelectedMaterial(selected_);
}

void Cube::setSelectedMaterial(const Urho3D::SharedPtr<Urho3D::Material> &material) {
    for (size_t freeCompOne = 0; freeCompOne < CUBE_SIZE; ++freeCompOne)
        for (size_t freeCompTwo = 0; freeCompTwo < CUBE_SIZE; ++freeCompTwo) {
            size_t i, j, k;

            std::tie(i, j, k) = getCoordinates(freeCompOne, freeCompTwo);

            if (!cubes[i][j][k])
                continue;

            auto model = cubes[i][j][k]->GetComponent<Urho3D::StaticModel>();
            model->SetMaterial(material);
        }
}

std::tuple<size_t, size_t, size_t> Cube::getCoordinates(size_t first, size_t second) const {
    size_t i, j, k;

    if (configuration.orientation) {
        i = configuration.dimension ? configuration.position : first;
        j = configuration.dimension ? first : configuration.position;
        k = second;
    } else {
        i = configuration.dimension ? configuration.position : first;
        j = second;
        k = configuration.dimension ? first : configuration.position;
    }

    return {i, j, k};
}

std::tuple<size_t, size_t, size_t>
Cube::getRotatedCoordinates(size_t i, size_t j, size_t k) const {
    Urho3D::IntVector3 center;
    if (configuration.orientation) {
        center = {static_cast<int>(configuration.dimension ? i : CUBE_SIZE / 2),
                  static_cast<int>(configuration.dimension ? CUBE_SIZE / 2 : j),
                  static_cast<int>(CUBE_SIZE / 2)};
    } else {
        center = {static_cast<int>(configuration.dimension ? i : CUBE_SIZE / 2),
                  static_cast<int>(CUBE_SIZE / 2),
                  static_cast<int>(configuration.dimension ? CUBE_SIZE / 2 : k)};
    }
    auto vector = Urho3D::IntVector3{static_cast<int>(i), static_cast<int>(j), static_cast<int>(k)} - center;

    Urho3D::Quaternion quaternion;

    if (configuration.orientation) {
        quaternion = {static_cast<float>(-configuration.direction), {
                configuration.dimension ? 1.0f : 0,
                configuration.dimension ? 0 : 1.0f,
                0}};
    } else {
        quaternion = {static_cast<float>(-configuration.direction), {
                configuration.dimension ? 1.0f : 0,
                0,
                configuration.dimension ? 0 : 1.0f}};
    }

    Urho3D::Vector3 result =
            (quaternion.RotationMatrix() * static_cast<Urho3D::Vector3>(vector)) + static_cast<Urho3D::Vector3>(center);

    size_t I = llround(result.x_), J = llround(result.y_), K = llround(result.z_);

//    std::cout << i << ' ' << j <<' ' << k << " -- " << I << ' ' << J << ' ' << K << '\n';

    return {I, J, K};
}

void Cube::Rotate(float degree, bool sync) {
    std::vector<std::vector<std::vector<Urho3D::Node *>>> invariant;

    if (sync) {
        invariant.resize(CUBE_SIZE, std::vector<std::vector<Urho3D::Node *>>
                (CUBE_SIZE, std::vector<Urho3D::Node *>(CUBE_SIZE, nullptr)));
    }

    for (size_t freeCompOne = 0; freeCompOne < CUBE_SIZE; ++freeCompOne) {
        for (size_t freeCompTwo = 0; freeCompTwo < CUBE_SIZE; ++freeCompTwo) {
            size_t i, j, k;
            Urho3D::Quaternion rotation;

            std::tie(i, j, k) = getCoordinates(freeCompOne, freeCompTwo);

            if (!cubes[i][j][k])
                continue;

            if (configuration.orientation) {
                if (configuration.dimension)
                    rotation = {degree, {0, 1, 0}};
                else
                    rotation = {degree, {1, 0, 0}};

            } else {
                if (configuration.dimension)
                    rotation = {degree, {0, 1, 0}};
                else
                    rotation = {degree, {0, 0, 1}};
            }

            cubes[i][j][k]->RotateAround(center_, rotation, Urho3D::TS_WORLD);

            if (sync != 0) {
                size_t I, J, K;
                std::tie(I, J, K) = getRotatedCoordinates(i, j, k);

                invariant[I][J][K] = cubes[i][j][k];
            }
        }
    }

    if (sync != 0) {
        for (size_t freeCompOne = 0; freeCompOne < CUBE_SIZE; ++freeCompOne)
            for (size_t freeCompTwo = 0; freeCompTwo < CUBE_SIZE; ++freeCompTwo) {
                size_t i, j, k;
                std::tie(i, j, k) = getCoordinates(freeCompOne, freeCompTwo);

                if (!cubes[i][j][k])
                    continue;

                cubes[i][j][k] = invariant[i][j][k];
            }
    }
}

void Cube::MovePosition(long at) {
    Deselect();

    if ((long) configuration.position + at < 0)
        at = ((long) CUBE_SIZE + at) % (long) CUBE_SIZE;

    configuration.position = static_cast<RotationConfiguration::Position>((configuration.position + at) % CUBE_SIZE);

    Select();
}

void Cube::FlipDimension() {
    Deselect();
    configuration.dimension = static_cast<RotationConfiguration::Dimension>(!configuration.dimension);
    Select();
}

void Cube::FlipOrientation() {
    Deselect();
    configuration.orientation = static_cast<RotationConfiguration::Orientation>(!configuration.orientation);
    Select();
}

void Cube::Deselect() {
    setSelectedMaterial(unselected_);
}

void Cube::Select() {
    setSelectedMaterial(selected_);
}