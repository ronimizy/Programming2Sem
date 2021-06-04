//
// Created by Георгий Круглов on 28.03.2021.
//

#ifndef LAB6_CUBE_H
#define LAB6_CUBE_H

#include "Urho3DAll.hpp"

#include <vector>
#include <tuple>

#include "RotationConfiguration.hpp"
#include "Box.hpp"
#include "../Logic/Cube.hpp"

namespace Visualization {
    class Cube {
    private:

        std::vector<std::vector<std::vector<Box *>>> cubes;
        Urho3D::Vector3 center_;

        RotationConfiguration configuration;

        const size_t CUBE_SIZE = 3;
        const float SCALE = 20.0f;
        const float SPACING = 0.1f;

        void setSelection(bool selected);

        [[nodiscard]]
        std::tuple<size_t, size_t, size_t> getCoordinates(size_t first, size_t second) const;

        [[nodiscard]]
        std::tuple<size_t, size_t, size_t>
        getRotatedCoordinates(size_t i, size_t j, size_t k) const;
    public:
        Cube(Urho3D::SharedPtr<Urho3D::Scene> &scene, Urho3D::ResourceCache *cache);

        ~Cube() {
            for (auto &slice : cubes) {
                for (auto &row : slice) {
                    for (Box *box : row)
                        delete box;
                }
            }
        }

        void Rotate(float degree, bool sync);

        void MovePosition(long at);

        void FlipDimension();

        void FlipOrientation();

        void Deselect();

        void Select();

        inline RotationConfiguration &Configuration() { return configuration; }

        Urho3D::Vector3 Center() { return center_; }

        void SetUnwrap(const Logic::Cube &);
    };
}

#endif //LAB6_CUBE_H
