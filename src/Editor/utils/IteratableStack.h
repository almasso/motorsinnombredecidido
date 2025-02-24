//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_ITERATABLESTACK_H
#define MOTORSINNOMBREDECIDIDO_ITERATABLESTACK_H

#include <list>
#include <unordered_map>

template <class T>
class IteratableStack {
public:
    void push(T* element) {
        _stack.push_front(element);
        _lookup[element] = _stack.begin();
    }

    T* pop() {
        if(_stack.empty()) return nullptr;
        T* elem = _stack.front();
        _stack.pop_front();
        _lookup.erase(elem);
        return elem;
    }

    bool remove(T* element) {
        auto it = _lookup.find(element);
        if(it == _lookup.end()) return false;
        _stack.erase(it->second);
        _lookup.erase(it);
        return true;
    }

    bool empty() const {
        return _stack.empty();
    }

    typename std::list<T*>::iterator begin() {
        return _stack.begin();
    }

    typename std::list<T*>::iterator end() {
        return _stack.end();
    }

private:
    std::list<T*> _stack;
    std::unordered_map<T*, typename std::list<T*>::iterator> _lookup;
};


#endif //MOTORSINNOMBREDECIDIDO_ITERATABLESTACK_H
