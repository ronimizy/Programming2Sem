//
//  main.cpp
//  Lab3
//
//  Created by Георгий Круглов on 10.02.2021.
//

#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include "XML/pugixml.hpp"

struct Node {
    double x, y;
    std::vector<std::string> routes;
    
    Node(std::string crd, std::string str) :x(0), y(0) {
        getCoordinatesFromString(std::move(crd));
        getRoutesFromString(std::move(str));
    }
    
    friend std::ostream& operator<<(std::ostream &lhs, Node &rhs) {
        
        lhs << "x: " << rhs.x << ", y: " << rhs.y;
        
        return lhs;
    }
    
private:
    void getCoordinatesFromString(std::string str) {
        std::vector<double> one;
        
        int i = 0;
        std::string buffer;
        
        while (char c = str[i++]) {
            if (c != ',') {
                buffer += c;
            } else {
                one.push_back(std::stod(buffer));
                buffer = "";
            }
        }
        if (!buffer.empty()) {
            one.push_back(std::stod(buffer));
        }
        
        x = one[0];
        y = one[1];
    }
    void getRoutesFromString(std::string str) {
        int i = 0;
        std::string buffer;
        
        while (char c = str[i++]) {
            if (c != '.' && c != ',') {
                buffer += c;
            } else {
                routes.push_back(buffer);
                buffer = "";
            }
        }
        if (!buffer.empty()) {
            routes.push_back(buffer);
        }
    }
};

int main() {
    
    pugi::xml_document document;
    pugi::xml_parse_result result = document.load_file("data.xml");
    
    
    if (result.status != 0) {
        std::cout << "File not loaded, description: " << result.description() << std::endl;
        exit(0);
    }
    
    
    std::vector<Node> nodes;
    std::map<std::string, std::vector<Node>> streets;
    pugi::xml_node set = document.child("dataset");
    
    for (pugi::xml_node station = set.child("transport_station"); station; station = station.next_sibling()) {
        nodes.emplace_back(station.child("coordinates").first_child().value(),
                           station.child("routes").first_child().value());
        
        std::vector<std::string> s;
        std::string str = station.child("location").first_child().value();
        
        int i = 0;
        std::string buffer;
        while (char c = str[i++]) {
            if (c != ',') {
                buffer += c;
            } else {
                s.push_back(buffer);
                buffer = "";
                i++;
            }
        }
        if (!buffer.empty()) {
            s.push_back(buffer);
        }
        
        for (auto &location : s) {
//            if (streets.count(location)) {
//                
//            }
            streets[location].push_back(nodes.back());
        }
    }
    
    for (auto & street : streets) {
        for (auto i : street.second) {
            std::cout << i << '\n';
        }
    }
    
    
    return 0;
}
