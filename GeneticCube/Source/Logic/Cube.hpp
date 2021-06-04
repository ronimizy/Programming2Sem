//
// Created by Георгий Круглов on 11.05.2021.
//

#ifndef GENETICCUBE_CUBE_HPP
#define GENETICCUBE_CUBE_HPP

#include <iostream>
#include <vector>
#include <array>
#include <compare>

#include "Moves.hpp"
#include "Color.hpp"
#include "FitnessStates.hpp"

namespace Logic {
    class Cube {
        //Вспомогательная функция для логического вращения сторон прилежащих граней
        void rotateAdj(const std::array<int, 4> &, const std::array<std::array<int, 3>, 6> &, int);

    protected:
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
    public:
        Cube() = default;

        explicit Cube(bool randomized) {
            if (randomized)
                Randomize();
        }

        explicit Cube(const std::string &);

        explicit Cube(const char *s) : Cube(std::string(s)) {};

        Cube(const Cube &);

        Cube(Cube &&) noexcept;

        virtual Cube &PerformMove(Move);

        Cube &PerformMoves(const std::vector<Move> &moves) {
            for (const Move &move : moves)
                PerformMove(move);

            return *this;
        }

        Cube &Randomize();

        [[nodiscard]]
        std::string ToUnwrapString() const;

        [[nodiscard]]
        std::string ToString() const;

        inline std::vector<Color> &operator[](int i) { return unwrap.at(i); }

        inline const std::vector<Color> &operator[](int i) const { return unwrap.at(i); }

        Cube &operator=(const Cube &);

        bool operator==(const Cube &) const;

        bool operator!=(const Cube &rhs) const { return !(*this == rhs); }
    };
}

std::ostream &operator<<(std::ostream &out, const Logic::Cube &cube);

#endif //GENETICCUBE_CUBE_HPP
