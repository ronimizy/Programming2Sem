//
// Created by Георгий Круглов on 22.02.2021.
//

#ifndef LAB3_NODE_H
#define LAB3_NODE_H

#include <vector>

struct Node {
    double x = 0, y = 0;

    explicit Node(const std::string &coordinates) {
        getCoordinatesFromString(coordinates);
    }

    double distance(Node &node) const {
        return sqrt(pow((x - node.x), 2) + pow((y - node.y), 2));
    }

    friend std::ostream &operator<<(std::ostream &lhs, Node &rhs) {
        lhs << "x: " << rhs.x << ", y: " << rhs.y;
        return lhs;
    }


private:
    void getCoordinatesFromString(const std::string &str) {
        std::pair<double, double> one = {-1, -1};
        std::string buffer;

        for (auto &c : str) {
            if (c != ',') {
                buffer += c;
            } else {
                if (one.first == -1) {
                    one.first = std::stod(buffer);
                } else {
                    one.second = std::stod(buffer);
                }
                buffer = "";
            }
        }
        if (!buffer.empty()) {
            if (one.first == -1) {
                one.first = std::stod(buffer);
            } else {
                one.second = std::stod(buffer);
            }
        }

        x = one.first;
        y = one.second;
    }
};

#endif //LAB3_NODE_H
