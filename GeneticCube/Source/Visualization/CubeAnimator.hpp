//
// Created by Георгий Круглов on 17.05.2021.
//

#ifndef GENETICCUBE_CUBEANIMATOR_HPP
#define GENETICCUBE_CUBEANIMATOR_HPP

#include "Cube.hpp"
#include "../Logic/Cube.hpp"
#include "../Logic/GeneticSolver.hpp"

#include <queue>
#include <thread>
#include <future>


namespace Visualization {
    class CubeAnimator {
        std::queue<RotationConfiguration> rotationQueue;

        Visualization::Cube *visualCube;
        Logic::Cube logicCube;

        Logic::GeneticSolver solver;

        const int FPS;
        bool animating_ = false;

        bool solving_ = false;
        bool solved_ = false;
        Logic::Cube solution;

        RotationConfiguration &configuration;

        float degreesLeft_;
        float step_;
        static constexpr const float baseDuration = 0.2f;
        float singleDuration = 0.2f;

        inline void calculateStep() {
            step_ = (float) configuration.direction / (singleDuration * FPS);
        };

        void setCurrentRotation(const RotationConfiguration &);

        void addConfiguration(const RotationConfiguration &);

    public:
        CubeAnimator(Cube *cube, int fps)
                : visualCube(cube), FPS(fps), configuration(cube->Configuration()),
                  solver(Logic::GeneticSolver(1000, 20, 200, 10,
                                              std::thread::hardware_concurrency(), Logic::Balanced,
                                              Logic::Descriptive)) {};

        ~CubeAnimator() {
            delete visualCube;
        }

        void RenderFrame();

        void Solve();

        void AddRotation(RotationConfiguration::Direction direction);

        bool TryMovePosition(long at);

        bool TryFlipOrientation();

        bool TryFlipDimension();

        void AddMove(Moves);

        void AddMoves(const std::vector<Moves> &);

        inline bool IsHorizontal() const { return configuration.dimension; }

        inline Urho3D::Vector3 Center() { return visualCube->Center(); }

        inline void SetDuration(float d) {
            if (animating_)
                return;

            singleDuration = baseDuration + d;
        }

        inline float &GetDuration() { return singleDuration; }

        inline void SetUnwrap(const Logic::Cube &cube) {
            logicCube = cube;
            visualCube->SetUnwrap(cube);
        }
    };
}


#endif //GENETICCUBE_CUBEANIMATOR_HPP
