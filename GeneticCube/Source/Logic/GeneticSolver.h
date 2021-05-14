//
// Created by Георгий Круглов on 12.05.2021.
//

#ifndef GENETICCUBE_GENETICSOLVER_H
#define GENETICCUBE_GENETICSOLVER_H

#include "Cube.h"

#include <vector>
#include <iostream>

namespace Logic {
    enum OptimizationType {
        SpeedOptimized,
        LengthOptimized
    };

    class GeneticSolver {
        const std::vector<std::vector<Moves>> permutations{
                {FR, LR, BR, RR, UR, R,  UR, B,  L,  F,  R,  U,  RR, U},
                {F,  R,  B,  L,  U,  LR, U,  BR, RR, FR, LR, UR, L,  UR},
                {U2, B,  U2, BR, R2, F,  RR, FR, U2, FR, U2, F,  RR},
                {U2, R,  U2, RR, F2, L,  FR, LR, U2, LR, U2, L,  FR},
                {UR, B2, D2, LR, F2, D2, B2, RR, UR},
                {U,  B2, D2, R,  F2, D2, B2, L,  U},
                {DR, RR, D,  R2, UR, R,  B2, L,  UR, LR, B2, U,  R2},
                {D,  L,  DR, L2, U,  LR, B2, RR, U,  R,  B2, UR, L2},
                {RR, U,  LR, U2, R,  UR, L,  RR, U,  LR, U2, R,  UR, L,  UR},
                {L,  UR, R,  U2, LR, U,  RR, L,  UR, R,  U2, LR, U,  RR, U},
                {FR, U,  B,  UR, F,  U,  BR, UR},
                {F,  UR, BR, U,  FR, UR, B,  U},
                {LR, U2, L,  RR, F2, R},
                {RR, U2, R,  LR, B2, L}
        };
        const std::vector<std::vector<Moves>> fullRotations{
                //Поворот вокруг оси x по часовой стрелке
                {LR, MR, R},
                //Поворот вокруг оси x против часовой стрелке
                {L,  M,  RR},
                //Поворот вокруг оси x на 180 градусов
                {L2, M2, R2},

                //Поворот вокруг оси y по часовой стрелке
                {U,  ER, DR},
                //Поворот вокруг оси y против часовой стрелки
                {UR, E,  D},
                //Поворот вокруг оси y на 180 градусов
                {U2, E2, D2}
        };
        const std::vector<std::vector<Moves>> orientations{
                //Поворот вокруг оси z по часовой стрелке
                {F,  S,  BR},
                //Поворот вокруг оси z против часовой стрелки
                {FR, SR, B},
                //Поворот вокруг оси z на 180 градусов
                {F2, S2, B2}
        };

        const Cube source;
        const Cube scrambled;
        std::basic_ostream<char> &out;

        void initPopulation(std::vector<Cube> &);

        void mutate(int eliteSize, std::vector<Cube> &population);

        void printCubes(const int &from, const int &upTo, const std::vector<Cube> &population) const;

        std::pair<Cube, Cube>
        solve(int populationSize, int eliteSize, int maxGenerationsCount, int maxResetCount);

    public:
        GeneticSolver(const Cube &, std::basic_ostream<char> &out = std::cout);

        //Первый куб - разобранный, хранит последовательность разборки, второй собранный, хранит последовательность для сборки первого,
        // если решение не найдено, второй куб - первый, без истории комманд
        std::pair<Cube, Cube>
        Solve(int populationSize, int eliteSize, int maxGenerationsCount, int maxResetCount,
              int maxThreadCount = -1, OptimizationType optimizationType = SpeedOptimized);
    };
}

#endif //GENETICCUBE_GENETICSOLVER_H
