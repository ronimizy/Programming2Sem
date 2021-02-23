//
// Created by Георгий Круглов on 22.02.2021.
//

#ifndef RNLINKEDMAP_ITERATOR
#define RNLINKEDMAP_ITERATOR

template<typename Key, typename Element>
RNContainer::LinkedMap<Key, Element>::Iterator::Iterator(RNContainer::LinkedMap<Key, Element>::Iterator::pointer ptr)
        : ptr(ptr) {}

template<typename Key, typename Element>
typename RNContainer::LinkedMap<Key, Element>::Iterator::reference
RNContainer::LinkedMap<Key, Element>::Iterator::operator*() const { return *ptr; }

template<typename Key, typename Element>
typename RNContainer::LinkedMap<Key, Element>::Iterator::pointer
RNContainer::LinkedMap<Key, Element>::Iterator::operator->() { return ptr; }

template<typename Key, typename Element>
typename RNContainer::LinkedMap<Key, Element>::Iterator &RNContainer::LinkedMap<Key, Element>::Iterator::operator++() {
    ptr = ptr->after;
    return *this;
}

template<typename Key, typename Element>
bool operator!=(const typename RNContainer::LinkedMap<Key, Element>::Iterator &lhs,
                const typename RNContainer::LinkedMap<Key, Element>::Iterator &rhs) {
    return lhs.ptr != rhs.ptr;
}

template<typename Key, typename Element>
bool operator==(const typename RNContainer::LinkedMap<Key, Element>::Iterator &lhs,
                const typename RNContainer::LinkedMap<Key, Element>::Iterator &rhs) {
    return lhs.ptr == rhs.ptr;
}

#endif //RNLINKEDMAP_ITERATOR