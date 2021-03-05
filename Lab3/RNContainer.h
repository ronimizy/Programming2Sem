//
// Created by Георгий Круглов on 17.02.2021.
//

#ifndef LAB3_RNCONTAINER_H
#define LAB3_RNCONTAINER_H
#define LINKED_MAP_SIZE_ 1000

#include <iostream>

namespace RNContainer {
    template<typename Key, typename Element>
    class LinkedMap {
        struct Cell {
            Key first;
            Element second;

            Cell *prev = nullptr;
            Cell *next = nullptr;

            Cell *before = nullptr;
            Cell *after = nullptr;

            Cell(Key key,
                 Element element,
                 Cell *before = nullptr,
                 Cell *prev = nullptr);

            Cell &operator=(Element &rhs);
        };


        using Hash = size_t;


        Cell *table[LINKED_MAP_SIZE_] = {nullptr};

        Cell *firstAdded = nullptr;
        Cell *lastAdded = nullptr;

        Hash hash(Key key);

        Cell *put(Key key, Element element);

        Cell *get(Key key);

        void eject(Key key);

    public:
        LinkedMap() = default;

        ~LinkedMap();

        Element &operator[](const Key &key);

        const Element &operator[](const Key &key) const;

        struct Iterator {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = Cell;
            using pointer = value_type *;
            using reference = value_type &;

            explicit Iterator(pointer ptr);

            reference operator*() const;

            pointer operator->();

            Iterator &operator++();

            bool operator!=(const Iterator &rhs);

        private:
            pointer ptr;
        };

        Iterator begin() { return Iterator(firstAdded); }

        Iterator end() { return Iterator(nullptr); }
    };
}


#include "RNContainer/LinkedMap/Iterator.hpp"
#include "RNContainer/LinkedMap/LinkedMap.hpp"
#include "RNContainer/LinkedMap/LinkedMapCell.hpp"

//#include "RNContainer/Instances.cpp"

#endif //LAB3_RNCONTAINER_H
