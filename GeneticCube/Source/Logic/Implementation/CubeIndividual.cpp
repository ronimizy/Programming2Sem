//
// Created by Георгий Круглов on 04.06.2021.
//

#include "../CubeIndividual.hpp"
#include "../CubeIndicesData.hpp"

using namespace Logic;

int CubeIndividual::NaiveFitness() const {
    int fitness = 0;

    for (const auto &i : unwrap)
        for (const auto &j : i)
            if (j == i[4])
                ++fitness;

    return fitness;
}

int CubeIndividual::countFitness() const {
    std::vector<int> fitness;
    std::vector<int> sides;

    fitness = fitnessBottomCross();
    if (fitness.empty())
        return NaiveFitness();


    sides = fitnessBars(fitness);
    if (sides.empty())
        return FitnessStates::BottomCross + NaiveFitness();

    fitness = sides;
    sides = fitnessFullSide(fitness);
    if (sides.empty())
        return FitnessStates::Bars + NaiveFitness();

    fitness = sides;
    sides = fitnessPyramid(fitness);
    if (sides.empty())
        return FitnessStates::FullSide + NaiveFitness();

    fitness = sides;
    sides = fitnessBag(fitness);
    if (sides.empty())
        return FitnessStates::Pyramid + NaiveFitness();

    fitness = sides;
    sides = fitnessTopCross(fitness);
    if (sides.empty())
        return FitnessStates::Bag + NaiveFitness();

    fitness = sides;
    sides = fitnessNoTopCorners(fitness);
    if (sides.empty())
        return FitnessStates::TopCross + NaiveFitness();

    fitness = sides;
    sides = fitnessSolved(fitness);
    if (sides.empty())
        return FitnessStates::NoTopCorners + NaiveFitness();

    return FitnessStates::Solved;
}

std::vector<int> CubeIndividual::fitnessBottomCross() const {
    std::vector<int> satisfyingSides;

    for (int side = 0; side < 6; ++side)
        if (unwrap[side][4] == unwrap[side][1] &&
            unwrap[side][4] == unwrap[side][3] &&
            unwrap[side][4] == unwrap[side][5] &&
            unwrap[side][4] == unwrap[side][7])
            satisfyingSides.push_back(side);

    return satisfyingSides;
}

std::vector<int> CubeIndividual::fitnessBars(const std::vector<int> &sides) const {
    using namespace CubeIndicesData::Fitness;

    std::vector<int> satisfyingSides;

    for (const int &side : sides) {
        bool satisfying = true;

        for (int i = 0; i < 6; ++i) {
            if (i == side || i == opposites[side])
                continue;

            if (unwrap[i][4] != unwrap[i][barsBases[side][i]]) {
                satisfying = false;
                break;
            }
        }

        if (satisfying)
            satisfyingSides.push_back(side);
    }

    return satisfyingSides;
}

std::vector<int> CubeIndividual::fitnessFullSide(const std::vector<int> &sides) const {
    std::vector<int> satisfyingSides;

    for (const int &side : sides) {
        bool satisfying = true;

        for (int i = 0; i < 9; ++i)
            if (unwrap[side][4] != unwrap[side][i]) {
                satisfying = false;
                break;
            }

        if (satisfying)
            satisfyingSides.push_back(side);
    }

    return satisfyingSides;
}

std::vector<int> CubeIndividual::fitnessPyramid(const std::vector<int> &sides) const {
    using namespace CubeIndicesData::Fitness;

    std::vector<int> satisfyingSides;

    for (const int &side : sides) {
        bool satisfying = true;

        for (int i = 0; i < 6; ++i) {
            if (i == opposites[side] || i == side)
                continue;

            if (unwrap[i][4] != unwrap[i][pyramidBases[side][i][0]] ||
                unwrap[i][4] != unwrap[i][pyramidBases[side][i][1]]) {
                satisfying = false;
                break;
            }
        }

        if (satisfying)
            satisfyingSides.push_back(side);
    }

    return satisfyingSides;
}

std::vector<int> CubeIndividual::fitnessBag(const std::vector<int> &sides) const {
    using namespace CubeIndicesData::Fitness;

    std::vector<int> satisfyingSides;

    for (const int &side : sides) {
        bool satisfying = true;

        for (int i = 0; i < 6; ++i) {
            if (i == side || i == opposites[side])
                continue;

            if (unwrap[i][4] != unwrap[i][bagEars[side][i][0]] ||
                unwrap[i][4] != unwrap[i][bagEars[side][i][1]]) {
                satisfying = false;
                break;
            }
        }

        if (satisfying)
            satisfyingSides.push_back(side);
    }

    return satisfyingSides;
}

std::vector<int> CubeIndividual::fitnessTopCross(const std::vector<int> &sides) const {
    std::vector<int> satisfyingSides;

    for (const int &side : sides) {
        if (unwrap[opposites[side]][4] == unwrap[opposites[side]][1] &&
            unwrap[opposites[side]][4] == unwrap[opposites[side]][3] &&
            unwrap[opposites[side]][4] == unwrap[opposites[side]][5] &&
            unwrap[opposites[side]][4] == unwrap[opposites[side]][7])
            satisfyingSides.push_back(side);
    }

    return satisfyingSides;
}

std::vector<int> CubeIndividual::fitnessNoTopCorners(const std::vector<int> &sides) const {
    using namespace CubeIndicesData::Fitness;

    std::vector<int> satisfyingSides;

    for (const int &side :sides) {
        bool satisfying = true;

        for (int i = 0; i < 6; ++i) {
            if (i == side || i == opposites[side])
                continue;

            if (unwrap[i][4] != unwrap[i][sidesTops[side][i]]) {
                satisfying = false;
                break;
            }
        }

        if (satisfying)
            satisfyingSides.push_back(side);
    }

    return satisfyingSides;
}

std::vector<int> CubeIndividual::fitnessSolved(const std::vector<int> &sides) const {
    using namespace CubeIndicesData::Fitness;

    std::vector<int> satisfyingSides;

    for (const int &side : sides) {
        bool satisfying = true;

        for (int i = 0; i < 6; ++i) {
            if (i == side)
                continue;

            for (const int &facelet : topCrossEars[side][i])
                if (unwrap[i][4] != unwrap[i][facelet]) {
                    satisfying = false;
                    break;
                }

            if (!satisfying)
                break;
        }

        if (satisfying)
            satisfyingSides.push_back(side);
    }

    return satisfyingSides;
}