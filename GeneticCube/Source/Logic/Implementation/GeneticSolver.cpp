//
// Created by Георгий Круглов on 12.05.2021.
//

#include "../GeneticSolver.h"

#include <random>
#include <algorithm>

using namespace Logic;

GeneticSolver::GeneticSolver(const Cube &cube, std::basic_ostream<char> &out)
        : source(cube.WithCleanHistory()), scrambled(cube), out(out) {}

void GeneticSolver::initPopulation(std::vector<Cube> &population) {
    for (auto &cube : population) {
        cube = source;
        cube.PerformMove(randomMove());
        cube.PerformMove(randomMove());
    }
}

std::pair<Cube, Cube>
GeneticSolver::solve(int populationSize, int eliteSize, int maxGenerationsCount, int maxResetCount) {
    for (int iteration = 0; iteration < maxResetCount; ++iteration) {
        out << "––––––––––––––––––––\n"
            << "Iteration № " << iteration + 1 << '\n'
            << "––––––––––––––––––––\n";

        std::vector<Cube> population(populationSize);
        initPopulation(population);

        for (int generationsCount = 0; generationsCount < maxGenerationsCount; ++generationsCount) {
            std::sort(population.begin(), population.end(), std::greater());

            out << "Generation №" << generationsCount << '\n'
                << "Top " << eliteSize << " performers:\n";
            printCubes(0, eliteSize, population);

            int solutionsCount = (int) (std::find_if(population.begin(), population.end(),
                                                     [&](const Cube &cube) {
                                                         return cube.Fitness() != FitnessStates::Solved;
                                                     }) - population.begin());

            if (solutionsCount != 0) {
                out << "Found " << solutionsCount << " solutions:\n";
                printCubes(0, solutionsCount, population);
            }
            out << '\n';

            mutate(eliteSize, population);

            if (solutionsCount != 0) {
                std::sort(population.begin(), population.begin() + solutionsCount,
                          [](const Cube &lhs, const Cube &rhs) {
                              return lhs.History().size() < rhs.History().size();
                          });

                out << "Solution found on iteration №" << iteration << "! It requires "
                    << population.front().History().size() << " moves:\n"
                    << movesToString(population.front().History())
                    << '\n';

                return {scrambled, population.front()};
            }
        }
    }

    return {scrambled, source};
}

std::pair<Cube, Cube>
GeneticSolver::Solve(int populationSize, int eliteSize, int maxGenerationsCount, int maxResetCount, int maxThreadCount,
                     OptimizationType optimizationType) {
    return solve(populationSize, eliteSize, maxGenerationsCount, maxResetCount);
}

void GeneticSolver::mutate(int eliteSize, std::vector<Cube> &population) {
    std::random_device device;
    std::mt19937 mt(device());

    std::uniform_int_distribution<int> parentPicker(0, eliteSize - 1);
    std::uniform_int_distribution<int> typePicker(0, 5);

    std::uniform_int_distribution<int> permutationPicker(0, (int) permutations.size() - 1);
    std::uniform_int_distribution<int> rotationPicker(0, 1);
    std::uniform_int_distribution<int> directionPicker(0, 2);

    for (int i = eliteSize; i < population.size(); ++i) {
        if (i != population.size() - 1) {
            int parent = parentPicker(device);
            population[i] = population[parent];
        } else {
            population[i] = source;
        }

        int type = typePicker(device);

        switch (type) {
            case 1:
                population[i].PerformMoves(permutations[permutationPicker(device)]);
            case 0:
                population[i].PerformMoves(permutations[permutationPicker(device)]);
                break;
            case 2:
                population[i].PerformMoves(permutations[permutationPicker(device)]);
                population[i].PerformMoves(
                        fullRotations[3 * rotationPicker(device) + directionPicker(device)]);
            case 3:
                population[i].PerformMoves(permutations[permutationPicker(device)]);
                population[i].PerformMoves(orientations[directionPicker(device)]);
            case 4:
                population[i].PerformMoves(permutations[permutationPicker(device)]);
                population[i].PerformMoves(
                        fullRotations[3 * rotationPicker(device) + directionPicker(device)]);
                population[i].PerformMoves(orientations[directionPicker(device)]);
            case 5:
                population[i].PerformMoves(orientations[directionPicker(device)]);
                population[i].PerformMoves(permutations[permutationPicker(device)]);
                population[i].PerformMoves(
                        fullRotations[3 * rotationPicker(device) + directionPicker(device)]);
            default:
                population[i].PerformMove(randomMove());
        }
    }
}

void GeneticSolver::printCubes(const int &from, const int &upTo, const std::vector<Cube> &population) const {
    for (int i = from; i < upTo; ++i) {
        if (population[i].Fitness() != 0) {
            out << "Fitness: " << population[i].Fitness() << "\nSequence: ";
            out << "[ " << movesToString(population[i].History()) << "]\n";
        }
    }
}