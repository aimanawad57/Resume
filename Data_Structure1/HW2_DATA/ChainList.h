#include "Node.h"

#ifndef DS_WET2_WINTER_2024_2025_CHAINLIST_H
#define DS_WET2_WINTER_2024_2025_CHAINLIST_H


template<class Q>
class List {
private:
    int listSize;
    Node<int, Q> *head;
public:
    List() : listSize(0), head(nullptr) {}

    ~List();
    void addNode(int key, const Q &element);
    void addNode(Node<int, Q> *other);
//    void removeBykey(int key);
    void removeHead();
    Node<int, Q> *find(int key);
    void removeRecord(int key, int teamId);
    void addRecord(int key, shared_ptr<Q> &data);
    Node<int, Q> *findRecord(int key, int teamId);

    void disLocate(int key) {
        auto theNode = find(key);
        if (!theNode || !head) {
            return; // Key not found or list is empty
        }
        auto current = head;

        if (head->getKey() == key) {
            head = head->getNext();
            theNode->setNext(nullptr); // Disconnect the node
            listSize--;
            return;
        }
        while (current->getNext()) {
            int n_key = current->getNext()->getKey();
            if (n_key == key) {
                current->setNext(theNode->getNext());
                theNode->setNext(nullptr);
                listSize--;
                return;
            }
            current = current->getNext();
        }
    }

    int getSize() const {
        return listSize;
    }

    void setSize(int size) {
        listSize = size;
    }

    Node<int, Q> *getFirst() {
        return head;
    }

    void SetHead(Node<int, Q> *newHead) {
        head = newHead;
    }
};


template<class Q>
void List<Q>::addNode(int key, const Q &element) {
    auto newNode = new Node<int, Q>(key, element);
    if (!head) {
        head = newNode;
    } else {
        auto temp = head;
        head = newNode;
        newNode->setNext(temp);
    }
    listSize++;
}

template<class Q>
void List<Q>::removeHead() {
    if (!head) {
        return;
    }
    auto temp = head;
    head = temp->getNext();
    if (temp) {
        delete temp;
    }
    listSize--;
}


template<class Q>
void List<Q>::addNode(Node<int, Q> *other) {
    if (!other) {
        return;
    }
    if (!head) {
        head = other;
        other = nullptr;
    } else {
        auto temp = head;
        head = other;
        other = nullptr;
        head->setNext(temp);
    }
    listSize++;
}


template<class Q>
Node<int, Q> *List<Q>::find(int key) {
    if (!head) {
        return nullptr;
    }
    auto temp = head;
    while (temp) {
        if (temp->getKey() == key) {
            return temp;
        }
        temp = temp->getNext();
    }
    return nullptr;
}

template<class Q>
List<Q>::~List() {
    while (head) {
        Node<int, Q> *temp = head->getNext();
        if (head) {
            delete head;
        }
        head = temp;
    }
}

template<class Q>
void List<Q>::removeRecord(int key, int teamId) {
    auto temp = findRecord(key, teamId);
    if (!temp) {
        return;
    }

    if (listSize == 1) {
        delete temp;
        head = nullptr;
        listSize--;
        return;
    }

    if (head == temp) {
        auto second = temp->getNext();
        delete head;
        head = second;
        listSize--;
        return;
    }

    auto curr = head;
    while (curr->getNext()) {
        int m_key = curr->getNext()->getKey();
        auto m_data = curr->getNext()->getData();
        if (m_key == key && *(m_data) == teamId) {
            curr->setNext(temp->getNext());
            delete temp;
            temp = nullptr;
            listSize--;
            return;
        }
        curr = curr->getNext();
    }

}

template<class Q>
void List<Q>::addRecord(int key, shared_ptr<Q> &data) {
    auto temp = findRecord(key, data);
    if (temp) {
        return;
    }
    auto newNode = new Node<int, Q>(key, data);
    if (!head) {
        head = newNode;
    } else {
        auto curr = head;
        head = newNode;
        newNode->setNext(curr);
    }
    listSize++;
}

template<class Q>
Node<int, Q> *List<Q>::findRecord(int key, int teamId) {
    if (!head) {
        return nullptr;
    }
    auto temp = head;
    while (temp) {
        if (temp->getKey() == key && *(temp->getData()) == teamId) {
            return temp;
        }
        temp = temp->getNext();
    }
    return nullptr;
}

#endif //DS_WET2_WINTER_2024_2025_CHAINLIST_H
