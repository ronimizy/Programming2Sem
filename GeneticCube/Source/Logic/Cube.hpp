//
// Created by Георгий Круглов on 11.05.2021.
//

#ifndef GENETICCUBE_CUBE_HPP
#define GENETICCUBE_CUBE_HPP

#include <iostream>
#include <vector>
#include <compare>

#include "Moves.hpp"
#include "Color.hpp"
#include "FitnessStates.hpp"

namespace Logic {
    class Cube {
        //Развертка куба
        //Если представлять в голове сборку развёртки, то грань back будет
        //перевёрнута вверх ногами (не 0 1 2, а 6 7 8).
        //В данной реализации грань back не перевёрнута, то есть после "сборки"
        //развёртки, левый верхний угол будет иметь индекс 0 (так проще считать)
        /*
         *     0             up
         *   4 1 5    left front right
         *     2            down
         *     3            back
         *
         *          0 1 2
         *          3 4 5
         *          6 7 8
         *
         *   0 1 2  0 1 2  0 1 2
         *   3 4 5  3 4 5  3 4 5
         *   6 7 8  6 7 8  6 7 8
         *
         *          0 1 2
         *          3 4 5
         *          6 7 8
         *
         *          8 7 6
         *          5 4 3
         *          2 1 0
         */
        std::vector<std::vector<Color>> unwrap = {
                {0, 0, 0, 0, 0, 0, 0, 0, 0},
                {1, 1, 1, 1, 1, 1, 1, 1, 1},
                {2, 2, 2, 2, 2, 2, 2, 2, 2},
                {3, 3, 3, 3, 3, 3, 3, 3, 3},
                {4, 4, 4, 4, 4, 4, 4, 4, 4},
                {5, 5, 5, 5, 5, 5, 5, 5, 5}
        };

        //opposites[i] - противоположная грань к грани i
        constexpr static const int opposites[6] = {2, 3, 0, 1, 5, 4};

        std::vector<Moves> history;
        std::vector<Moves> secondaryHistory;

        int fitness_ = FitnessStates::Solved;

        //Вспомогательная функция для логического вращения сторон прилежащих граней
        void rotateAdj(const std::vector<int> &, const std::vector<std::vector<int>> &, int);

        //Расчёт собранности кубика
        int countFitness() const;

        //Оценка сборки креста
        std::vector<int> fitnessBottomCross() const;

        //Оценка собранности боковых столбцов
        std::vector<int> fitnessBars(const std::vector<int> &) const;

        //Оценка собранности целой стороны
        std::vector<int> fitnessFullSide(const std::vector<int> &) const;

        //Оценка собранности боковых пирамид
        std::vector<int> fitnessPyramid(const std::vector<int> &) const;

        //Оценка собранности всех facelet 'ов кроме верхней грани
        std::vector<int> fitnessBag(const std::vector<int> &) const;

        //Оценка собранности верхнего креста
        std::vector<int> fitnessTopCross(const std::vector<int> &) const;

        //Оценка собранности кубика без учёта верхних углов
        std::vector<int> fitnessNoTopCorners(const std::vector<int> &) const;

        std::vector<int> fitnessSolved(const std::vector<int> &) const;

    public:
        struct RandomScramble {};

        Cube() {};

        Cube(const RandomScramble &) {
            Randomize();
        }

        Cube(const std::string &);

        Cube(const Cube &);

        Cube(Cube &&) noexcept;

        Cube &PerformMove(Moves);

        Cube &PerformMoves(const std::vector<Moves> &moves) {
            for (const Moves &move : moves)
                PerformMove(move);

            return *this;
        }

        inline const int &Fitness() const { return fitness_; }

        int NaiveFitness() const;

        Cube &Randomize();

        inline const std::vector<Moves> &History() const { return history; }

        inline Cube &ClearHistory() {
            history.clear();
            return *this;
        }

        inline const std::vector<Moves> &SecondaryHistory() const { return secondaryHistory; }

        inline Cube &ClearSecondaryHistory() {
            secondaryHistory.clear();
            return *this;
        }

        inline Cube &EraseMoveAt(size_t i) {
            history.erase(history.begin() + i);
            return *this;
        }

        inline Cube &InsertMoveAt(size_t i, Moves move) {
            history.insert(history.begin() + i, move);
            return *this;
        }

        std::string ToUnwrapString() const;

        std::string ToString() const;

        Cube WithCleanHistory() const {
            Cube cube(*this);
            cube.ClearHistory();
            cube.ClearSecondaryHistory();
            return cube;
        }

        inline std::vector<Color> operator[](int i) { return unwrap.at(i); }

        inline const std::vector<Color> operator[](int i) const { return unwrap.at(i); }

        Cube &operator=(const Cube);

        bool operator==(const Cube &) const;

        bool operator!=(const Cube &rhs) const { return !(*this == rhs); }

        struct FitnessSortGreater {
            bool operator()(const Cube &lhs, const Cube &rhs) {
                return lhs.fitness_ > rhs.fitness_;
            }
        };

        struct HistorySortLess {
            bool operator()(const Cube &lhs, const Cube &rhs) {
                return lhs.History().size() < rhs.History().size();
            }
        };

        struct NotSolved {
            bool operator()(const Cube &cube) {
                return cube.fitness_ != FitnessStates::Solved;
            }
        };
    };
}

std::ostream &operator<<(std::ostream &out, const Logic::Cube &cube);

#endif //GENETICCUBE_CUBE_HPP
