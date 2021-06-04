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
                             LoggingMode loggingMode, std::basic_ostream<char> &out)
        : populationSize_(populationSize), eliteSize_(eliteSize), maxGenerationsCount_(maxGenerationsCount),
          maxResetCount_(maxResetCount), optimizationType_(optimizationType), loggingMode_(loggingMode), out_(out),
          threadCount_(threadCount), threadPool(threadCount) {
}

void GeneticSolver::initPopulation(std::vector<CubeIndividual> &population) const {
    for (auto &cube : population) {
        cube = source;
        cube.PerformMove(randomMove());
        cube.PerformMove(randomMove());
    }
}

CubeIndividual GeneticSolver::solve() {
    std::vector<CubeIndividual> solutions;

    for (int iteration = 0; iteration < maxResetCount_; ++iteration) {
        std::vector<CubeIndividual> population(populationSize_);

        initPopulation(population);

        for (int generationsCount = 0; generationsCount < maxGenerationsCount_; ++generationsCount) {
            if (solutionFound)
                return source;

            std::sort(population.begin(), population.end(), CubeIndividual::FitnessSortGreater());

            int solutionsCount = (int) (std::find_if(population.begin(), population.end(), CubeIndividual::NotSolved()) -
                                        population.begin());

            if (loggingMode_ == Verbose) {
                logTopPerformers(generationsCount, population);
                logSolutions(solutionsCount, population);
            } else if (loggingMode_ == Descriptive) {
                logProgressMultiThread(population.front(), generationsCount);
            }

            std::random_device device;
            std::mt19937 mt(device());

            for (unsigned int i = eliteSize_; i < populationSize_; ++i)
                mutate(population[i], population, mt);

            if (solutionsCount != 0) {
                std::sort(population.begin(), population.begin() + solutionsCount, CubeIndividual::HistorySortLess());

                if (optimizationType_ != LengthOptimized)
                    return population.front();
                else {
                    solutions.push_back(population.front());
                    break;
                }
            }
        }
    }

    std::sort(solutions.begin(), solutions.end(), CubeIndividual::HistorySortLess());

    if (solutions.empty())
        return source;
    else
        return solutions.front();
}

void GeneticSolver::SolveAsync(const CubeIndividual &cube, CubeIndividual &solution, bool &solving, bool &solved) {
    solving = true;
    solved = false;

    threadPool.Enqueue([&] {
        solution = Solve(cube);

        solved = true;
    });
}

CubeIndividual GeneticSolver::Solve(const CubeIndividual &cube) {
    scrambled = cube;
    source = cube.WithCleanHistory();

    CubeIndividual solution;

    if (loggingMode_ == Verbose) {
        solution = solve();
    } else {
        std::vector<CubeIndividual> solutions;
        std::mutex mutex;
        std::condition_variable cv;

        unsigned int done = 0;

        for (int _ = 0; _ < threadCount_; ++_)
            threadPool.Enqueue([&]() -> void {
                CubeIndividual result = solve();

                std::unique_lock<std::mutex> lock(mutex);
                if (!result.History().empty() && !solutionFound) {
                    solutions.push_back(result);
                    if (optimizationType_ != LengthOptimized) {
                        solutionFound = true;
                    }
                }

                ++done;
                cv.notify_one();
            });

        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, [&] { return done == threadCount_; });

        std::sort(solutions.begin(), solutions.end(), CubeIndividual::HistorySortLess());
        if (solutions.empty())
            solution = source;
        else
            solution = solutions.front();
    }

    solutionFound = false;

    if (solution.History().empty() && solution.Fitness() != Solved) {
        out_ << "No solution was found\n";
    } else {
        if (loggingMode_ != Silent) {
            out_ << "Found solution requires " << solution.History().size() << " moves.\n"
                 << "Optimizing...\n";
        }
        solution = optimize(solution);

        if (loggingMode_ != Silent) {
            out_ << "Source fitness was: " << scrambled.Fitness() << '\n';
            out_ << "Source scramble was:\n";
            for (const Move &move : scrambled.History())
                out_ << moveToString(move) << ' ';
            out_ << '\n';

            out_ << "Optimized solution requires " << solution.History().size() << " moves:\n";
            for (const Move &move : solution.History())
                out_ << moveToString(move) << ' ';
            out_ << '\n';
        }
    }

    return solution;
}

