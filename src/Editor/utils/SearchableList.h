//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_SEARCHABLELIST_H
#define MOTORSINNOMBREDECIDIDO_SEARCHABLELIST_H

#include <list>
#include <unordered_map>

/**
 * @~english
 * Provides an implementation to a specific ADT (abstract data type) that defines a list with element search and deletion
 * at quasi-constant time.
 *
 * @tparam T Whatever class
 *
 * @~spanish
 * Ofrece una implementación de un TAD (tipo abstracto de datos) que define una lista con búsqueda y borrado de ele,entos
 * en tiempo casi constante.
 *
 * @tparam T Cualquier clase
 */
template <class T>
class SearchableList {
public:
    /**
     * @~english
     * @brief Pushes an element to the front of the list.
     *
     * @param element Element
     *
     * @~spanish
     * @brief Introduce un elemento al principio de la lista.
     *
     * @param element Elemento
     */
    void push_front(T element) {
        _list.push_front(element);
        _lookup[element] = _list.begin();
    }

    /**
     * @~english
     * @brief Pushes an element to the back of the list.
     *
     * @param element Element
     *
     * @~spanish
     * @brief Introduce un elemento al final de la lista.
     *
     * @param element Elemento
     */
    void push_back(T element) {
        _list.push_back(element);
        _lookup[element] = --_list.end();
    }

    /**
     * @~english
     * @brief Pops and returns the element at the front of the list.
     *
     * @return Element at the front of the list.
     *
     * @~spanish
     * @brief Quita y devuelve el elemento al frente de la lista.
     *
     * @return Elemento al frente de la lista
     */
    T pop_front() {
        if(_list.empty()) return nullptr;
        T* elem = _list.front();
        _list.pop_front();
        _lookup.erase(elem);
        return elem;
    }

    /**
     * @~english
     * @brief Pops and returns the element at the back of the list.
     *
     * @return Element at the back of the list.
     *
     * @~spanish
     * @brief Quita y devuelve el elemento al final de la lista.
     *
     * @return Elemento al final de la lista
     */
    T pop_back() {
        if(_list.empty()) return nullptr;
        T* elem = --_list.end();
        _list.pop_back();
        _lookup.erase(elem);
        return elem;
    }

    /**
     * @~english
     * @brief Removes an element from the list at whatever position.
     *
     * @param element Element to remove
     *
     * @return \c true if the element removal was successful, \c false if no element was found and couldn't remove.
     *
     * @~spanish
     * @brief Elimina un elemento de la lista en cualquier posición.
     *
     * @param element Elemento a eliminar
     *
     * @return \c true si el borrado del elemento fue correcto, \c false si no se encontró el elemento y no se puedo eliminar.
     */
    bool erase(T element) {
        typename std::unordered_map<T, typename std::list<T>::iterator>::iterator it = _lookup.find(element);
        if(it == _lookup.end()) return false;
        _list.erase(it->second);
        _lookup.erase(it);
        return true;
    }

    [[nodiscard]] uint32_t size() const {
        return _list.size();
    }

    /**
     * @~english
     * @brief Returns whether the list is empty or not.
     *
     * @return \c true if the list is empty, \c false if the list contains at least one element.
     *
     * @~spanish
     * @brief Devuelve si la lista está o no vacía.
     *
     * @return \c true si la lista está vacía, \c false si la lista contiene al menos un elemento.
     */
    [[nodiscard]] bool empty() const {
        return _list.empty();
    }

    /**
     * @~english
     * @brief Returns an iterator to the first element of the list.
     *
     * @return Iterator to the first element of the list.
     *
     * @~spanish
     * @brief Devuelve un iterador al primer elemento de la lista.
     *
     * @return Iterador al primer elemento de la lista.
     */
    typename std::list<T>::const_iterator begin() const {
        return _list.begin();
    }
    typename std::list<T>::iterator begin() {
        return _list.begin();
    }

    /**
     * @~english
     * @brief Returns an iterator to the element before the end of the list.
     *
     * @return Iterator to the element before the end of the list.
     *
     * @~spanish
     * @brief Devuelve un iterador al elemento anterior al final de la lista.
     *
     * @return Iterador al elemento anterior al final de la lista.
     */
    typename std::list<T>::const_iterator end() const {
        return _list.end();
    }
    typename std::list<T>::iterator end()  {
        return _list.end();
    }

    /**
     * @~english
     * @brief Returns you an iterator to a certain element on the list.
     *
     * @param element Element
     *
     * @return Iterator to \c element
     *
     * @~spanish
     * @brief Devuelve un iterador a un cierto elemento en la lista.
     *
     * @param element Elemento
     *
     * @return Iterador a \c element
     */
    typename std::list<T>::const_iterator find(T element) const {
        typename std::unordered_map<T, typename std::list<T>::iterator>::iterator it = _lookup.find(element);
        if(it == _lookup.end()) return _list.end();
        return it->second;
    }
    typename std::list<T>::iterator find(T element) {
        typename std::unordered_map<T, typename std::list<T>::iterator>::iterator it = _lookup.find(element);
        if(it == _lookup.end()) return _list.end();
        return it->second;
    }

private:
    /**
     * @~english
     * @brief List that contains the elements.
     *
     * @~spanish
     * @brief Lista que contiene los elementos.
     */
    std::list<T> _list;

    /**
     * @~english
     * @brief Map that contains the element as the key, and an iterator to the list as the value.
     *
     * @~spanish
     * @brief Diccionario que contiene el elemento como clave, y un iterador a la lista como valor.
     */
    std::unordered_map<T, typename std::list<T>::iterator> _lookup;
};


#endif //MOTORSINNOMBREDECIDIDO_SEARCHABLELIST_H
