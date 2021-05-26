//
// Created by Георгий Круглов on 17.05.2021.
//

#include "../CubeAnimator.hpp"

using namespace Visualization;

void CubeAnimator::RenderFrame() {
    if (solved_ && solving_) {
        solving_ = false;
        AddMoves(solution.History());
    } else if (solving_)
        return;

    if (animating_) {
        degreesLeft_ -= step_;
        if (degreesLeft_ == 0) {
            animating_ = false;
            visualCube->Rotate(step_, true);
            logicCube.PerformMove(configuration.ToMove());

            if (rotationQueue.empty() && solved_) {
                logicCube.ClearHistory();
                solved_ = false;
            }
        } else
            visualCube->Rotate(step_, false);
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

    if (animating_ || solving_)
        return;

    solver.SolveAsync(logicCube, solution, solving_, solved_);
}

void CubeAnimator::setCurrentRotation(const RotationConfiguration &conf) {
    assert(!animating_);

    animating_ = true;
    visualCube->Deselect();
    configuration = conf;
    visualCube->Select();
    degreesLeft_ = configuration.direction;
    calculateStep();
}

void CubeAnimator::addConfiguration(const RotationConfiguration &conf) {
    rotationQueue.push(conf);
}

void CubeAnimator::AddRotation(RotationConfiguration::Direction direction) {
    if (solving_ || solved_)
        return;

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

void CubeAnimator::AddMove(Logic::Moves move) {
    if (solving_)
        return;

    addConfiguration(move);
}

void CubeAnimator::AddMoves(const std::vector<Logic::Moves> &moves) {
    if (solving_)
        return;

    for (const Logic::Moves &move : moves)
        addConfiguration(move);
}
