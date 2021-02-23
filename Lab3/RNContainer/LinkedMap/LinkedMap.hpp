//
// Created by Георгий Круглов on 22.02.2021.
//

#ifndef RNLINKEDMAP
#define RNLINKEDMAP

//#include "../RNContainer.h"
#include <functional>

//Hash
template<typename Key, typename Element>
typename RNContainer::LinkedMap<Key, Element>::Hash RNContainer::LinkedMap<Key, Element>::hash(Key key) {
    return (std::hash<Key>{}(key)) % LINKED_MAP_SIZE_;
}

//Put in Map
template<typename Key, typename Element>
typename RNContainer::LinkedMap<Key, Element>::Cell *
RNContainer::LinkedMap<Key, Element>::put(Key key, Element element) {
    Hash h = hash(key);

    if (table[h] == nullptr) {
        table[h] = new Cell(key, element, lastAdded);

        if (lastAdded) {
            lastAdded->after = table[h];
        }
        table[h]->before = lastAdded;
        lastAdded = table[h];
        if (!firstAdded) {
            firstAdded = table[h];
        }

        return table[h];
    } else {
        Cell *cell = table[h];

        while (cell->next && cell->first != key) {
            cell = cell->next;
        }

        if (cell->first != key) {
            return new Cell(key, element, lastAdded, cell);
        } else {
            if (cell->before && cell != lastAdded) {
                cell->before->after = cell->after;
            }
            if (cell->after) {
                cell->after->before = cell->before;
            }

            if (lastAdded != cell) {
                if (lastAdded) {
                    lastAdded->after = cell;
                }
                cell->before = lastAdded;
                cell->after = nullptr;
            }

            lastAdded = cell;
            *cell = element;

            return cell;
        }
    }
}

//Get from Map
template<typename Key, typename Element>
typename RNContainer::LinkedMap<Key, Element>::Cell *RNContainer::LinkedMap<Key, Element>::get(Key key) {
    Hash h = hash(key);
    Cell *cell = table[h];

    if (!cell) {
        return nullptr;
    }

    while (cell->next && cell->first != key) {
        cell = cell->next;
    }

    if (cell->first == key) {
        return cell;
    } else {
        return nullptr;
    }
}

//Eject from map
template<typename Key, typename Element>
void RNContainer::LinkedMap<Key, Element>::eject(Key key) {
    Hash h = hash(key);
    if (!table[h]) {
        return;
    }

    Cell *ptr = table[h];
    while (ptr->next && ptr->first != key) {
        ptr = ptr->next;
    }

    if (ptr->first == key) {
        if (ptr->prev) {
            ptr->prev->next = ptr->next;
        } else {
            table[h] = ptr->next;
        }
        if (ptr->next) {
            ptr->next->prev = ptr->prev;
        }

        if (ptr->before) {
            ptr->before->after = ptr->after;
        }
        if (ptr->after) {
            ptr->after->before = ptr->before;
        } else {
            lastAdded = ptr->before;
        }

        delete ptr;
    }
}

//Destructor
template<typename Key, typename Element>
RNContainer::LinkedMap<Key, Element>::~LinkedMap() {
    for (; lastAdded; lastAdded = lastAdded->before) {
        delete lastAdded;
    }
}

//Subscript
template<typename Key, typename Element>
Element &RNContainer::LinkedMap<Key, Element>::operator[](Key &key) {
    Cell *element = get(key);

    if (!element) {
        Cell *cell = put(key, Element());
        return cell->second;
    } else {
        return element->second;
    }
}

//Const subscript
template<typename Key, typename Element>
const Element &RNContainer::LinkedMap<Key, Element>::operator[](Key &key) const {
    const Element &a = (*this)[key];

    return a;
}

#endif //RNLINKEDMAP