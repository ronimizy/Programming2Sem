//
// Created by Георгий Круглов on 12.05.2021.
//

#include "../GeneticSolver.hpp"

#include <random>
#include <algorithm>
#include <utility>

using namespace Logic;

GeneticSolver::GeneticSolver(unsigned int populationSize, unsigned int eliteSize, unsigned int maxGenerationsCount,
                             unsigned int maxResetCount, unsigned int threadCount,
                             OptimizationType optimizationType,
                             bool verbose, std::basic_ostream<char> &out)
        : populationSize_(populationSize), eliteSize_(eliteSize), maxGenerationsCount_(maxGenerationsCount),
          maxResetCount_(maxResetCount), optimizationType_(optimizationType), verbose_(verbose), out_(out),
          threadCount_(threadCount), threadPool(verbose ? 0 : threadCount) {
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
            if (solutionFound)
                return source;

            std::sort(population.begin(), population.end(), Cube::FitnessSortGreater());

            int solutionsCount = (int) (std::find_if(population.begin(), population.end(), Cube::NotSolved()) -
                                        population.begin());

            if (verbose_) {
                logTopPerformers(generationsCount, population);
                logSolutions(solutionsCount, population);
            } else {
                logProgressMultiThread(population.front(), generationsCount);
            }

            std::random_device device;
            std::mt19937 mt(device());

            for (unsigned int i = eliteSize_; i < populationSize_; ++i)
                mutate(population[i], population, mt);

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

void GeneticSolver::SolveAsync(const Cube &cube, Cube &solution, bool &solving, bool &solved) {
    solving = true;
    solved = false;

    threadPool.enqueue([&] {
        solution = Solve(cube);

        solved = true;
    });
}

Cube GeneticSolver::Solve(const Cube &cube) {
    scrambled = cube;
    source = cube.WithCleanHistory();

    Cube solution;

    if (verbose_) {
        solution = solve();
    } else {
        std::vector<Cube> solutions;
        std::mutex mutex;
        std::condition_variable cv;

        unsigned int done = 0;

        for (int _ = 0; _ < threadCount_; ++_)
            threadPool.enqueue([&]() -> void {
                Cube result = solve();

                std::unique_lock<std::mutex> lock(mutex);
                if (!result.History().empty() && !solutionFound) {
                    solutions.push_back(result);
                    if (optimizationType_ == SpeedOptimized) {
                        solutionFound = true;
                    }
                }

                ++done;
                cv.notify_one();
            });

        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, [&] { return done == threadCount_; });

        std::sort(solutions.begin(), solutions.end(), Cube::HistorySortLess());
        if (solutions.empty())
            solution = source;
        else
            solution = solutions.front();
    }

    solutionFound = false;

    out_ << "Source fitness was: " << scrambled.Fitness() << '\n';
    out_ << "Source scramble was:\n";
    for (const Moves &move : scrambled.History())
        out_ << moveToString(move) << ' ';
    out_ << '\n';

    if (solution.History().empty() && solution.Fitness() != Solved) {
        out_ << "No solution was found\n";
    } else {
        out_ << "Found solution requires " << solution.History().size() << " moves.\n"
             << "Optimizing...\n";
        solution = optimize(solution);
        out_ << "Optimized solution requires " << solution.History().size() << " moves:\n";
        for (const Moves &move : solution.History())
            out_ << moveToString(move) << ' ';
        out_ << '\n';
    }

    return solution;
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

void GeneticSolver::logProgressMultiThread(const Cube &top, unsigned int generationNumber) {
    if (solutionFound)
        return;

    std::unique_lock<std::mutex> lock(logMutex);

    out_ << "Thread " << std::this_thread::get_id() << " top performer at generation " << generationNumber << " has [" << top.Fitness() << "] fitness with "
         << top.History().size() << " moves\n";
}

void GeneticSolver::printCubes(unsigned int upTo, const std::vector<Cube> &population) const {
    for (unsigned int i = 0; i < upTo; ++i) {
        if (population[i].Fitness() != 0) {
            out_ << "Fitness: " << population[i].Fitness() << "\nSequence (" << population[i].History().size() << "): ";
            out_ << "[ " << movesToString(population[i].History()) << "]\n";
        }
    }
}

struct GeneticSolver::SolutionCycle {
    std::vector<Moves> sequence;
    std::vector<Moves> adjustment;

    std::pair<unsigned int, unsigned int> coordinates = {-1, -1};

    SolutionCycle() = default;

    SolutionCycle(std::vector<Moves> sequence_, std::vector<Moves> adjustment_,
                  std::pair<unsigned int, unsigned int> coordinates_)
            : sequence(std::move(sequence_)), adjustment(std::move(adjustment_)),
              coordinates(std::move(coordinates_)) {};

    SolutionCycle(const SolutionCycle &origin) = default;

    SolutionCycle(SolutionCycle &&origin) noexcept = default;

    SolutionCycle &operator=(const SolutionCycle &) = default;

    inline int eliminating() const {
        return (int) coordinates.second - (int) coordinates.first - (int) adjustment.size();
    }
};

Cube GeneticSolver::optimize(const Cube &solution) {
    Cube cube(solution);

    for (int i = 0; i < cube.History().size() - 1; ++i) {
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
            } else if (current / 10 == 0 && next / 10 == 2 ||
                       current / 10 == 2 && next / 10 == 0) {
                cube.EraseMoveAt(i);
                cube.EraseMoveAt(i);
                cube.InsertMoveAt(i, Moves(10 + current % 10));
            } else if (current / 10 == 1 && next / 10 == 2 ||
                       current / 10 == 2 && next / 10 == 1) {
                cube.EraseMoveAt(i);
                cube.EraseMoveAt(i);
                cube.InsertMoveAt(i, Moves(current % 10));
            }
        }
    }

    Cube origin(source);
    std::vector<SolutionCycle> cycles;
    std::mutex mutex;
    std::condition_variable cv;
    unsigned int done = 0;

    for (int i = 0; i < cube.History().size(); ++i) {
        threadPool.enqueue([&, i, origin]() -> void {
            Cube iterator(origin.WithCleanHistory());
            SolutionCycle cycle;

            for (int j = i; j < cube.History().size(); ++j) {
                iterator.PerformMove(cube.History()[j]);
                if (origin == iterator) {
                    std::unique_lock<std::mutex> lock(mutex);

                    cycle.sequence = iterator.History();
                    cycle.adjustment = {};
                    cycle.coordinates = {i, j};

                    continue;
                }

                for (std::vector<Moves> const &axisRotation : fullRotations) {
                    iterator.PerformMoves(axisRotation);
                    if (origin == iterator) {
                        std::unique_lock<std::mutex> lock(mutex);

                        cycle.sequence = iterator.History();
                        cycle.adjustment = {};
                        cycle.coordinates = {i, j};

                        continue;
                    }
                }

                for (std::vector<Moves> const &axisOrientation : orientations) {
                    iterator.PerformMoves(axisOrientation);
                    if (origin == iterator) {
                        std::unique_lock<std::mutex> lock(mutex);

                        cycle.sequence = iterator.History();
                        cycle.adjustment = {};
                        cycle.coordinates = {i, j};

                        continue;
                    }
                }
            }

            std::unique_lock<std::mutex> lock(mutex);
            ++done;
            cv.notify_one();
            if (cycle.coordinates.second != -1 &&
                cycle.coordinates.second - cycle.coordinates.first > cycle.adjustment.size()) {
                out_ << "State at position " << i << "repeated itself at position " << cycle.coordinates.second
                     << " eliminating " << cycle.eliminating() << " moves\n";
                cycles.push_back(cycle);
            }
        });

        origin.PerformMove(cube.History()[i]);
    }

    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock, [&] { return done == cube.History().size(); });

    std::sort(cycles.begin(), cycles.end(), [] (const SolutionCycle &lhs, const SolutionCycle &rhs) {
        return lhs.eliminating() > rhs.eliminating();
    });

    for (int i = 0; i < cycles.size() - 1; ++i) {
        for (int j = i + 1; j < cycles.size(); ++j) {
            if (cycles[j].coordinates.first < cycles[i].coordinates.second)
                cycles.erase(cycles.begin() + j);
        }
    }

    std::sort(cycles.begin(), cycles.end(), [] (const SolutionCycle &lhs, const SolutionCycle &rhs) {
        return lhs.coordinates.first < rhs.coordinates.first;
    });

    for (auto &cycle : cycles) {
        std::cout << cycle.coordinates.first << ' ' << cycle.coordinates.second << '\n';
    }

    int offset = 0;
    for (const SolutionCycle &cycle : cycles) {
        for (unsigned int _ = cycle.coordinates.first; _ <= cycle.coordinates.second; ++_)
            cube.EraseMoveAt(cycle.coordinates.first - offset);
        for (const Moves &move : cycle.adjustment)
            cube.InsertMoveAt(cycle.coordinates.first - offset, move);

        offset += cycle.eliminating();
    }

    return cube;
}