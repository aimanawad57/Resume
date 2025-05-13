

#ifndef DS_WET2_WINTER_2024_2025_NODE_H
#define DS_WET2_WINTER_2024_2025_NODE_H

#include "TeamAndJockey.h"
#include "stdexcept"
#include "memory"
#include "iostream"

using namespace std;

template<class K, class T>
class Node {
private:
    K key;
    shared_ptr<T> data;
    Node *next;
    Node<K, Team> *parent;
    bool removed;
public:
    Node(K key, shared_ptr<T> data, Node<K, Team> *parent = nullptr,
         Node<K, T> *next = nullptr, bool removed = false) : key(key),
                                                             next(next), parent(
                    parent), removed(removed) {
        if (!data) {
            throw std::runtime_error("constructor with null data");
        }
        this->data = data;
    }


    Node(const Node<K, T> &other) : key(other.key), data(other.data),
                                    next(nullptr), parent(
                    nullptr), removed(other.removed) {
        if (data == nullptr) {
            throw std::runtime_error("copy constructor with null data");
        }
        if (other.parent) {
            parent = other.parent;
        }
        if (other.next) {
            next = other.next;
        }
    }

    Node<K, T> &operator=(const Node<K, T> &other) {
        if (this == &other) {
            return *this; // Handle self-assignment
        }

        // Copy the key and data
        key = other.key;
        if (other.data == nullptr) {
            throw std::runtime_error("operator = with null data");
        }
        data = other.data;

        // Copy the next and parent pointers
        next = other.next;
        parent = other.parent;

        return *this;
    }

    bool isRemoved() const {
        return removed;
    }

    void remove() {
        removed = true;
    }


    K getKey() const {
        if (key == 0) {
            return key;
        }
        if (key) {
            return key;
        } else {
            throw std::runtime_error("trying to return null key");
        }
    }

    shared_ptr<T> getData() const {
        if (!data) {
            throw std::runtime_error("Null data");
        }
        return data;
    }

    ~Node() {
    }

    Node<K, T> *getNext() {
        return next;
    }

    void setNext(Node<K, T> *other) {
        next = other;
    }

    Node<K, Team> *getParent() {
        try {
            return parent;
        } catch (...) {
            throw std::runtime_error("catched error of:");
        }
    }

    void setParent(Node<K, Team> *newParent) {
        parent = newParent;
    }

    void setData(const std::shared_ptr<T> &newData) {
        data = newData;
    }

    void setKey(int newKey) {
        this->key = newKey;
    }

};

#endif //DS_WET2_WINTER_2024_2025_NODE_H


