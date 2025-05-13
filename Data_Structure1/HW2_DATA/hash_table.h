#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <cmath>
#include "Node.h"
#include "ChainList.h"


#define INITIAL_SIZE 97
#define ALFA 1.1

template<class Q>

class hashtable {
private:
    List<Q> *m_table;
    int m_size;
    int m_elemNum;

    List<Q> *resize_array();
public:
    hashtable();
    virtual ~hashtable();
    int hashFunction(int id, int size);
    Node<int, Q> *insert(int key, shared_ptr<Q> &data);
    bool remove(int key);
    int getSize();
    List<Q> *getTable();
    Node<int, Q> *find(int key);
    Node<int, Q> *findRecord(int key, int teamId);
    Node<int, Q> *insertRecord(int key, int teamId);
    bool removeRecord(int key, int teamId);

};

template<class Q>
Node<int, Q> *hashtable<Q>::find(int key) {
    int index = hashFunction(key, m_size);
    Node<int, Q> *temp = m_table[index].find(key);
    if (temp != nullptr) {
        return temp;
    }
    return nullptr;
}

template<class Q>
bool hashtable<Q>::remove(int key) {
    int index = hashFunction(key, m_size);
    Node<int, Q> *current = m_table[index].find(key);

    if (current == nullptr || current->isRemoved()) {
        return false;
    }

    current->remove();
    return true;
}


template<class Q>
Node<int, Q> *hashtable<Q>::insert(int key, shared_ptr<Q> &data) {
    int index = hashFunction(key, m_size);
    Node<int, Q> *current = m_table[index].find(key);

    if (current) {
        return nullptr;
    }

    if (current == nullptr) {
        Node<int, Q> *newNode = new Node<int, Q>(key, data, nullptr, nullptr,
                                                 false);
        m_table[index].addNode(newNode);
        m_elemNum++;
        if ((double)(m_elemNum / m_size) >= ALFA) {
            resize_array();
            int newIndex = hashFunction(key, m_size);
            return m_table[newIndex].find(key);
        }
        return newNode;
    }

    return nullptr;
}

template<class Q>
List<Q> *hashtable<Q>::resize_array() {
    int oldSize = m_size;
    int newSize = m_size * 2;
    List<Q> *newArray = new List<Q>[newSize]; // New table

    for (int i = 0; i < oldSize; i++) {
        Node<int, Q> *current = m_table[i].getFirst();
        while (current) {
            int newIndex = hashFunction(current->getKey(), newSize);

            Node<int, Q> *next = current->getNext();
            current->setNext(nullptr); // Disconnect the node from the old chain
            newArray[newIndex].addNode(
                    current); // Add the node to the new table
            current = next;
        }
        m_table[i].SetHead(nullptr);
    }

    // Delete the old table
    delete[] m_table;

    m_table = newArray;
    m_size = newSize;

    return newArray;
}

template<class Q>
int hashtable<Q>::hashFunction(int id, int size) {
    return ((id % size) + size) % size;
}

template<class Q>
List<Q> *hashtable<Q>::getTable() {
    return m_table;
}

template<class Q>
int hashtable<Q>::getSize() {
    return m_size;
}

template<class Q>
hashtable<Q>::~hashtable() {
    delete[] m_table;
}

template<class Q>
hashtable<Q>::hashtable() : m_table(new List<Q>[INITIAL_SIZE]),
                            m_size(INITIAL_SIZE), m_elemNum(0) {}

template<class Q>
Node<int, Q> *hashtable<Q>::findRecord(int key, int teamId) {
    int index = hashFunction(key, m_size);
    if (m_table[index].getSize() > 0) {
        auto temp = m_table[index].findRecord(key, teamId);
        if (temp != nullptr) {
            return temp;
        }
    }
    return nullptr;
}

template<class Q>
Node<int, Q> *hashtable<Q>::insertRecord(int key, int teamId) {
    int index = hashFunction(key, m_size);
    auto current = m_table[index].findRecord(key, teamId);
    if (current) {
        return nullptr;
    }
    if (current == nullptr) {
        auto record = make_shared<Record>(teamId);
        Node<int, Q> *newNode = new Node<int, Q>(key, record, nullptr, nullptr,
                                                 false);
        m_table[index].addNode(newNode);
        m_elemNum++;
        if ((double)(m_elemNum / m_size) >= ALFA) {
            resize_array();
            int newIndex = hashFunction(key, m_size);
            return m_table[newIndex].find(key);
        }
        return newNode;
    }


    return nullptr;
}

template<class Q>
bool hashtable<Q>::removeRecord(int key, int teamId) {
    int index = hashFunction(key, m_size);
    Node<int, Q> *current = m_table[index].findRecord(key, teamId);
    if (current == nullptr) {
        return false;
    }
    m_table[index].removeRecord(key, teamId);
    m_elemNum--;
    return true;
}


#endif