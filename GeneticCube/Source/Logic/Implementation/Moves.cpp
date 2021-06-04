//
// Created by Георгий Круглов on 12.05.2021.
//

#include "../Moves.hpp"
#include <sstream>

using namespace Logic;

Move randomMove() {
    static std::mt19937 generator(std::random_device{}());
    static std::uniform_int_distribution<Move::Direction> directionPicker(Logic::Move::Clockwise, Logic::Move::HalfTurn);
    static std::uniform_int_distribution<Move::Face> facePicker(Logic::Move::Up, Logic::Move::Side);

    return Move(directionPicker(generator), facePicker(generator));
}

std::string moveToString(const Move &move) {
    std::string answer;

    switch (move.face) {
        case 0:
            answer += "U";
            break;
        case 1:
            answer += "F";
            break;
        case 2:
            answer += "D";
            break;
        case 3:
            answer += "B";
            break;
        case 4:
            answer += "L";
            break;
        case 5:
            answer += "R";
            break;
        case 6:
            answer += "M";
            break;
        case 7:
            answer += "E";
            break;
        case 8:
            answer += "S";
            break;
    }

    switch (move.direction) {
        case Logic::Move::Clockwise:
            break;
        case Logic::Move::CounterClockwise:
            answer += "'";
            break;
        case Logic::Move::HalfTurn:
            answer += "2";
            break;
    }

    return answer;
}

std::string movesToString(const std::vector<Move> &moves) {
    std::string answer;

    for (const Move &move : moves) {
        answer += moveToString(move) + ' ';
    }

    return answer;
}

std::vector<Move> movesFromString(const std::string &string) {
    std::stringstream stream(string);
    std::vector<Move> result;

    std::string move;
    while (stream >> move) {
        Move::Face face;
        Move::Direction direction = Logic::Move::Clockwise;

        switch (move[0]) {
            case 'U':
                face = Logic::Move::Up;
                break;
            case 'F':
                face = Logic::Move::Front;
                break;
            case 'D':
                face = Logic::Move::Down;
                break;
            case 'B':
                face = Logic::Move::Back;
                break;
            case 'L':
                face = Logic::Move::Left;
                break;
            case 'R':
                face = Logic::Move::Right;
                break;
            case 'M':
                face = Logic::Move::Middle;
                break;
            case 'E':
                face = Logic::Move::Edge;
                break;
            case 'S':
                face = Logic::Move::Side;
                break;
            default:
                throw std::invalid_argument(move + " - not a valid command");
        }

        if (move.size() > 1) {
            switch (move[1]) {
                case '\'':
                    direction = Logic::Move::CounterClockwise;
                    break;
                case '2':
                    direction = Logic::Move::HalfTurn;
                    break;
                default:
                    throw std::invalid_argument(move + " - not a valid command");
            }
        }

        result.emplace_back(direction, face);
    }

    return result;
}

std::istream &operator>>(std::istream &in, Move &move) {
    int value;
    in >> value;

    if (value / 10 > 2 || value % 10 > 8)
        throw std::invalid_argument("Invalid move was provided");

    move = Move(static_cast<Move::Direction>(value / 10), static_cast<Move::Face>(value % 10));

    return in;
}

std::ostream &operator<<(std::ostream &out, const Logic::Move &move) {
    return out << moveToString(move);
}