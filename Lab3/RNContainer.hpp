//
// Created by Георгий Круглов on 17.02.2021.
//

#ifndef LAB3_RNCONTAINER_HPP
#define LAB3_RNCONTAINER_HPP
#define SIZE 1000

namespace RNContainer {
    template<typename Key, typename Element>
    struct LinkedMapCell {
        Key first;
        Element second;

        LinkedMapCell *prev = nullptr;
        LinkedMapCell *next = nullptr;

        LinkedMapCell *before = nullptr;
        LinkedMapCell *after = nullptr;

        LinkedMapCell(Key key, Element element, LinkedMapCell *before = nullptr, LinkedMapCell *prev = nullptr)
                : first(key), second(element), prev(prev), before(before) {
            if (before) {
                before->after = this;
            }
            if (prev) {
                prev->next = this;
            }
        };

        LinkedMapCell &operator=(Element &rhs) {
            second = rhs;

            return *this;
        }
    };

    template<typename Key, typename Element>
    class LinkedMap {
        using Cell = LinkedMapCell<Key, Element>;
        using Hash = unsigned long;

        Cell *table[SIZE] = {nullptr};
        int primes[10] = {3, 5, 7, 11, 13, 17, 19, 23, 29, 31};

        Cell *firstAdded = nullptr;
        Cell *lastAdded = nullptr;

        Hash hash(Key key) {
            Hash sum = 0;
            for (size_t i = 0; i < key.size(); i++) {
                sum += primes[abs(key[i] - 'a') % 10] * primes[i % 10];
            }

            return sum % SIZE;
        }

