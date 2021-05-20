#include <iostream>

#include "Source/Logic/GeneticSolver.hpp"
#include "Source/Visualization/RubikApplication.hpp"

URHO3D_DEFINE_APPLICATION_MAIN(Visualization::RubikApplication)


//int main() {

//    std::cout << movesToString(Logic::Cube(true).History());

//
//    std::clock_t start = clock();
//
//    Logic::Cube cube(true);
//    Logic::GeneticSolver solver(1000, 20, 200,
//                                10, 0, Logic::SpeedOptimized, true);
//    auto solution = solver.Solve(cube);
//
//    std::cout << "\n\n\n";
//    std::cout << "Source fitness was: " << solution.first.Fitness() << '\n';
//    std::cout << "Source scramble was:\n";
//    for (const Moves &move : solution.first.History())
//        std::cout << moveToString(move) << ' ';
//    std::cout << '\n';
//
//    if (!solution.second.History().empty()) {
//        std::cout << "Solution requires " << solution.second.History().size() << " moves:\n";
//        for (const Moves &move : solution.second.History())
//            std::cout << moveToString(move) << ' ';
//    } else {
//        std::cout << "No solution was found";
//    }
//
//    std::cout << "\n\n----------------\n\n";
//    std::cout << "Program was running for " << (clock() - start) / (CLOCKS_PER_SEC * 60) << " minutes";
//    std::cout << "\n\n----------------";
//
//    return 0;
//}
