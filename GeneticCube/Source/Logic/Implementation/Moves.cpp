//
// Created by Георгий Круглов on 12.05.2021.
//

#include "../Moves.h"
#include <sstream>

Moves randomMove() {
    static std::random_device device;
    static std::mt19937 mt(device());

    static std::uniform_int_distribution<int> directionPicker(0, 2);
    static std::uniform_int_distribution<int> sidePicker(0, 8);

    return Moves(directionPicker(device) * 10 + sidePicker(device));
}

std::string moveToString(const Moves &move) {
    std::string answer;

    switch (move % 10) {
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

    switch (move / 10) {
        case 1:
            answer += "'";
            break;
        case 2:
            answer += "2";
            break;
    }

    return answer;
}

std::string movesToString(const std::vector<Moves> &moves) {
    std::string answer;

    for (const Moves &move : moves) {
        answer += moveToString(move) + ' ';
    }

    return answer;
}

std::vector<Moves> movesFromString(const std::string &string) {
    std::stringstream stream(string);
    std::vector<Moves> result;

    std::string move;
    while (stream >> move) {
        int type;
        int direction = 0;

        switch (move[0]) {
            case 'U':
                type = 0;
                break;
            case 'F':
                type = 1;
                break;
            case 'D':
                type = 2;
                break;
            case 'B':
                type = 3;
                break;
            case 'L':
                type = 4;
                break;
            case 'R':
                type = 5;
                break;
            case 'M':
                type = 6;
                break;
            case 'E':
                type = 7;
                break;
            case 'S':
                type = 8;
                break;
            default:
                throw std::invalid_argument(move + " - not a valid command");
        }

        if (move.size() > 1) {
            switch (move[1]) {
                case '\'':
                    direction = 1;
                    break;
                case '2':
                    direction = 2;
                    break;
                default:
                    throw std::invalid_argument(move + " - not a valid command");
            }
        }

        result.push_back(Moves(10 * direction + type));
    }

    return result;
}