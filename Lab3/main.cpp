//
//  main.cpp
//  Lab3
//
//  Created by Георгий Круглов on 10.02.2021.
//

#include <iostream>
#include <utility>
#include <cmath>
#include <limits>

#include "XML/pugixml.hpp"

#include "RNFormat.hpp"
#include "RNContainer.h"
#include "Node.h"


int main() {

    pugi::xml_document document;
    pugi::xml_parse_result result = document.load_file("../data.xml");

    if (result.status) {
        std::cout << "File not loaded, description: " << result.description() << std::endl;
        exit(0);
    }


    std::cout << "\n\n\n\n";

    RNContainer::LinkedMap<std::string, int> streets;
    RNContainer::LinkedMap<std::string, RNContainer::LinkedMap<std::string, int>> stops;
    RNContainer::LinkedMap<std::string, RNContainer::LinkedMap<std::string, std::vector<Node>>> lengths;

    pugi::xml_node set = document.child("dataset");

    //Парсинг
    for (pugi::xml_node station = set.child("transport_station"); station; station = station.next_sibling()) {
        //Создание узла

        Node node(station.child("coordinates").first_child().value());
        std::string type = station.child("type_of_vehicle").first_child().value();


        //Считывание названий улиц
        std::vector<std::string> s;
        std::string str = station.child("location").first_child().value();

        //Разбиение на отдельные улицы
        std::string buffer;
        for (auto &c : str) {
            if (c != ',') {
                buffer += c;
            } else {
                RNFormat::formatStreetName(buffer);
                s.push_back(buffer);
                buffer = "";
            }
        }
        if (!buffer.empty()) {
            RNFormat::formatStreetName(buffer);
            s.push_back(buffer);
        }

        for (auto &location : s) {
            streets[location]++;
        }


        for (auto &route : RNFormat::getRoutesFromString(station.child("routes").first_child().value())) {
            stops[type][route]++;
            lengths[type][route].push_back(node);
        }
    }


    std::cout << "|–––––––––––––––––––––––––––––––|\n";
    std::cout << "Маршруты с самым большим числом \nостановок для видов транспорта:\n";
    for (auto &type : stops) {
        std::pair<std::string, int> top1;

        for (auto &route : type.second) {
            if (route.second > top1.second) {
                top1 = {route.first, route.second};
            }
        }

        std::cout << "\t" << type.first << ": " << top1.first << " – " << top1.second << '\n';
    }


    std::cout << ">–––––––––––––––––––––––––––––––<\n";
    std::cout << "Самые длинные маршруты для видов транспорта:\n";
    for (auto &type : lengths) {
        std::pair<std::string, double> top1 = {"", 0};

        for (auto &route : type.second) {
            auto &nodes = route.second;
            double routeLength = 0;

            for (size_t i = 0; i < nodes.size() - 1; i++) {
                double distance = std::numeric_limits<double>::max();

                for (size_t j = i + 1; j < nodes.size(); j++) {
                    distance = fmin(distance, nodes[i].distance(nodes[j]));
                }

                routeLength += distance;
            }

            if (routeLength > top1.second) {
                top1 = {route.first, routeLength};
            }
        }

        std::cout << "\t" << type.first << ": " << top1.first << " – " << top1.second << '\n';
    }


    std::cout << ">–––––––––––––––––––––––––––––––<\n";
    std::pair<std::string, int> top1;
    for (auto &street : streets) {
        if (street.second > top1.second) {
            top1 = {street.first, street.second};
        }
    }
    std::cout << "Улица с наибольшим числом остановок: \n\t" << top1.first << " – " << top1.second;


    std::cout << "\n|–––––––––––––––––––––––––––––––|";
    std::cout << "\n\n\n\n";

    return 0;
}
