//
// Created by Георгий Круглов on 22.05.2021.
//

#ifndef GENETICCUBE_SUCCESSFULSEQUENCES_HPP
#define GENETICCUBE_SUCCESSFULSEQUENCES_HPP

#include <vector>
#include <fstream>
#include <compare>

#include "Moves.hpp"

namespace Logic {
    class SuccessfulSequences {
        struct Sequence {
            std::vector<Moves> seq;
            size_t priority = 0;

            Sequence() {};

            Sequence(const std::vector<Moves> &vector) : seq(vector) {};

            Sequence &operator++() {
                ++priority;
                return *this;
            }

            const std::vector<Moves> operator*() { return seq; }

            Moves &operator[](int i) { return seq.at(i); }

            const Moves &operator[](int i) const { return seq.at(i); }

            auto operator<=>(const Sequence &rhs) const { return priority <=> rhs.priority; }
        };

        std::vector<std::vector<Sequence>> tensor;

        constexpr static const char fileName[] = "GENETIC_CUBE.slns";
        static const int stateCount = 8;
        static const int maxCount = 2000;

    public:

        SuccessfulSequences() : tensor(stateCount) {
            std::fstream file(fileName, std::ios::in);

            if (file.is_open()) {
                std::clog << "Loading successful sequences\n";

                for (int state = 0; state < stateCount; ++state) {
                    int count;
                    file >> count;
                    tensor[state].resize(count);

                    for (int sequence = 0; sequence < count; ++sequence) {
                        int size;
                        file >> size >> tensor[state][sequence].priority;
                        tensor[state][sequence].seq.resize(size);

                        for (int i = 0; i < size; ++i)
                            file >> tensor[state][sequence][i];
                    }
                }

                file.close();

                std::clog << "Successful sequences loaded\n";
                for (int i = 0; i < stateCount; ++i) {
                    std::clog << "\t[" << i << "] - (" << tensor[i].size() << ") max priority - " << tensor[i].front().priority << "\n";
                }
            } else {
                std::clog << "Successful sequences file created\n";
            }
        }

        ~SuccessfulSequences() {
            std::clog << "Saving successful sequences\n";

            for (auto &state : tensor) {
                std::sort(state.begin(), state.end(), std::greater());

                if (state.size() > maxCount)
                    state.resize(maxCount);
            }

            std::fstream file(fileName, std::ios::out);

            for (int state = 0; state < stateCount; ++state) {
                file << tensor[state].size() << ' ';
                for (int sequence = 0; sequence < tensor[state].size(); ++sequence) {
                    file << tensor[state][sequence].seq.size() << ' ' << tensor[state][sequence].priority << ' ';

                    for (const auto move : tensor[state][sequence].seq)
                        file << move << ' ';
                }
            }

            file.close();
            std::clog << "Successful sequences saved\n";
        }

        std::vector<Sequence> &operator[](unsigned int i) { return tensor.at(i); }
    };
}


#endif //GENETICCUBE_SUCCESSFULSEQUENCES_HPP
