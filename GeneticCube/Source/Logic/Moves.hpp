//
// Created by Георгий Круглов on 11.05.2021.
//


#ifndef GENETICCUBE_MOVES_HPP
#define GENETICCUBE_MOVES_HPP

#include <random>
#include <vector>
#include <iostream>

//@no-format
enum Moves {
    Up =      0,  UpRev =     10, UpHalf =      20,
    Front =   1,  FrontRev =  11, FrontHalf =   21,
    Down =    2,  DownRev =   12,  DownHalf =   22,
    Back =    3,  BackRev =   13, BackHalf =    23,
    Left =    4,  LeftRev =   14, LeftHalf =    24,
    Right =   5,  RightRev =  15, RightHalf =   25,
    Middle =  6,  MiddleRev = 16, MiddleHalf =  26,
    Edge =    7,  EdgeRev =   17, EdgeHalf =    27,
    Side =    8,  SideRev =   18, SideHalf =    28,

    U = 0, UR = 10, U2 = 20,
    F = 1, FR = 11, F2 = 21,
    D = 2, DR = 12, D2 = 22,
    B = 3, BR = 13, B2 = 23,
    L = 4, LR = 14, L2 = 24,
    R = 5, RR = 15, R2 = 25,
    M = 6, MR = 16, M2 = 26,
    E = 7, ER = 17, E2 = 27,
    S = 8, SR = 18, S2 = 28
};
//@format

Moves randomMove();

std::string moveToString(const Moves &move);

std::string movesToString(const std::vector<Moves> &moves);

std::vector<Moves> movesFromString(const std::string &string);

std::istream &operator>>(std::istream &, Moves &);

#endif //GENETICCUBE_MOVES_HPP

