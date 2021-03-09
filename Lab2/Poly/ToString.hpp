//
//  ToString.cpp
//  Lab2
//
//  Created by Георгий Круглов on 08.03.2021.
//

#include <sstream>

template<typename T>
std::string str(std::pair<const size_t, T> power, bool isFirst) {
    std::stringstream s;
    
    if (power.second < 0) {
        if (!isFirst) {
            s << "- ";
        } else {
            s << "-";
        }
    } else {
        if (!isFirst) {
            s << "+ ";
        }
    }

    s << abs(power.second);
    if (power.first > 0) {
        s << 'x';
        if (power.first > 1) {
            s << '^' << power.first;
        }
    }

    return s.str();
}