void GeneticSolver::mutate(CubeIndividual &cube, std::vector<CubeIndividual> &population, std::mt19937 &generator) {
    int fitnessIndex = cube.Fitness() / 100;

    std::uniform_int_distribution<int> parentPicker(0, (int) eliteSize_ - 1);
    std::uniform_int_distribution<int> typePicker(0, successfulSequences[fitnessIndex].empty() ? 5 : 6);

    std::uniform_int_distribution<int> permutationPicker(0, (int) permutations.size() - 1);
    std::uniform_int_distribution<int> rotationPicker(0, 1);
    std::uniform_int_distribution<int> directionPicker(0, 2);
    std::uniform_int_distribution<int> successfulPicker(0, (int) successfulSequences[fitnessIndex].size() - 1);

    cube = population[parentPicker(generator)];

    int type = typePicker(generator);
    int successfulPick = -1;

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
            break;
        case 3:
            cube.PerformMoves(permutations[permutationPicker(generator)]);
            cube.PerformMoves(orientations[directionPicker(generator)]);
            break;
        case 4:
            cube.PerformMoves(permutations[permutationPicker(generator)]);
            cube.PerformMoves(
                    fullRotations[3 * rotationPicker(generator) + directionPicker(generator)]);
            cube.PerformMoves(orientations[directionPicker(generator)]);
            break;
        case 5:
            cube.PerformMoves(orientations[directionPicker(generator)]);
            cube.PerformMoves(permutations[permutationPicker(generator)]);
            cube.PerformMoves(fullRotations[3 * rotationPicker(generator) + directionPicker(generator)]);
            break;
        case 6:
            successfulPick = successfulPicker(generator);
            cube.PerformMoves(*successfulSequences[fitnessIndex][successfulPick]);
            break;
        default:
            cube.PerformMove(randomMove());
    }

    if (cube.Fitness() / 100 > fitnessIndex) {
        std::unique_lock<std::mutex> lock(learningMutex);
        if (successfulPick != -1)
            ++successfulSequences[fitnessIndex][successfulPick];
        else {
            successfulSequences[fitnessIndex].emplace_back(cube.SecondaryHistory());
            cube.ClearSecondaryHistory();
        }
    }
}

void GeneticSolver::logTopPerformers(const unsigned int &generationsCount, const std::vector<CubeIndividual> &population) const {
    out_ << "Generation №" << generationsCount << '\n'
         << "Top " << eliteSize_ << " performers:\n";
    printCubes(eliteSize_, population);
}

void GeneticSolver::logSolutions(const unsigned int &solutionsCount, const std::vector<CubeIndividual> &population) const {
    out_ << "Found " << solutionsCount << " solutions:\n";
    printCubes(solutionsCount, population);
}

void GeneticSolver::logProgressMultiThread(const CubeIndividual &top, unsigned int generationNumber) {
    if (solutionFound)
        return;

    std::unique_lock<std::mutex> lock(logMutex);

    out_ << "Thread " << std::this_thread::get_id() << " top performer at generation " << generationNumber << " has ["
         << top.Fitness() << "] fitness with "
         << top.History().size() << " moves\n";
}

void GeneticSolver::printCubes(unsigned int upTo, const std::vector<CubeIndividual> &population) const {
    for (unsigned int i = 0; i < upTo; ++i) {
        if (population[i].Fitness() != 0) {
            out_ << "Fitness: " << population[i].Fitness() << "\nSequence (" << population[i].History().size() << "): ";
            out_ << "[ " << movesToString(population[i].History()) << "]\n";
        }
    }
}

