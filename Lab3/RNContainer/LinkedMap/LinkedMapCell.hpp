//
// Created by Георгий Круглов on 22.02.2021.
//

#ifndef RNLINKEDMAP_CELL
#define RNLINKEDMAP_CELL

template<typename Key, typename Element>
RNContainer::LinkedMap<Key, Element>::Cell::Cell(Key key,
                                                 Element element,
                                                 Cell *before,
                                                 Cell *prev)
        : first(key),
          second(element),
          prev(prev),
          before(before) {
    if (before) {
        before->after = this;
    }
    if (prev) {
        prev->next = this;
    }
};

template<typename Key, typename Element>
typename RNContainer::LinkedMap<Key, Element>::Cell &RNContainer::LinkedMap<Key, Element>::Cell::operator=(Element &rhs) {
    second = rhs;

    return *this;
}

#endif //RNLINKEDMAP_CELL