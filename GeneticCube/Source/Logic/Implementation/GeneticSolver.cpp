//
// Created by Георгий Круглов on 12.05.2021.
//

#include "../GeneticSolver.hpp"

#include <random>
#include <algorithm>
#include <thread>

using namespace Logic;

GeneticSolver::GeneticSolver(unsigned int populationSize, unsigned int eliteSize, unsigned int maxGenerationsCount,
                             unsigned int maxResetCount, unsigned int threadLimit,
                             OptimizationType optimizationType,
                             bool verbose, std::basic_ostream<char> &out)
        : populationSize_(populationSize), eliteSize_(eliteSize), maxGenerationsCount_(maxGenerationsCount),
          maxResetCount_(maxResetCount), optimizationType_(optimizationType), verbose_(verbose), out_(out) {
    int availableThreadCount = (int) std::thread::hardware_concurrency();

    if (threadLimit > availableThreadCount || threadLimit == 0)
        threadLimit = availableThreadCount;

    threadCount_ = threadLimit;
}

void GeneticSolver::initPopulation(std::vector<Cube> &population) const {
    for (auto &cube : population) {
        cube = source;
        cube.PerformMove(randomMove());
        cube.PerformMove(randomMove());
    }
}

Cube GeneticSolver::solve() {
    std::vector<Cube> solutions;

    for (int iteration = 0; iteration < maxResetCount_; ++iteration) {
        std::vector<Cube> population(populationSize_);

        initPopulation(population);

        for (int generationsCount = 0; generationsCount < maxGenerationsCount_; ++generationsCount) {
            std::sort(population.begin(), population.end(), std::greater());

            int solutionsCount = (int) (std::find_if(population.begin(), population.end(), Cube::NotSolved()) -
                                        population.begin());

            if (verbose_) {
                logTopPerformers(generationsCount, population);
                logSolutions(solutionsCount, population);

                std::random_device device;
                std::mt19937 mt(device());

                std::vector<std::thread> threads;
                threads.reserve(threadCount_);

                unsigned int mutatedIndex = populationSize_;

                std::mutex mutex;

                for (int _ = 0; _ < threadCount_; ++_) {
                    threads.emplace_back([&] {
                        while (true) {
                            mutex.lock();

                            if (mutatedIndex > eliteSize_)
                                mutateMultiThread(population[--mutatedIndex], population, mt, mutex);
                            else {
                                mutex.unlock();
                                break;
                            }
                        }
                    });
                }

                for (std::thread &thread : threads)
                    thread.join();

            } else {
                if (solutionFound)
                    return source;

                logProgressMultiThread(population);

                std::random_device device;
                std::mt19937 mt(device());

                for (unsigned int i = eliteSize_; i < populationSize_; ++i)
                    mutate(population[i], population, mt);
            }

            if (solutionsCount != 0) {
                std::sort(population.begin(), population.begin() + solutionsCount, Cube::HistorySortLess());

                if (optimizationType_ == SpeedOptimized)
                    return population.front();
                else {
                    solutions.push_back(population.front());
                    break;
                }
            }
        }
    }

    std::sort(solutions.begin(), solutions.end(), Cube::HistorySortLess());

    if (solutions.empty())
        return source;
    else
        return solutions.front();
}

Cube GeneticSolver::Solve(const Cube &cube) {
    scrambled = cube;
    source = cube.WithCleanHistory();

    Cube solution;

    if (verbose_) {
        solution = solve();
    } else {
        std::vector<Cube> solutions;
        std::vector<std::thread> threads;
        threads.reserve(threadCount_);

        std::mutex mutex;

        for (int _ = 0; _ < threadCount_; ++_)
            threads.emplace_back([&] {
                Cube result = solve();

                if (!result.History().empty() && !solutionFound) {
                    mutex.lock();
                    solutions.push_back(result);
                    if (optimizationType_ == SpeedOptimized) {
                        solutionFound = true;
                    }
                    mutex.unlock();
                }
            });

        for (std::thread &thread : threads)
            thread.join();

        std::sort(solutions.begin(), solutions.end(), Cube::HistorySortLess());
        if (solutions.empty())
            solution = source;
        else
            solution = solutions.front();
    }

    std::cout << "Source fitness was: " << scrambled.Fitness() << '\n';
    std::cout << "Source scramble was:\n";
    for (const Moves &move : scrambled.History())
        std::cout << moveToString(move) << ' ';
    std::cout << '\n';

    if (solution.History().empty()) {
        std::cout << "No solution was found";
    } else {
        std::cout << "Found solution requires " << solution.History().size() << " moves.\n"
                  << "Optimizing...\n";
        solution = optimize(solution);
        std::cout << "Optimized solution requires " << solution.History().size() << " moves:\n";
        for (const Moves &move : solution.History())
            std::cout << moveToString(move) << ' ';
    }

    return solution;
}