struct SolutionCycle {
    std::vector<Move> sequence;
    std::vector<Move> adjustment;

    std::pair<int, int> coordinates = {-1, -1};

    SolutionCycle() = default;

    SolutionCycle(std::vector<Move> sequence_, std::vector<Move> adjustment_,
                  std::pair<unsigned int, unsigned int> coordinates_)
            : sequence(std::move(sequence_)), adjustment(std::move(adjustment_)),
              coordinates(std::move(coordinates_)) {};

    SolutionCycle(const SolutionCycle &origin) = default;

    SolutionCycle(SolutionCycle &&origin) noexcept = default;

    SolutionCycle &operator=(const SolutionCycle &) = default;

    [[nodiscard]]
    inline int eliminating() const {
        return (int) coordinates.second - (int) coordinates.first + 1 - (int) adjustment.size();
    }
};

CubeIndividual GeneticSolver::optimize(const CubeIndividual &solution) {
    CubeIndividual phaseOneCube(solution);

    for (int i = 0; i < phaseOneCube.History().size() - 1; ++i) {
        Move current = phaseOneCube.History()[i], next = phaseOneCube.History()[i + 1];

        if (current.face == next.face) {
            if (current.direction == Move::Clockwise && next.direction == Move::CounterClockwise ||
                current.direction == Move::CounterClockwise && next.direction == Move::Clockwise ||
                current.direction == Move::HalfTurn && next.direction == Move::HalfTurn) {
                phaseOneCube.EraseMoveAt(i);
                phaseOneCube.EraseMoveAt(i);
            } else if (current == next && current.direction != Move::HalfTurn) {
                phaseOneCube.EraseMoveAt(i);
                phaseOneCube.EraseMoveAt(i);
                phaseOneCube.InsertMoveAt(i, {Move::HalfTurn, current.face});
            } else if (current.direction == Move::Clockwise && next.direction == Move::HalfTurn ||
                       current.direction == Move::HalfTurn && next.direction == Move::Clockwise) {
                phaseOneCube.EraseMoveAt(i);
                phaseOneCube.EraseMoveAt(i);
                phaseOneCube.InsertMoveAt(i, {Move::CounterClockwise, current.face});
            } else if (current.direction == Move::CounterClockwise && next.direction == Move::HalfTurn ||
                       current.direction == Move::HalfTurn && next.direction == Move::CounterClockwise) {
                phaseOneCube.EraseMoveAt(i);
                phaseOneCube.EraseMoveAt(i);
                phaseOneCube.InsertMoveAt(i, {Move::Clockwise, current.face});
            }
        }
    }

    if (loggingMode_ != Silent)
        out_ << "After removing mutually compensating moves found solution requires " << phaseOneCube.History().size()
             << " moves\n";

    if (optimizationType_ == SpeedOptimized)
        return phaseOneCube;

    CubeIndividual phaseTwoCube(phaseOneCube);
    CubeIndividual state(source);
    std::vector<SolutionCycle> cycles;
    std::mutex mutex;
    std::condition_variable cv;
    unsigned int done = 0;

    for (int moveLeadToStateIndex = -1;
         moveLeadToStateIndex < (int) phaseTwoCube.History().size() - 1; ++moveLeadToStateIndex) {
        if (moveLeadToStateIndex != -1)
            state.PerformMove(phaseTwoCube.History()[moveLeadToStateIndex]);

        threadPool.Enqueue([&, moveLeadToStateIndex, state]() -> void {
            CubeIndividual modifiedState(state.WithCleanHistory());
            SolutionCycle cycle;

            for (int j = moveLeadToStateIndex + 1; j < phaseTwoCube.History().size(); ++j) {
                modifiedState.PerformMove(phaseTwoCube.History()[j]);
                if (state == modifiedState) {
                    cycle.sequence = modifiedState.History();
                    cycle.adjustment = {};
                    cycle.coordinates = {moveLeadToStateIndex + 1, j};

                    continue;
                }

                for (std::vector<Move> const &axisRotation : fullRotations) {
                    CubeIndividual rotatedState(state);
                    rotatedState.PerformMoves(axisRotation);
                    if (modifiedState == rotatedState) {
                        cycle.sequence = modifiedState.History();
                        cycle.adjustment = axisRotation;
                        cycle.coordinates = {moveLeadToStateIndex + 1, j};

                        continue;
                    }
                }

                for (std::vector<Move> const &orientation : orientations) {
                    CubeIndividual rotatedState(state);
                    rotatedState.PerformMoves(orientation);
                    if (modifiedState == rotatedState) {
                        cycle.sequence = modifiedState.History();
                        cycle.adjustment = orientation;
                        cycle.coordinates = {moveLeadToStateIndex + 1, j};

                        continue;
                    }
                }
            }

            std::unique_lock<std::mutex> lock(mutex);
            ++done;
            cv.notify_one();

            if (cycle.coordinates.second != -1 &&
                cycle.coordinates.second - cycle.coordinates.first > cycle.adjustment.size()) {
                if (loggingMode_ == Verbose) {
                    out_ << "State at position " << moveLeadToStateIndex << " repeated itself at position "
                         << cycle.coordinates.second
                         << " eliminating " << cycle.eliminating() << " moves\n";
                }
                cycles.push_back(cycle);
            }
        });
    }

    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock, [&] { return done == phaseTwoCube.History().size(); });

    if (cycles.empty())
        return phaseOneCube;

    std::sort(cycles.begin(), cycles.end(), [](const SolutionCycle &lhs, const SolutionCycle &rhs) {
        return lhs.eliminating() > rhs.eliminating();
    });

    for (int i = 0; i < cycles.size() - 1; ++i) {
        for (int j = i + 1; j < cycles.size(); ++j) {
            if (cycles[i].coordinates.first < cycles[j].coordinates.first &&
                cycles[j].coordinates.first < cycles[i].coordinates.second ||
                cycles[i].coordinates.first < cycles[j].coordinates.second &&
                cycles[j].coordinates.second < cycles[i].coordinates.second) {
                cycles.erase(cycles.begin() + j);
                --j;
            }
        }
    }

    std::sort(cycles.begin(), cycles.end(), [](const SolutionCycle &lhs, const SolutionCycle &rhs) {
        return lhs.coordinates.first < rhs.coordinates.first;
    });

    if (loggingMode_ == Verbose) {
        for (auto &cycle : cycles) {
            out_ << cycle.coordinates.first << ' ' << cycle.coordinates.second << "\n\t"
                 << movesToString(cycle.sequence)
                 << "\n\t" << movesToString(cycle.adjustment) << '\n';
        }
    }

    int offset = 0;
    for (const SolutionCycle &cycle : cycles) {
        for (unsigned int _ = cycle.coordinates.first; _ <= cycle.coordinates.second; ++_)
            phaseTwoCube.EraseMoveAt(cycle.coordinates.first - offset);
        for (const Move &move : cycle.adjustment)
            phaseTwoCube.InsertMoveAt(cycle.coordinates.first - offset, move);

        offset += cycle.eliminating();
    }

    if (phaseTwoCube.Fitness() != Solved) {
        out_ << "WARNING!!! OPTIMIZED SOLUTION APPEARS TO BE WRONG!!! (Less optimized correct solution was provided)\n";
        out_ << "Removed cycles:\n";
        for (const SolutionCycle &cycle : cycles) {
            out_ << cycle.coordinates.first << ' ' << cycle.coordinates.second << "\n\t"
                 << movesToString(cycle.sequence)
                 << "\n\t" << movesToString(cycle.adjustment) << '\n';
        }

        return phaseOneCube;
    }

    return phaseTwoCube;
}