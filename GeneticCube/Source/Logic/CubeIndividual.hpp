//
// Created by Георгий Круглов on 04.06.2021.
//

#ifndef GENETICCUBE_CUBEINDIVIDUAL_HPP
#define GENETICCUBE_CUBEINDIVIDUAL_HPP

#include "Cube.hpp"

namespace Logic {
    class CubeIndividual : public Cube {
        std::vector<Move> history;
        std::vector<Move> secondaryHistory;

        //opposites[i] - противоположная грань к грани i
        constexpr static const int opposites[6] = {2, 3, 0, 1, 5, 4};

        int fitness_ = FitnessStates::Solved;

        //Расчёт собранности кубика
        [[nodiscard]]
        int countFitness() const;

        //Оценка сборки креста
        [[nodiscard]]
        std::vector<int> fitnessBottomCross() const;

        //Оценка собранности боковых столбцов
        [[nodiscard]]
        std::vector<int> fitnessBars(const std::vector<int> &) const;

        //Оценка собранности целой стороны
        [[nodiscard]]
        std::vector<int> fitnessFullSide(const std::vector<int> &) const;

        //Оценка собранности боковых пирамид
        [[nodiscard]]
        std::vector<int> fitnessPyramid(const std::vector<int> &) const;

        //Оценка собранности всех facelet 'ов кроме верхней грани
        [[nodiscard]]
        std::vector<int> fitnessBag(const std::vector<int> &) const;

        //Оценка собранности верхнего креста
        [[nodiscard]]
        std::vector<int> fitnessTopCross(const std::vector<int> &) const;

        //Оценка собранности кубика без учёта верхних углов
        [[nodiscard]]
        std::vector<int> fitnessNoTopCorners(const std::vector<int> &) const;

        [[nodiscard]]
        std::vector<int> fitnessSolved(const std::vector<int> &) const;

    public:
        CubeIndividual() : Cube() {};

        CubeIndividual(bool randomized) {
            if (randomized)
                Randomize();
        };

        CubeIndividual(std::string &s) : Cube(s) {};

        CubeIndividual(const char *s) : Cube(s) {};

        CubeIndividual(const CubeIndividual &origin)
                : Cube(origin), fitness_(origin.fitness_), history(origin.history),
                  secondaryHistory(origin.secondaryHistory) {};

        CubeIndividual(CubeIndividual &&origin) noexcept
                : Cube(std::move(origin)), fitness_(origin.fitness_), history(std::move(origin.history)),
                  secondaryHistory(std::move(origin.secondaryHistory)) {}

        [[nodiscard]]
        inline const int &Fitness() const { return fitness_; }

        [[nodiscard]]
        int NaiveFitness() const;

        [[nodiscard]]
        inline const std::vector<Move> &History() const { return history; }

        inline CubeIndividual &ClearHistory() {
            history.clear();
            return *this;
        }

        [[nodiscard]]
        inline const std::vector<Move> &SecondaryHistory() const { return secondaryHistory; }

        inline CubeIndividual &ClearSecondaryHistory() {
            secondaryHistory.clear();
            return *this;
        }

        inline CubeIndividual &EraseMoveAt(long i) {
            history.erase(history.begin() + i);
            return *this;
        }

        inline CubeIndividual &InsertMoveAt(long i, Move move) {
            history.insert(history.begin() + i, move);
            return *this;
        }

        [[nodiscard]]
        CubeIndividual WithCleanHistory() const {
            CubeIndividual cube(*this);
            cube.ClearHistory();
            cube.ClearSecondaryHistory();
            return cube;
        }


    public:
        CubeIndividual &PerformMove(Move move) override {
            history.push_back(move);
            secondaryHistory.push_back(move);

            Cube::PerformMove(move);

            fitness_ = countFitness();

            return *this;
        }

        CubeIndividual &operator=(const CubeIndividual &rhs) {
            if (this != &rhs) {
                unwrap = rhs.unwrap;
                history = rhs.history;
//                secondaryHistory = rhs.secondaryHistory;
                fitness_ = rhs.fitness_;
            }

            return *this;
        }


        struct FitnessSortGreater {
            bool operator()(const CubeIndividual &lhs, const CubeIndividual &rhs) {
                return lhs.fitness_ > rhs.fitness_;
            }
        };

        struct HistorySortLess {
            bool operator()(const CubeIndividual &lhs, const CubeIndividual &rhs) {
                return lhs.History().size() < rhs.History().size();
            }
        };

        struct NotSolved {
            bool operator()(const CubeIndividual &cube) {
                return cube.fitness_ != FitnessStates::Solved;
            }
        };
    };
}


#endif //GENETICCUBE_CUBEINDIVIDUAL_HPP
