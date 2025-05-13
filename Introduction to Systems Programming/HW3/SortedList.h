#pragma once

#include <iostream>
#include <stdexcept>

namespace mtm {

    template<typename T>
    class SortedList {
    public:
        /**
         *
         * the class should support the following public interface:
         * if needed, use =defualt / =delete
         *
         * constructors and destructor:

         * 1. SortedList() - creates an empty list.
         * 2. copy constructor
         * 3. operator= - assignment operator
         * 4. ~SortedList() - destructor
         *
         * iterator:
         * 5. class ConstIterator;
         * 6. begin method
         * 7. end method
         *
         * functions:
         * 8. insert - inserts a new element to the list
         * 9. remove - removes an element from the list
         * 10. length - returns the number of elements in the list
         * 11. filter - returns a new list with elements that satisfy a given condition
         * 12. apply - returns a new list with elements that were modified by an operation
         */

        SortedList();
        SortedList(const SortedList &sortedList);
        SortedList &operator=(const SortedList &sortedList);
        ~SortedList();

        class ConstIterator;

        const ConstIterator begin() const;
        const ConstIterator end() const;

        void insert(const T &element);
        void remove(const SortedList<T>::ConstIterator &it);
        int length() const;
        template<class Func>
        SortedList<T> apply(Func func) const;
        template<class Predicate>
        SortedList<T> filter(Predicate c) const;

    private:
        struct Node {
            T data;
            Node *next;

            Node(const T &data) : data(data), next(nullptr) {}
        };

        void clearList();
        Node *head;
        int size{};
    };


    template<class T>
    SortedList<T>::SortedList() : head(nullptr), size(0) {}


    template<class T>
    SortedList<T>::SortedList(const SortedList<T> &sortedList):head(nullptr),
                                                               size(0) {
        Node *ptr = sortedList.head;
        while (ptr != nullptr) {
            try {
                insert(ptr->data);
            } catch (std::bad_alloc &e) {
                throw;
            }
            ptr = ptr->next;
        }
    }


    template<class T>
    SortedList<T> &SortedList<T>::operator=(const SortedList &sortedList) {
        if (this == &sortedList) {
            return *this;
        }
        if (sortedList.size == 0) {
            clearList();
            head = nullptr;
            size = 0;
            return *this;
        }

        Node *newhead = nullptr;
        try {
            newhead = new Node(sortedList.head->data);
            Node *ptr1 = newhead;
            Node *ptr2 = sortedList.head->next;
            while (ptr2 != nullptr) {
                ptr1->next = new Node(ptr2->data);
                ptr1 = ptr1->next;
                ptr2 = ptr2->next;
            }
            ptr1->next = nullptr;

        } catch (const std::bad_alloc &e) {
            while (newhead != nullptr) {
                Node *tmp = newhead->next;
                delete newhead;
                newhead = tmp;
            }
            throw;
        }
        clearList();
        head = newhead;
        size = sortedList.size;
        return *this;
    }

    template<class T>
    void SortedList<T>::clearList() {
        Node *tmp = head;
        while (head != nullptr) {
            tmp = head->next;
            delete head;
            head = tmp;
        }
    }


    template<class T>
    SortedList<T>::~SortedList() {
        clearList();
    }


    template<class T>
    void SortedList<T>::insert(const T &element) {
        try {
            Node *newNode = new Node(element);
            // the element should be at the head(empty list / the bigger)
            if (head == nullptr || !(head->data > element)) {
                newNode->next = head;
                head = newNode;
            } else {
                //the new element should be in the middle or at the end.
                Node *ptr = head;
                while (ptr->next != nullptr && ptr->next->data > element) {
                    ptr = ptr->next;
                }
                newNode->next = ptr->next;
                ptr->next = newNode;
            }
            size++;
        } catch (std::bad_alloc &e) {
            throw; //rethrow the exception;
        }
    }


    template<class T>
    void SortedList<T>::remove(const ConstIterator &it) {
        if (!head || !(it != end())) {
            return; //list is empty or iterator points to end of list
            //shouldn't use throw?
        }
        if (head == it.current) {
            Node *ptr = head;
            head = head->next;
            delete ptr;
            size--;
            return;
        }
        Node *ptr = head;
        while (ptr->next && ptr->next != it.current) {
            ptr = ptr->next;
        }
        if (!ptr->next) {
            return; //not found
        }
        Node *tmp = ptr->next;
        ptr->next = ptr->next->next;
        delete tmp;
        size--;
    }


    template<class T>
    template<class Predicate>
    SortedList<T> SortedList<T>::filter(Predicate c) const {
        SortedList<T> newList;
        Node *ptr = head;

        while (ptr != nullptr) {
            if (c(ptr->data)) {
                newList.insert(ptr->data);
            }
            ptr = ptr->next;
        }

        return newList;
    }


    template<class T>
    template<class Func>
    SortedList<T> SortedList<T>::apply(Func func) const {
        SortedList<T> newList;
        Node *ptr = head;

        while (ptr != nullptr) {
            newList.insert(func(ptr->data));
            ptr = ptr->next;
        }

        return newList;
    }


    template<class T>
    int SortedList<T>::length() const {
        return size;
    }


    template<class T>
    class SortedList<T>::ConstIterator {
        /**
         * the class should support the following public interface:
         * if needed, use =defualt / =delete
         *
         * constructors and destructor:
         * 1. a ctor(or ctors) your implementation needs
         * 2. copy constructor
         * 3. operator= - assignment operator
         * 4. ~ConstIterator() - destructor
         *
         * operators:
         * 5. operator* - returns the element the iterator points to
         * 6. operator++ - advances the iterator to the next element
         * 7. operator!= - returns true if the iterator points to a different element
         *
         */
    public:
        ConstIterator(const ConstIterator &) = default;
        ConstIterator &operator=(const ConstIterator &) = default;
        ~ConstIterator() = default;
        explicit ConstIterator(const Node *node);

        const T &operator*() const;
        ConstIterator &operator++();
        bool operator!=(const ConstIterator &iterator) const;

    private:
        const Node *current;

        friend class SortedList<T>;
    };

    template<class T>
    SortedList<T>::ConstIterator::ConstIterator(
            const mtm::SortedList<T>::Node *node):current(node) {}

    template<class T>
    const T &SortedList<T>::ConstIterator::operator*() const {
        if (current == nullptr) {
            throw std::out_of_range("");
        }
        return current->data;
    }

    template<class T>
    typename SortedList<T>::ConstIterator &
    SortedList<T>::ConstIterator::operator++() {
        if (!current) {
            throw std::out_of_range("");
        }
        current = current->next;
        return *this;
    }

    template<class T>
    bool SortedList<T>::ConstIterator::operator!=(
            const ConstIterator &iterator) const {
        return current != iterator.current;
    }

    template<class T>
    const typename SortedList<T>::ConstIterator SortedList<T>::begin() const {
        return ConstIterator(head);
    }

    template<class T>
    const typename SortedList<T>::ConstIterator SortedList<T>::end() const {
        return ConstIterator(nullptr);
    }

}