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


namespace Visualization {
    class CubeAnimator {
        std::queue<RotationConfiguration> rotationQueue;

        Visualization::Cube *visualCube;
        Logic::Cube logicCube;

        Logic::GeneticSolver solver;

        const int FPS;
        bool animating_ = false;

        RotationConfiguration &configuration;

        float duration_;
        float degreesLeft_;

        inline float step() {
            return (float) configuration.direction /
                   (configuration.duration() * (float) FPS);
        };

        void setCurrentRotation(const RotationConfiguration &);

        void addConfiguration(const RotationConfiguration &);

    public:
        CubeAnimator(Cube *cube, int fps)
                : visualCube(cube), FPS(fps), configuration(cube->Configuration()),
                  solver(Logic::GeneticSolver(1000, 20, 200, 10,
                                              std::thread::hardware_concurrency() - 2, Logic::SpeedOptimized)) {};

        void RenderFrame();

        void Solve();

        void AddRotation(RotationConfiguration::Direction direction);

        bool TryMovePosition(long at);

        bool TryFlipOrientation();

        bool TryFlipDimension();

        void AddMove(Moves);

        void AddMoves(const std::vector<Moves> &);

        inline bool IsHorizontal() { return configuration.dimension; }

        inline Urho3D::Vector3 Center() { return visualCube->Center(); }
    };
}


#endif //GENETICCUBE_CUBEANIMATOR_HPP
