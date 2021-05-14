#include <iostream>

#include "Source/Logic/Cube.h"
#include "Source/Logic/GeneticSolver.h"


int main() {

    Logic::Cube cube(true);
    Logic::GeneticSolver solver(cube);
    solver.Solve(10, 20, 200, 10);

    std::cout << "Source fitness was: " << cube.Fitness() << '\n';
    std::cout << "Source scramble was:\n";
    for (const Moves &move : cube.History())
        std::cout << moveToString(move) << ' ';

//    Logic::Cube cube;
//    for (const std::pair<Moves, Moves> &move :
//            {std::pair{U, UR}, {UR, U}, {U2, U2},
//             {F, FR}, {FR, F}, {F2, F2},
//             {D, DR}, {DR, D}, {D2, D2},
//             {B, BR}, {BR, B}, {B2, B2},
//             {L, LR}, {LR, L}, {L2, L2},
//             {R, RR}, {RR, R}, {R2, R2},
//             {M, MR}, {MR, M}, {M2, M2},
//             {E, ER}, {ER, E}, {E2, E2},
//             {S, SR}, {SR, S}, {S2, S2}}) {
//        std::cout << moveToString(move.first);
//        cube.PerformMove(move.first);
//        std::cout << cube;
//        cube.PerformMove(move.second);
//    }

//auto scrambleMoves = movesFromString(
//        "B' R' E2 L2 S' M' R2 L2 B' E2 S' E' E2 U' B2 M' E2 F2 U B' U M2 L B' M' L B S' D' U E U F' U' R' E2 E M2 B2 R E M2 S2 R' F M D M' U R2 E' E F2 U M R' B E2 B B F2 R U F R2 E M B2 E' D2 M2 B' R D2 E B' F L' S2 D2 S' S' B B2 M D' F S2 B2 B B S' L2 F E' U' S2 U' S S' L2 R' F D R' B2 S L2 E F' D E2 S' M' B2 U2 S' R2 U2 F");
//auto solutionMoves = movesFromString(
//        "U' L' F R B L U L' U B' R' F' L' U' L U' U' E D F S B' F2 S2 B2 R' U L' U2 R U' L R' U L' U2 R U' L U' L2 M2 R2 R2 U' B2 D2 L' F2 D2 B2 R' U' F2 S2 B2 U2 R U2 R' F2 L F' L' U2 L' U2 L F' L' M' R F S B' F2 S2 B2 L' U2 L R' F2 R L M R' R2");
//
//    Logic::Cube cube;
//    std::cout << "----\n" << cube << "----\n";
//
//    for (const Moves &move : scrambleMoves) {
//        std::cout << moveToString(move) << ' ';
//        cube.PerformMove(move);
//        std::cout << cube;
//    }
//    std::cout << "--------------------------------------------------\n";
//
//    std::cout << "----\n" << cube << "----\n";
//
//    for (const Moves &move : solutionMoves) {
//        std::cout << moveToString(move) << ' ';
//        cube.PerformMove(move);
//        std::cout << cube;
//        int fitness = cube.Fitness();
//        if (fitness > 0) {
//            std::cout << '\n';
//            std::cout << fitness << '\n';
//            break;
//        }
//    }
//
//
//    int stop = 1;

    return 0;
}
