//
// Created by Георгий Круглов on 12.05.2021.
//

#ifndef GENETICCUBE_GENETICSOLVER_HPP
#define GENETICCUBE_GENETICSOLVER_HPP

#include "Cube.hpp"
#include "../Utility/ThreadPool.hpp"
#include "SuccessfulSequences.hpp"

#include <vector>
#include <iostream>
#include <thread>
#include <future>

namespace Logic {
    enum OptimizationType {
        SpeedOptimized,
        LengthOptimized
    };

    class GeneticSolver {
        struct SolutionCycle;

        const std::vector<std::vector<Moves>> permutations {
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
        const std::vector<std::vector<Moves>> fullRotations {
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
        const std::vector<std::vector<Moves>> orientations {
                //Поворот вокруг оси z по часовой стрелке
                {F,  S,  BR},
                //Поворот вокруг оси z против часовой стрелки
                {FR, SR, B},
                //Поворот вокруг оси z на 180 градусов
                {F2, S2, B2}
        };

        std::mutex learningMutex;
        SuccessfulSequences successfulSequences;

        bool solutionFound = false;

        Cube source;
        Cube scrambled;

        bool verbose_;
        std::basic_ostream<char> &out_;

        unsigned int populationSize_;
        unsigned int eliteSize_;
        unsigned int maxGenerationsCount_;
        unsigned int maxResetCount_;
        unsigned int threadCount_;
        OptimizationType optimizationType_;

        std::mutex logMutex;
        Utility::ThreadPool threadPool;

        void initPopulation(std::vector<Cube> &) const;

        void mutate(Cube &cube, std::vector<Cube> &population, std::mt19937 &generator);

        void logTopPerformers(const unsigned int &generationsCount, const std::vector<Cube> &population) const;

        void logSolutions(const unsigned int &solutionsCount, const std::vector<Cube> &population) const;

        void logProgressMultiThread(const Cube &population, unsigned int generationNumber);

        void printCubes(unsigned int upTo, const std::vector<Cube> &population) const;


        Cube solve();

    public:

        Cube optimize(const Cube &);

        GeneticSolver(unsigned int populationSize, unsigned int eliteSize, unsigned int maxGenerationsCount,
                      unsigned int maxResetCount, unsigned int threadCount = 1,
                      OptimizationType optimizationType = SpeedOptimized,
                      bool verbose = false, std::basic_ostream<char> &out = std::cout);

        void SolveAsync(const Cube &cube, Cube &solution, bool &solving, bool &solved);

        Cube Solve(const Cube &cube);
    };
}

#endif //GENETICCUBE_GENETICSOLVER_HPP
