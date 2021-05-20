//
// Created by Георгий Круглов on 17.05.2021.
//

#include "../CubeAnimator.hpp"

using namespace Visualization;

void CubeAnimator::RenderFrame() {
    if (animating_) {
        degreesLeft_ -= step();
        if (degreesLeft_ == 0) {
            animating_ = false;
            visualCube->Rotate(step(), true);
            logicCube.PerformMove(configuration.ToMove());
//            std::cout << logicCube;
//            std::cout << "direction: " << configuration.direction << '\n'
//                      << "position: " << configuration.position << '\n'
//                      << "move: " << configuration.ToMove() << '\n';
        } else
            visualCube->Rotate(step(), false);
    } else if (!rotationQueue.empty()) {
        setCurrentRotation(rotationQueue.front());
        rotationQueue.pop();

        animating_ = true;
        RenderFrame();
    }
}

void CubeAnimator::Solve() {
    while (!rotationQueue.empty())
        rotationQueue.pop();

    if (animating_)
        return;

    std::vector<Moves> sequence;
    Logic::Cube solution;
    std::thread th([&] {
        solution = solver.Solve(logicCube);
    });

    th.join();

    AddMoves(solution.History());
}

void CubeAnimator::setCurrentRotation(const RotationConfiguration &conf) {
    assert(!animating_);

    animating_ = true;
    visualCube->Deselect();
    configuration = conf;
    visualCube->Select();
    degreesLeft_ = configuration.direction;
}

void CubeAnimator::addConfiguration(const RotationConfiguration &conf) {
    rotationQueue.push(conf);
}

void CubeAnimator::AddRotation(RotationConfiguration::Direction direction) {
    RotationConfiguration newConf(configuration);
    newConf.direction = direction;

    addConfiguration(newConf);
}

bool CubeAnimator::TryMovePosition(long at) {
    if (animating_)
        return false;

    visualCube->MovePosition(at);

    return true;
}

bool CubeAnimator::TryFlipOrientation() {
    if (animating_)
        return false;

    visualCube->FlipOrientation();

    return true;
}

bool CubeAnimator::TryFlipDimension() {
    if (animating_)
        return false;

    visualCube->FlipDimension();

    return true;
}

void CubeAnimator::AddMove(Moves move) {
    addConfiguration(move);
}

void CubeAnimator::AddMoves(const std::vector<Moves> &moves) {
    for (const Moves &move : moves)
        addConfiguration(move);
}
