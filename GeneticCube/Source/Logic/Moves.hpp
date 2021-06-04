//
// Created by Георгий Круглов on 11.05.2021.
//


#ifndef GENETICCUBE_MOVES_HPP
#define GENETICCUBE_MOVES_HPP

#include <random>
#include <vector>
#include <iostream>

//@no-format
namespace Logic {
    struct Move {
        enum Direction {
            Clockwise, CounterClockwise, HalfTurn
        };
        enum Face {
            Up    , Front, Down ,
            Back  , Left , Right,
            Middle, Edge , Side ,
        };

        Direction direction;
        Face face;

        constexpr Move(Direction d = Clockwise, Face f = Up) : direction(d), face(f) {};

        bool operator==(const Move &rhs) const {
            return direction == rhs.direction && face == rhs.face;
        }

        bool operator!=(const Move &rhs) const {
            return direction != rhs.direction || face != rhs.face;
        }
    };


    namespace Moves {
        static constexpr Move U {static_cast<Move::Direction>(0), static_cast<Move::Face>(0)};
        static constexpr Move UR{static_cast<Move::Direction>(1), static_cast<Move::Face>(0)};
        static constexpr Move U2{static_cast<Move::Direction>(2), static_cast<Move::Face>(0)};

        static constexpr Move F {static_cast<Move::Direction>(0), static_cast<Move::Face>(1)};
        static constexpr Move FR{static_cast<Move::Direction>(1), static_cast<Move::Face>(1)};
        static constexpr Move F2{static_cast<Move::Direction>(2), static_cast<Move::Face>(1)};

        static constexpr Move D {static_cast<Move::Direction>(0), static_cast<Move::Face>(2)};
        static constexpr Move DR{static_cast<Move::Direction>(1), static_cast<Move::Face>(2)};
        static constexpr Move D2{static_cast<Move::Direction>(2), static_cast<Move::Face>(2)};

        static constexpr Move B {static_cast<Move::Direction>(0), static_cast<Move::Face>(3)};
        static constexpr Move BR{static_cast<Move::Direction>(1), static_cast<Move::Face>(3)};
        static constexpr Move B2{static_cast<Move::Direction>(2), static_cast<Move::Face>(3)};

        static constexpr Move L {static_cast<Move::Direction>(0), static_cast<Move::Face>(4)};
        static constexpr Move LR{static_cast<Move::Direction>(1), static_cast<Move::Face>(4)};
        static constexpr Move L2{static_cast<Move::Direction>(2), static_cast<Move::Face>(4)};

        static constexpr Move R {static_cast<Move::Direction>(0), static_cast<Move::Face>(5)};
        static constexpr Move RR{static_cast<Move::Direction>(1), static_cast<Move::Face>(5)};
        static constexpr Move R2{static_cast<Move::Direction>(2), static_cast<Move::Face>(5)};

        static constexpr Move M {static_cast<Move::Direction>(0), static_cast<Move::Face>(6)};
        static constexpr Move MR{static_cast<Move::Direction>(1), static_cast<Move::Face>(6)};
        static constexpr Move M2{static_cast<Move::Direction>(2), static_cast<Move::Face>(6)};

        static constexpr Move E {static_cast<Move::Direction>(0), static_cast<Move::Face>(7)};
        static constexpr Move ER{static_cast<Move::Direction>(1), static_cast<Move::Face>(7)};
        static constexpr Move E2{static_cast<Move::Direction>(2), static_cast<Move::Face>(7)};

        static constexpr Move S {static_cast<Move::Direction>(0), static_cast<Move::Face>(8)};
        static constexpr Move SR{static_cast<Move::Direction>(1), static_cast<Move::Face>(8)};
        static constexpr Move S2{static_cast<Move::Direction>(2), static_cast<Move::Face>(8)};
    };
}
//@format

Logic::Move randomMove();

std::string moveToString(const Logic::Move &move);

std::string movesToString(const std::vector<Logic::Move> &moves);

std::vector<Logic::Move> movesFromString(const std::string &string);

std::istream &operator>>(std::istream &, Logic::Move &);

std::ostream &operator<<(std::ostream &, const Logic::Move &);

#endif //GENETICCUBE_MOVES_HPP

