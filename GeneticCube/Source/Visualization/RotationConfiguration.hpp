//
// Created by Георгий Круглов on 18.05.2021.
//

#ifndef GENETICCUBE_ROTATIONCONFIGURATION_HPP
#define GENETICCUBE_ROTATIONCONFIGURATION_HPP

#include "../Logic/Moves.hpp"

class RotationConfiguration {
public:
    enum Orientation {
        Front = true,
        Side = false
    };
    enum Dimension {
        Horizontal = true,
        Vertical = false
    };
    enum Position {
        Close = 0,
        Middle = 1,
        Far = 2
    };
    enum Direction {
        QuarterTurn = 90,
        CounterQuarterTurn = -90,
        HalfTurn = 180,
        CounterHalfTurn = -180
    };

    Orientation orientation = Front;
    Dimension dimension = Horizontal;
    Position position = Middle;
    Direction direction = QuarterTurn;

    RotationConfiguration() noexcept {};

    RotationConfiguration(const RotationConfiguration &origin) noexcept
            : orientation(origin.orientation), dimension(origin.dimension),
              position(origin.position), direction(origin.direction) {};

    RotationConfiguration(RotationConfiguration &&origin) noexcept
            : orientation(origin.orientation), dimension(origin.dimension),
              position(origin.position), direction(origin.direction) {};

    RotationConfiguration &operator=(const RotationConfiguration &rhs) {
        if (this != &rhs) {
            orientation = rhs.orientation;
            dimension = rhs.dimension;
            position = rhs.position;
            direction = rhs.direction;
        }

        return *this;
    }


    RotationConfiguration &operator=(const Moves &rhs) {
        RotationConfiguration r(rhs);
        orientation = r.orientation;
        dimension = r.dimension;
        position = r.position;
        direction = r.direction;

        return *this;
    }

    RotationConfiguration(Moves move) {
        if (move / 10 == 0) {
            if (move % 10 == Moves::Right ||
                move % 10 == Moves::Back ||
                move % 10 == Moves::Up)
                direction = QuarterTurn;
            else
                direction = CounterQuarterTurn;
        } else if (move / 10 == 1) {
            if (move % 10 == Moves::Right ||
                move % 10 == Moves::Back ||
                move % 10 == Moves::Up)
                direction = CounterQuarterTurn;
            else
                direction = QuarterTurn;
        } else {
            direction = HalfTurn;
        }

        if (move % 10 == Moves::Up ||
            move % 10 == Moves::Edge ||
            move % 10 == Moves::Down)
            dimension = Dimension::Horizontal;
        else
            dimension = Dimension::Vertical;

        if (move % 10 == Moves::Left ||
            move % 10 == Moves::Middle ||
            move % 10 == Moves::Right)
            orientation = Orientation::Front;
        else
            orientation = Orientation::Side;

        if (move % 10 == Moves::Left ||
            move % 10 == Moves::Front ||
            move % 10 == Moves::Down)
            position = Position::Close;
        else if (move % 10 == Moves::Middle ||
                 move % 10 == Moves::Side ||
                 move % 10 == Moves::Edge)
            position = Position::Middle;
        else
            position = Position::Far;
    }

    Moves ToMove() {
        int type;
        int face;

        if (direction == QuarterTurn || direction == CounterQuarterTurn) {
            if (position != Far)
                type = (direction == QuarterTurn ? 1 : 0);
            else
                type = (direction == QuarterTurn ? 0 : 1);
        } else {
            type = 2;
        }

        if (position == Close) {
            if (dimension == Horizontal)
                face = Moves::Down;
            else if (orientation == Front)
                face = Moves::Left;
            else
                face = Moves::Front;
        } else if (position == Middle) {
            if (dimension == Horizontal)
                face = Moves::Edge;
            else if (orientation == Front)
                face = Moves::Middle;
            else
                face = Moves::Side;
        } else {
            if (dimension == Horizontal)
                face = Moves::Up;
            else if (orientation == Front)
                face = Moves::Right;
            else
                face = Moves::Back;
        }

        return Moves(10 * type + face);
    }
};

#endif //GENETICCUBE_ROTATIONCONFIGURATION_HPP
