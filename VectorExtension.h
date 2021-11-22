
#ifndef VECTOREXTENSION_H
#define VECTOREXTENSION_H

#include <functional>
#include <vector>

template<typename E> class VectorExtension {

  public:
    static int constexpr INDEX_NOT_FOUND = -1;

  public:
    static int findIndexOfElement(std::vector<E> &vector, E element) {
        for (int i = 0; i < vector.size(); ++i) {
            if (vector[i] == element) { return i; }
        }
        return INDEX_NOT_FOUND;
    }

  public:
    template<typename V>
    static int findIndexOfValue(
            std::vector<E *> &vector, V valueToFind,
            const std::function<bool(E &element1, V &element2)> &predicate) {
        for (long unsigned int i = 0; i < vector.size(); ++i) {
            if (predicate(*(vector[i]), valueToFind)) { return i; }
        }
        return INDEX_NOT_FOUND;
    }

  public:
    template<typename V>
    static int findIndexOfKey(
            std::vector<E *> &vector, V keyToFind,
            const std::function<bool(E &element1, V &element2)> &predicate) {
        for (long unsigned int i = 0; i < vector.size(); ++i) {
            if (predicate(*(vector[i]), keyToFind)) { return i; }
        }
        return INDEX_NOT_FOUND;
    }

  public:
    static void deleteVectorOfPointers(std::vector<E> &vector) {
        for (long unsigned int i = 0; i < vector.size(); ++i) {
            delete vector[i];
        }
    }
};


#endif // VECTOREXTENSION_H
