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


    RotationConfiguration &operator=(const Logic::Move &rhs) {
        RotationConfiguration r(rhs);
        orientation = r.orientation;
        dimension = r.dimension;
        position = r.position;
        direction = r.direction;

        return *this;
    }

    RotationConfiguration(Logic::Move move) {
        if (move.direction == Logic::Move::Clockwise) {
            if (move.face == Logic::Move::Right ||
                move.face == Logic::Move::Back ||
                move.face == Logic::Move::Up)
                direction = QuarterTurn;
            else
                direction = CounterQuarterTurn;
        } else if (move.direction == Logic::Move::CounterClockwise) {
            if (move.face == Logic::Move::Right ||
                move.face == Logic::Move::Back ||
                move.face == Logic::Move::Up)
                direction = CounterQuarterTurn;
            else
                direction = QuarterTurn;
        } else {
            direction = HalfTurn;
        }

        if (move.face == Logic::Move::Up ||
            move.face == Logic::Move::Edge ||
            move.face == Logic::Move::Down)
            dimension = Dimension::Horizontal;
        else
            dimension = Dimension::Vertical;

        if (move.face == Logic::Move::Left ||
            move.face == Logic::Move::Middle ||
            move.face == Logic::Move::Right)
            orientation = Orientation::Front;
        else
            orientation = Orientation::Side;

        if (move.face == Logic::Move::Left ||
            move.face == Logic::Move::Front ||
            move.face == Logic::Move::Down)
            position = Position::Close;
        else if (move.face == Logic::Move::Middle ||
                 move.face == Logic::Move::Side ||
                 move.face == Logic::Move::Edge)
            position = Position::Middle;
        else
            position = Position::Far;
    }

    Logic::Move ToMove() {
        Logic::Move::Direction type;
        Logic::Move::Face face;

        if (direction == QuarterTurn || direction == CounterQuarterTurn) {
            if (position != Far)
                type = (direction == QuarterTurn ? Logic::Move::CounterClockwise : Logic::Move::Clockwise);
            else
                type = (direction == QuarterTurn ? Logic::Move::Clockwise : Logic::Move::CounterClockwise);
        } else {
            type = Logic::Move::HalfTurn;
        }

        if (position == Close) {
            if (dimension == Horizontal)
                face = Logic::Move::Down;
            else if (orientation == Front)
                face = Logic::Move::Left;
            else
                face = Logic::Move::Front;
        } else if (position == Middle) {
            if (dimension == Horizontal)
                face = Logic::Move::Edge;
            else if (orientation == Front)
                face = Logic::Move::Middle;
            else
                face = Logic::Move::Side;
        } else {
            if (dimension == Horizontal)
                face = Logic::Move::Up;
            else if (orientation == Front)
                face = Logic::Move::Right;
            else
                face = Logic::Move::Back;
        }

        return {type, face};
    }
};

#endif //GENETICCUBE_ROTATIONCONFIGURATION_HPP
