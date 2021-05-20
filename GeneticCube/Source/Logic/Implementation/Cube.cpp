//
// Created by Георгий Круглов on 11.05.2021.
//

#include "../Cube.hpp"
#include "../CubeIndicesData.hpp"

#include <algorithm>
#include <queue>
#include <random>
#include <sstream>

using namespace Logic;

std::string Cube::ToString() const {
    static const char spacer[] = "       ";
    std::stringstream out;

    std::cout << '\n';
    for (int row = 0; row < 3; ++row) {
        out << spacer;
        for (int column = 0; column < 3; ++column) {
            out << unwrap[0][row * 3 + column] << ' ';
        }

        out << '\n';
    }
    out << '\n';

    for (int row = 0; row < 3; ++row) {
        for (const int &face : {4, 1, 5}) {
            for (int column = 0; column < 3; ++column) {
                out << unwrap[face][row * 3 + column] << ' ';
            }

            out << ' ';
        }

        out << '\n';
    }
    out << "\n";

    for (const int &face : {2, 3}) {
        for (int row = 0; row < 3; ++row) {
            out << spacer;
            for (int column = 0; column < 3; ++column) {
                out << unwrap[face][row * 3 + column] << ' ';
            }

            out << '\n';
        }
        out << "\n";
    }
    out << "\n";

    return out.str();
}

std::ostream &operator<<(std::ostream &out, const Cube &cube) {
    return out << cube.ToString();
}

Cube &Cube::operator=(const Cube rhs) {
    if (&rhs != this) {
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 9; ++j) {
                unwrap[i][j] = rhs.unwrap[i][j];
            }

        history = rhs.history;
        fitness_ = rhs.fitness_;
    }

    return *this;
}

void Cube::PerformMove(Moves move) {
    using namespace CubeIndicesData::Rotation;

    history.push_back(move);

    if (move % 10 < 6) {
        Color buffer[9];
        std::copy(std::begin(unwrap[move % 10]), std::end(unwrap[move % 10]), buffer);
        //Вращение соответствующей стороны
        int counter = 0;
        for (const int &i : faceletRotation[move / 10])
            unwrap[move % 10][counter++] = buffer[i];
    }

    if (move / 10 == 2) {
        move = Moves(move % 10);

        rotateAdj(
                std::vector<int>(
                        std::begin(subFaces[move % 10][move / 10]),
                        std::end(subFaces[move % 10][move / 10])),
                subFaceletsRotation[move % 10],
                begins[move % 10]);
    }

    rotateAdj(
            std::vector<int>(
                    std::begin(subFaces[move % 10][move / 10]),
                    std::end(subFaces[move % 10][move / 10])),
            subFaceletsRotation[move % 10],
            begins[move % 10]);

    fitness_ = countFitness();
}

void Cube::rotateAdj(const std::vector<int> &subFaces, const int facelets[6][3], int begin) {
    //Вращение смежных сторон
    std::queue<Color> queue;
    for (const int &facelet : facelets[begin])
        queue.push(unwrap[begin][facelet]);

    for (const int &face : subFaces) {
        for (const int &facelet : facelets[face])
            queue.push(unwrap[face][facelet]);

        for (const int &facelet : facelets[face]) {
            unwrap[face][facelet] = queue.front();
            queue.pop();
        }
    }
}

int Cube::NaiveFitness() const {
    int fitness = 0;

    for (const auto &i : unwrap)
        for (const auto &j : i)
            if (j == i[4])
                ++fitness;

    return fitness;
}

int Cube::countFitness() const {
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

std::vector<int> Cube::fitnessBottomCross() const {
    std::vector<int> satisfyingSides;

    for (int side = 0; side < 6; ++side)
        if (unwrap[side][4] == unwrap[side][1] &&
            unwrap[side][4] == unwrap[side][3] &&
            unwrap[side][4] == unwrap[side][5] &&
            unwrap[side][4] == unwrap[side][7])
            satisfyingSides.push_back(side);

    return satisfyingSides;
}

std::vector<int> Cube::fitnessBars(const std::vector<int> &sides) const {
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

std::vector<int> Cube::fitnessFullSide(const std::vector<int> &sides) const {
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

std::vector<int> Cube::fitnessPyramid(const std::vector<int> &sides) const {
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

std::vector<int> Cube::fitnessBag(const std::vector<int> &sides) const {
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

std::vector<int> Cube::fitnessTopCross(const std::vector<int> &sides) const {
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

std::vector<int> Cube::fitnessNoTopCorners(const std::vector<int> &sides) const {
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

std::vector<int> Cube::fitnessSolved(const std::vector<int> &sides) const {
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

void Cube::Randomize() {
    std::random_device device;
    std::mt19937 mt(device());

    std::uniform_int_distribution<int> commandCountPicker(20, 120);
    int commandCount = commandCountPicker(device);

    for (int i = 0; i < commandCount; ++i) {
        PerformMove(randomMove());
    }

    std::cout << '\n';
}