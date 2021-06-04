//
// Created by Георгий Круглов on 17.05.2021.
//

#ifndef GENETICCUBE_CUBEANIMATOR_HPP
#define GENETICCUBE_CUBEANIMATOR_HPP

#include "Cube.hpp"
#include "../Logic/GeneticSolver.hpp"

#include <queue>
#include <thread>
#include <future>


namespace Visualization {
    class CubeAnimator {
        std::queue<RotationConfiguration> rotationQueue;

        Visualization::Cube *visualCube;
        Logic::CubeIndividual logicCube;

        Logic::GeneticSolver solver;

        const int FPS;
        bool animating_ = false;

        bool solving_ = false;
        bool solved_ = false;
        Logic::CubeIndividual solution;

        RotationConfiguration &configuration;

        float degreesLeft_;
        float step_;
        static constexpr const float baseDuration = 0.2f;
        float singleDuration = 0.2f;

        inline void calculateStep() {
            step_ = (float) configuration.direction / (singleDuration * (float) FPS);
        };

        void setCurrentRotation(const RotationConfiguration &);

        void addConfiguration(const RotationConfiguration &);

    public:
        CubeAnimator(Cube *cube, int fps)
                : visualCube(cube), FPS(fps), configuration(cube->Configuration()),
                  solver(Logic::GeneticSolver(1000, 20, 150, 10,
                                              std::thread::hardware_concurrency() - 1, Logic::SpeedOptimized,
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

        void AddMove(Logic::Move);

        void AddMoves(const std::vector<Logic::Move> &);

        inline bool IsHorizontal() const { return configuration.dimension; }

        inline Urho3D::Vector3 Center() { return visualCube->Center(); }

        inline void SetDuration(float d) { singleDuration = baseDuration + d; }

        inline float &GetDuration() { return singleDuration; }

        inline void SetUnwrap(const Logic::CubeIndividual &cube) {
            if (animating_ || solving_)
                return;

            logicCube = cube;
            visualCube->SetUnwrap(cube);
        }

        inline std::string GetCubeString() { return logicCube.ToString(); }
    };
}


#endif //GENETICCUBE_CUBEANIMATOR_HPP