void GeneticSolver::mutateMultiThread(Cube &cube, std::vector<Cube> &population, std::mt19937 &generator,
                                      std::mutex &mutex) const {
    mutex.unlock();
    mutate(cube, population, generator);
}

void GeneticSolver::mutate(Cube &cube, std::vector<Cube> &population, std::mt19937 &generator) const {
    std::uniform_int_distribution<int> parentPicker(0, (int) eliteSize_ - 1);
    std::uniform_int_distribution<int> typePicker(0, 5);

    std::uniform_int_distribution<int> permutationPicker(0, (int) permutations.size() - 1);
    std::uniform_int_distribution<int> rotationPicker(0, 1);
    std::uniform_int_distribution<int> directionPicker(0, 2);

    cube = population[parentPicker(generator)];

    int type = typePicker(generator);

    switch (type) {
        case 1:
            cube.PerformMoves(permutations[permutationPicker(generator)]);
        case 0:
            cube.PerformMoves(permutations[permutationPicker(generator)]);
            break;
        case 2:
            cube.PerformMoves(permutations[permutationPicker(generator)]);
            cube.PerformMoves(
                    fullRotations[3 * rotationPicker(generator) + directionPicker(generator)]);
        case 3:
            cube.PerformMoves(permutations[permutationPicker(generator)]);
            cube.PerformMoves(orientations[directionPicker(generator)]);
        case 4:
            cube.PerformMoves(permutations[permutationPicker(generator)]);
            cube.PerformMoves(
                    fullRotations[3 * rotationPicker(generator) + directionPicker(generator)]);
            cube.PerformMoves(orientations[directionPicker(generator)]);
        case 5:
            cube.PerformMoves(orientations[directionPicker(generator)]);
            cube.PerformMoves(permutations[permutationPicker(generator)]);
            cube.PerformMoves(fullRotations[3 * rotationPicker(generator) + directionPicker(generator)]);
        default:
            cube.PerformMove(randomMove());
    }
}

void GeneticSolver::logTopPerformers(const unsigned int &generationsCount, const std::vector<Cube> &population) const {
    out_ << "Generation №" << generationsCount << '\n'
         << "Top " << eliteSize_ << " performers:\n";
    printCubes(eliteSize_, population);
}

void GeneticSolver::logSolutions(const unsigned int &solutionsCount, const std::vector<Cube> &population) const {
    out_ << "Found " << solutionsCount << " solutions:\n";
    printCubes(solutionsCount, population);
}

void GeneticSolver::logProgressMultiThread(const std::vector<Cube> &population) {
    std::unique_lock<std::mutex> lock(logMutex);

    out_ << "Thread " << std::this_thread::get_id() << " top performer has " << population.front().Fitness() << " with "
         << population.front().History().size() << " moves\n";
}

void GeneticSolver::printCubes(unsigned int upTo, const std::vector<Cube> &population) const {
    for (unsigned int i = 0; i < upTo; ++i) {
        if (population[i].Fitness() != 0) {
            out_ << "Fitness: " << population[i].Fitness() << "\nSequence (" << population[i].History().size() << "): ";
            out_ << "[ " << movesToString(population[i].History()) << "]\n";
        }
    }
}

Cube GeneticSolver::optimize(const Cube &solution) const {
    Cube cube = solution;

    int i = 0;
    while (i < cube.History().size() - 1) {
        Moves current = cube.History()[i], next = cube.History()[i + 1];

        if (current % 10 == next % 10) {
            if (current / 10 == 0 && next / 10 == 1 ||
                current / 10 == 1 && next / 10 == 0 ||
                current / 10 == 2 && next / 10 == 2) {
                cube.EraseMoveAt(i);
                cube.EraseMoveAt(i);
            } else if (current == next && current / 10 != 2) {
                cube.EraseMoveAt(i);
                cube.EraseMoveAt(i);
                cube.InsertMoveAt(i, Moves(20 + current % 10));
            }
        }

        ++i;
    }

    return cube;
}