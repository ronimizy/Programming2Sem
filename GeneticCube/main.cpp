#include <iostream>
#include <thread>
#include <chrono>

#include "Source/Logic/GeneticSolver.hpp"
#include "Source/Visualization/RubikApplication.hpp"

URHO3D_DEFINE_APPLICATION_MAIN(Visualization::RubikApplication)

//int main() {
//    Logic::Cube cube;
//    std::cout << cube;
//    using namespace Logic::Moves;
//    cube.PerformMove(L);
//    std::cout << cube;
//
//    for (auto move : {U, UR, U2, U2,
//                      F, FR, F2, F2,
//                      D, DR, D2, D2,
//                      B, BR, B2, B2,
//                      L, LR, L2, L2,
//                      R, RR, R2, R2}) {
//        std::cout << move << '\n';
//        cube.PerformMove(move);
//        std::cout << cube;
//    }
//
//    return 0;
//}

//int main() {
//    Logic::GeneticSolver solver(1000, 20, 200, 10,
//                                std::thread::hardware_concurrency() - 1, Logic::SpeedOptimized, Logic::Silent);
//    size_t average = 0;
//    int maxCount = 200;
//    clock_t start = clock();
//    for (int i = 1; i <= maxCount; ++i) {
//        clock_t begin = clock();
//
//        Logic::Cube cube(Logic::Cube::RandomScramble {});
//        Logic::Cube solution = solver.Solve(cube);
//
//        clock_t end = clock();
//
//        average = (average * (i - 1) + (end - begin) / CLOCKS_PER_SEC) / i;
//        std::cout << (end - start) / ((unsigned long long) CLOCKS_PER_SEC * 60 * 60) << ":" << ((end - start) / (CLOCKS_PER_SEC * 60)) % 60 << ":" << ((end - start) / CLOCKS_PER_SEC) % 60
//                    << "| Cube " << i << "/" << maxCount << " is solved in " << (end - begin) / CLOCKS_PER_SEC
//                  << " seconds, average time is " << average << " seconds - " << solution.History().size() << " moves\n";
//    }
//    std::cout << "Program was running for " << (float) (clock() - start) /  (float) (CLOCKS_PER_SEC * 60) << " minutes\n";
//
//    return 0;
//}







//int main() {
//
//    //Corner test
//    //Logic::Cube c("WWWWWWWWRGGWGGGGGGYYYYYYYYYBBBBBBBBBOOOOOOOOOGRRRRRRRR");
//
//    //Edge test
//    Logic::Cube c("WWRWWRWWYGGWGGWGGGYYYYYYYYWBBBBBBBBBOOOOOOOOORGGRRRRRR");
//
//    //Permutation test
//    //Logic::Cube c("WWWWWGWWWGGGGGWGGGYYYYYYYYYBBBBBBBBBOOOOOOOOORRRRRRRRR");
//
//    return 0;
//}

//int main() {
//    std::cout << movesToString(Logic::Cube(true).History());
//
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

//    return 0;
//}
