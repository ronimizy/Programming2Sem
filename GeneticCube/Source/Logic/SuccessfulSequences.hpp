//
// Created by Георгий Круглов on 22.05.2021.
//

#ifndef GENETICCUBE_SUCCESSFULSEQUENCES_HPP
#define GENETICCUBE_SUCCESSFULSEQUENCES_HPP

#include <vector>
#include <fstream>

#include "Moves.hpp"

namespace Logic {
    class SuccessfulSequences {
        std::vector<std::vector<std::vector<Moves>>> tensor;

        constexpr static const char fileName[] = "GENETIC_CUBE.slns";
        static const int stateCount = 8;
        static const int maxCount = 5000;

    public:
        SuccessfulSequences() : tensor(stateCount) {
            std::fstream file(fileName, std::ios::in);

            if (file.is_open()) {
                std::clog << "Loading successful sequences\n";

                for (int i = 0; i < stateCount; ++i) {
                    int sequencesCount;
                    file >> sequencesCount;

                    tensor[i].resize(sequencesCount);

                    for (int j = 0; j < sequencesCount; ++j) {
                        int sequenceLength;
                        file >> sequenceLength;

                        tensor[i][j].resize(sequenceLength);

                        for (int k = 0; k < sequenceLength; ++k)
                            file >> tensor[i][j][k];
                    }
                }

                file.close();

                std::clog << "Successful sequences loaded\n";
            }
        }

        ~SuccessfulSequences() {
            std::clog << "Saving successful sequences\n";

            for (auto state : tensor) {
                std::sort(state.begin(), state.end(), [](const std::vector<Moves> &lhs, const std::vector<Moves> &rhs) {
                    return lhs.size() < rhs.size();
                });

                if (state.size() > maxCount)
                    state.resize(maxCount);
            }

            std::fstream file(fileName, std::ios::out);

            for (int i = 0; i < stateCount; ++i) {
                file << tensor[i].size() << "\n";

                for (int j = 0; j < tensor[i].size(); ++j) {
                    file << tensor[i][j].size() << '\t';

                    for (int k = 0; k < tensor[i][j].size(); ++k)
                        file << tensor[i][j][k] << ' ';
                    file << '\n';
                }
            }

            file.close();
            std::clog << "Successful sequences saved\n";
        }

        std::vector<std::vector<Moves>> &operator[](unsigned int i) { return tensor.at(i); }
    };
}


#endif //GENETICCUBE_SUCCESSFULSEQUENCES_HPP