        Cell *put(Key key, Element element) {
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

        Cell *get(Key key) {
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

        void eject(Key key) {
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

    public:
        LinkedMap() = default;

        ~LinkedMap() {
            for (; lastAdded; lastAdded = lastAdded->before) {
                delete lastAdded;
            }
        }

        Element &operator[](Key &key) {
            Cell *element = get(key);

            if (!element) {
                Cell *cell = put(key, Element());
                return cell->second;
            } else {
                return element->second;
            }
        }

        const Element &operator[](Key &key) const {
            Element &element = *this[key];

            return element;
        }

        struct Iterator {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = Cell;
            using pointer = value_type *;
            using reference = value_type &;

            explicit Iterator(pointer ptr) : ptr(ptr) {};

            reference operator*() const { return *ptr; }

            pointer operator->() { return ptr; }

            Iterator &operator++() {
                ptr = ptr->after;
                return *this;
            }

            friend bool operator==(const Iterator &lhs, const Iterator &rhs) {
                return lhs.ptr == rhs.ptr;
            }

            friend bool operator!=(const Iterator &lhs, const Iterator &rhs) {
                return lhs.ptr != rhs.ptr;
            }

        private:
            pointer ptr;
        };

        Iterator begin() { return Iterator(firstAdded); }

        Iterator end() { return Iterator(nullptr); }
    };

    template<typename Key, typename InnerKey, typename Element>
    struct MultimapCell {
        using Map = LinkedMap<InnerKey, Element>;

        Key first;
        Map *second = nullptr;

        MultimapCell *prev = nullptr;
        MultimapCell *next = nullptr;

        MultimapCell *before = nullptr;
        MultimapCell *after = nullptr;

        MultimapCell() = default;

        explicit MultimapCell(Key key, Map *value, MultimapCell *before = nullptr, MultimapCell *prev = nullptr)
                : first(key), before(before), prev(prev) {
            this->second = value;
        }

        MultimapCell &operator=(LinkedMap<InnerKey, Element> &rhs) {
            *second = rhs;

            return *this;
        }
    };

    template<typename Key, typename InnerKey, typename Element>
    class MultiMap {
        using Cell = MultimapCell<Key, InnerKey, Element>;
        using Map = LinkedMap<InnerKey, Element>;
        using Hash = unsigned long;

        Cell *table[SIZE] = {nullptr};
        int primes[10] = {3, 5, 7, 11, 13, 17, 19, 23, 29, 31};

        Cell *firstAdded = nullptr;
        Cell *lastAdded = nullptr;

        Hash hash(Key key) {
            Hash sum = 0;
            for (int i = 0; i < key.size(); i++) {
                sum += primes[abs(key[i] - 'a') % 10] * primes[i % 10];
            }

            return sum % SIZE;
        }

        Cell *put(Key key, Map *value) {
            Hash h = hash(key);

            if (!table[h]) {
                table[h] = new Cell(key, value);
                if (lastAdded) {
                    lastAdded->after = table[h];
                    table[h]->before = lastAdded;
                }
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

                if (cell->first == key) {
                    if (lastAdded) {
                        if (cell->before && cell != lastAdded) {
                            cell->before->after = cell->after;
                        }
                        if (cell->after) {
                            cell->after->before = cell->before;
                        }

                        if (cell != lastAdded) {
                            lastAdded->after = cell;
                            cell->before = lastAdded;
                            cell->after = nullptr;
                        }
                    }

                    lastAdded = cell;
                    *cell = *value;

                    return cell;
                } else {
                    cell->next = new Cell(key, value, lastAdded, cell);
                    if (lastAdded) {
                        lastAdded->after = cell->next;
                    }

                    lastAdded = cell->next;

                    return cell->next;
                }
            }
        }

        Cell *get(Key key) {
            long h = hash(key);
            if (!table[h]) {
                return nullptr;
            }

            Cell *ptr = table[h];
            while (ptr->next && ptr->first != key) {
                ptr = ptr->next;
            }

            if (ptr->first == key) {
                return ptr;
            } else {
                return nullptr;
            }
        }

    public:
        void eject(Key key, InnerKey innerKey) {
            Hash h = hash(key);
            if (table[h] == nullptr) {
                return;
            }

            Cell *ptr = table[h];
            while (ptr->next != nullptr && ptr->first != key) {
                ptr = ptr->next;
            }

            if (ptr->first == key) {
                ptr->second->eject(innerKey);
            } else {
                return;
            }
        }

        void ejectAll(Key key) {
            Hash h = hash(key);
            if (table[h] == nullptr) {
                return;
            }

            Cell *ptr = table[h];
            while (ptr->next != nullptr && ptr->first != key) {
                ptr = ptr->next;
            }

            if (ptr->first == key) {
                if (ptr->prev != nullptr) {
                    ptr->prev->next = ptr->next;
                } else {
                    table[h] = ptr->next;
                }
                if (ptr->next != nullptr) {
                    ptr->next->prev = ptr->prev;
                }

                delete ptr;
            } else {
                return;
            }
        }

        MultiMap() = default;

        ~MultiMap() {
            for (; lastAdded; lastAdded = lastAdded->before) {
                delete lastAdded;
            }
        }

        Map &operator[](Key &key) {
            Cell *map = get(key);

            if (!map) {
                Cell *cell = put(key, new Map());
                return *cell->second;
            } else {
                return *map->second;
            }
        }

        const Map &operator[](Key &key) const {
            Cell &map = *this[key];

            return map;
        }

        struct Iterator {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = Cell;
            using pointer = value_type *;
            using reference = value_type &;

            explicit Iterator(pointer ptr) : ptr(ptr) {};

            reference operator*() const { return *ptr; }

            pointer operator->() { return ptr; }

            Iterator &operator++() {
                ptr = ptr->after;
                return *this;
            }

            friend bool operator==(const Iterator &lhs, const Iterator &rhs) {
                return lhs.ptr == rhs.ptr;
            }

            friend bool operator!=(const Iterator &lhs, const Iterator &rhs) {
                return lhs.ptr != rhs.ptr;
            }

        private:
            pointer ptr;
        };

        Iterator begin() { return Iterator(firstAdded); }

        Iterator end() { return Iterator(nullptr); }
    };
}

#endif //LAB3_RNCONTAINER_HPP
