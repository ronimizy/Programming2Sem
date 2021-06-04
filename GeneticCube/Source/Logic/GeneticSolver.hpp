//
// Created by Георгий Круглов on 12.05.2021.
//

#ifndef GENETICCUBE_GENETICSOLVER_HPP
#define GENETICCUBE_GENETICSOLVER_HPP

#include "CubeIndividual.hpp"
#include "../Utility/ThreadPool.hpp"
#include "SuccessfulSequences.hpp"

#include <vector>
#include <iostream>
#include <thread>
#include <future>

namespace Logic {
    enum OptimizationType {
        SpeedOptimized,
        Balanced,
        LengthOptimized
    };
    enum LoggingMode {
        Silent,
        Descriptive,
        Verbose
    };

    class GeneticSolver {
        const std::vector<std::vector<Move>> permutations {
                {Moves::FR, Moves::LR, Moves::BR, Moves::RR, Moves::UR, Moves::R,  Moves::UR, Moves::B,  Moves::L,  Moves::F,  Moves::R,  Moves::U,  Moves::RR, Moves::U},
                {Moves::F,  Moves::R,  Moves::B,  Moves::L,  Moves::U,  Moves::LR, Moves::U,  Moves::BR, Moves::RR, Moves::FR, Moves::LR, Moves::UR, Moves::L,  Moves::UR},
                {Moves::U2, Moves::B,  Moves::U2, Moves::BR, Moves::R2, Moves::F,  Moves::RR, Moves::FR, Moves::U2, Moves::FR, Moves::U2, Moves::F,  Moves::RR},
                {Moves::U2, Moves::R,  Moves::U2, Moves::RR, Moves::F2, Moves::L,  Moves::FR, Moves::LR, Moves::U2, Moves::LR, Moves::U2, Moves::L,  Moves::FR},
                {Moves::RR, Moves::U,  Moves::LR, Moves::U2, Moves::R,  Moves::UR, Moves::L,  Moves::RR, Moves::U,  Moves::LR, Moves::U2, Moves::R,  Moves::UR, Moves::L,  Moves::UR},
                {Moves::L,  Moves::UR, Moves::R,  Moves::U2, Moves::LR, Moves::U,  Moves::RR, Moves::L,  Moves::UR, Moves::R,  Moves::U2, Moves::LR, Moves::U,  Moves::RR, Moves::U},
                {Moves::DR, Moves::RR, Moves::D,  Moves::R2, Moves::UR, Moves::R,  Moves::B2, Moves::L,  Moves::UR, Moves::LR, Moves::B2, Moves::U,  Moves::R2},
                {Moves::D,  Moves::L,  Moves::DR, Moves::L2, Moves::U,  Moves::LR, Moves::B2, Moves::RR, Moves::U,  Moves::R,  Moves::B2, Moves::UR, Moves::L2},
                {Moves::UR, Moves::B2, Moves::D2, Moves::LR, Moves::F2, Moves::D2, Moves::B2, Moves::RR, Moves::UR},
                {Moves::U,  Moves::B2, Moves::D2, Moves::R,  Moves::F2, Moves::D2, Moves::B2, Moves::L,  Moves::U},
                {Moves::FR, Moves::U,  Moves::B,  Moves::UR, Moves::F,  Moves::U,  Moves::BR, Moves::UR},
                {Moves::F,  Moves::UR, Moves::BR, Moves::U,  Moves::FR, Moves::UR, Moves::B,  Moves::U},
                {Moves::LR, Moves::U2, Moves::L,  Moves::RR, Moves::F2, Moves::R},
                {Moves::RR, Moves::U2, Moves::R,  Moves::LR, Moves::B2, Moves::L}
        };
        const std::vector<std::vector<Move>> fullRotations {
                //Поворот вокруг оси x по часовой стрелке
                {Moves::LR, Moves::MR, Moves::R},
                //Поворот вокруг оси x против часовой стрелке
                {Moves::L,  Moves::M,  Moves::RR},
                //Поворот вокруг оси x на 180 градусов
                {Moves::L2, Moves::M2, Moves::R2},

                //Поворот вокруг оси y по часовой стрелке
                {Moves::U,  Moves::ER, Moves::DR},
                //Поворот вокруг оси y против часовой стрелки
                {Moves::UR, Moves::E,  Moves::D},
                //Поворот вокруг оси y на 180 градусов
                {Moves::U2, Moves::E2, Moves::D2}
        };
        const std::vector<std::vector<Move>> orientations {
                //Поворот вокруг оси z по часовой стрелке
                {Moves::F,  Moves::S,  Moves::BR},
                //Поворот вокруг оси z против часовой стрелки
                {Moves::FR, Moves::SR, Moves::B},
                //Поворот вокруг оси z на 180 градусов
                {Moves::F2, Moves::S2, Moves::B2}
        };

        std::mutex learningMutex;
        SuccessfulSequences successfulSequences;

        bool solutionFound = false;

        CubeIndividual source;
        CubeIndividual scrambled;

        LoggingMode loggingMode_;
        std::basic_ostream<char> &out_;

        unsigned int populationSize_;
        unsigned int eliteSize_;
        unsigned int maxGenerationsCount_;
        unsigned int maxResetCount_;
        unsigned int threadCount_;
        OptimizationType optimizationType_;

        std::mutex logMutex;
        Utility::ThreadPool threadPool;

        void initPopulation(std::vector<CubeIndividual> &) const;

        void mutate(CubeIndividual &cube, std::vector<CubeIndividual> &population, std::mt19937 &generator);

        void logTopPerformers(const unsigned int &generationsCount, const std::vector<CubeIndividual> &population) const;

        void logSolutions(const unsigned int &solutionsCount, const std::vector<CubeIndividual> &population) const;

        void logProgressMultiThread(const CubeIndividual &population, unsigned int generationNumber);

        void printCubes(unsigned int upTo, const std::vector<CubeIndividual> &population) const;


        CubeIndividual solve();
        CubeIndividual optimize(const CubeIndividual &);
    public:


        GeneticSolver(unsigned int populationSize, unsigned int eliteSize, unsigned int maxGenerationsCount,
                      unsigned int maxResetCount, unsigned int threadCount = 1,
                      OptimizationType optimizationType = SpeedOptimized,
                      LoggingMode loggingMode = Descriptive, std::basic_ostream<char> &out = std::cout);

        void SolveAsync(const CubeIndividual &cube, CubeIndividual &solution, bool &solving, bool &solved);

        CubeIndividual Solve(const CubeIndividual &cube);
    };
}

#endif //GENETICCUBE_GENETICSOLVER_HPP
