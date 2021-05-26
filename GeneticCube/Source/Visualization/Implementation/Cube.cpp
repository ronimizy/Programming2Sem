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

                auto node = new Box(cache, scene);
                node->SetPosition({
                                          (SCALE / (float) (CUBE_SIZE * CUBE_SIZE) + SPACING) * (float) j,

                                          (SCALE / (float) (CUBE_SIZE * CUBE_SIZE) + SPACING) * (float) i,

                                          (SCALE / (float) (CUBE_SIZE * CUBE_SIZE) + SPACING) * (float) k});

                node->SetScale(SCALE / (float) (CUBE_SIZE * CUBE_SIZE));

                cubes[i][j][k] = node;
            }
        }
    }

    SetUnwrap(Logic::Cube{});
    Select();
}

void Cube::setSelection(bool selected) {
    for (size_t freeCompOne = 0; freeCompOne < CUBE_SIZE; ++freeCompOne)
        for (size_t freeCompTwo = 0; freeCompTwo < CUBE_SIZE; ++freeCompTwo) {
            size_t i, j, k;

            std::tie(i, j, k) = getCoordinates(freeCompOne, freeCompTwo);

            if (!cubes[i][j][k])
                continue;

            if (selected)
                cubes[i][j][k]->Select();
            else
                cubes[i][j][k]->Deselect();
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
    auto vector = Urho3D::IntVector3 {static_cast<int>(i), static_cast<int>(j), static_cast<int>(k)} - center;

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
    std::vector<std::vector<std::vector<Box *>>> invariant;

    if (sync) {
        invariant.resize(CUBE_SIZE, std::vector<std::vector<Box *>>
                (CUBE_SIZE, std::vector<Box *>(CUBE_SIZE, nullptr)));
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


                cubes[i][j][k]->Sync(configuration);
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
    setSelection(false);
}

void Cube::Select() {
    setSelection(true);
}

void Cube::SetUnwrap(const Logic::Cube &cube) {
    std::vector<Logic::Color> colors;

    //Top slice
    cubes[2][0][2]->SetColors({cube[0][0], 0, 0, cube[3][2], cube[4][0], 0});
    cubes[2][1][2]->SetColors({cube[0][1], 0, 0, cube[3][1], 0, 0});
    cubes[2][2][2]->SetColors({cube[0][2], 0, 0, cube[3][0], 0, cube[5][2]});

    cubes[2][0][1]->SetColors({cube[0][3], 0, 0, 0, cube[4][1], 0});
    cubes[2][1][1]->SetColors({cube[0][4], 0, 0, 0, 0, 0});
    cubes[2][2][1]->SetColors({cube[0][5], 0, 0, 0, 0, cube[5][1]});

    cubes[2][0][0]->SetColors({cube[0][6], cube[1][0], 0, 0, cube[4][2], 0});
    cubes[2][1][0]->SetColors({cube[0][7], cube[1][1], 0, 0, 0, 0});
    cubes[2][2][0]->SetColors({cube[0][8], cube[1][2], 0, 0, 0, cube[5][0]});


    ///Mid slice
    cubes[1][0][2]->SetColors({0, 0, 0, cube[3][5], cube[4][3], 0});
    cubes[1][1][2]->SetColors({0, 0, 0, cube[3][4], 0, 0});
    cubes[1][2][2]->SetColors({0, 0, 0, cube[3][3], 0, cube[5][5]});

    cubes[1][0][1]->SetColors({0, 0, 0, 0, cube[4][4], 0});
    cubes[1][2][1]->SetColors({0, 0, 0, 0, 0, cube[5][3]});

    cubes[1][0][0]->SetColors({0, cube[1][3], 0, 0, cube[4][5], 0});
    cubes[1][1][0]->SetColors({0, cube[1][4], 0, 0, 0, 0});
    cubes[1][2][0]->SetColors({0, cube[1][5], 0, 0, 0, cube[5][3]});


    ///Bottom slice
    cubes[0][0][2]->SetColors({0, 0, cube[2][6], cube[3][8], cube[4][6], 0});
    cubes[0][1][2]->SetColors({0, 0, cube[2][7], cube[3][7], 0, 0});
    cubes[0][2][2]->SetColors({0, 0, cube[2][8], cube[3][6], 0, cube[5][8]});

    cubes[0][0][1]->SetColors({0, 0, cube[2][3], 0, cube[4][7], 0});
    cubes[0][1][1]->SetColors({0, 0, cube[2][4], 0, 0, 0});
    cubes[0][2][1]->SetColors({0, 0, cube[2][5], 0, 0, cube[5][7]});

    cubes[0][0][0]->SetColors({0, cube[1][6], cube[2][0], 0, cube[4][8], 0});
    cubes[0][1][0]->SetColors({0, cube[1][7], cube[2][1], 0, 0, 0});
    cubes[0][2][0]->SetColors({0, cube[1][8], cube[2][2], 0, 0, cube[5][6]});
}