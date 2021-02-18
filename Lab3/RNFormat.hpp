//
// Created by Георгий Круглов on 17.02.2021.
//

#include <iostream>
#include <vector>

namespace RNFormat {
    void correct(std::string &buffer) {
        if (buffer == "ул.") {
            buffer = "улица";
            return;
        }
        if (buffer == "пр." || buffer == "pr.") {
            buffer = "проспект";
            return;
        }
        if (buffer == "пер.") {
            buffer = "переулок";
            return;
        }
        if (buffer == "Sovetsky") {
            buffer = "Советский";
            return;
        }
        if (buffer == "бул.") {
            buffer = "бульвар";
            return;
        }
        if (buffer == "ш.") {
            buffer = "шоссе";
            return;
        }
        if (buffer == "наб.") {
            buffer = "набережная";
            return;
        }
        if (buffer == "пл.") {
            buffer = "площадь";
            return;
        }
        if (buffer == "дор.") {
            buffer = "дорога";
            return;
        }
        if (buffer == "р.") {
            buffer = "реки";
            return;
        }
    }

    void formatStreetName(std::string &name) {
        std::string buffer;
        std::string result;

        if (name[0] == ' ') {
            name = name.substr(1);
        }

        for (auto &c : name) {
            if (c != '?') {
                if (c != '.') {
                    if (c != ' ') {
                        buffer += c;
                    } else {
                        correct(buffer);
                        result += buffer;
                        result += ' ';
                        buffer = "";
                    }
                } else {
                    buffer += c;
                    correct(buffer);
                    result += buffer;
                    buffer = "";
                }
            }
        }
        if (!buffer.empty()) {
            correct(buffer);
            result += buffer;
        }

        name = result;
    }

    std::vector<std::string> getRoutesFromString(const std::string str) {
        std::string buffer;
        std::vector<std::string> answer;

        for (const char &c : str) {
            if (c != '.' && c != ',') {
                buffer += c;
            } else {
                answer.push_back(buffer);
                buffer = "";
            }
        }
        if (!buffer.empty()) {
            answer.push_back(buffer);
        }

        return answer;
    }
}