#pragma once
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include <stdexcept>
using namespace std;

template <class ElementType>
class LinkedList
{
public:
    class Node {
    public:

        ElementType data;
        Node* next;

        Node()
            : next(0)
        {}

        Node(ElementType dataValue)
            : data(dataValue), next(0)
        {}

    };
    typedef Node* NodePtr;

    NodePtr first;
    int mySize;

    LinkedList();

    ~LinkedList();

    bool empty() const;

    void insert(ElementType dataVal, int index);

    void push_back(ElementType dataVal);

    void erase(int index);

    void display(ostream& out) const;

    int size() const;

    int search(ElementType dataVal);

    NodePtr getFirst() const;
};



template<class ElementType>
LinkedList<ElementType>::LinkedList()
    : first(0), mySize(0)
{}

template<class ElementType>
LinkedList<ElementType>::~LinkedList() {
    LinkedList<ElementType>::NodePtr prev = first,
        ptr;
    while (prev != nullptr) {
        ptr = prev->next;
        delete prev;
        prev = ptr;
    }
}

template<class ElementType>
bool LinkedList<ElementType>::empty() const {
    return mySize == 0;
}

template<class ElementType>
void LinkedList<ElementType>::insert(ElementType dataVal, int index) {
    if (index < 0 or index > mySize)
    {
        cerr << "Illegal location to insert " << index << endl;
        return;
    }

    LinkedList<ElementType>::NodePtr newPtr = new Node(dataVal),
        predPtr = first;
    if (index == 0)
    {
        newPtr->next = first;
        first = newPtr;
    }
    else {
        for (int i = 1; i < index; i++)
            predPtr = predPtr->next;
        newPtr->next = predPtr->next;
        predPtr->next = newPtr;
    }
    mySize++;
}


template<class ElementType>
void LinkedList<ElementType>::push_back(ElementType dataVal) {
    LinkedList<ElementType>::NodePtr newNode = new Node(dataVal);
    if (first == nullptr) {
        first = newNode;
    }
    else {
        NodePtr current = first;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
    mySize++;
    
}

template <class ElementType>
void LinkedList<ElementType>::erase(int index) {
    // Check for an empty list or invalid index
    if (first == nullptr || index < 0 || index >= mySize) {
        throw out_of_range("Index out of range");
    }

    LinkedList<ElementType>::NodePtr toDelete = first;

    // If the node to be deleted is the first node
    if (index == 0) {
        first = first->next;
    }
    else {
        // Find the node just before the one to delete
        LinkedList<ElementType>::NodePtr current = first;
        for (int i = 0; i < index - 1; ++i) {
            current = current->next;
        }

        // Update the link to bypass the node to be deleted
        toDelete = current->next;
        current->next = current->next->next;
    }

    // Delete the node and decrement the size
    delete toDelete;
    mySize--;
}


template<class ElementType>
void LinkedList<ElementType>::display(ostream& out) const {
    LinkedList<ElementType>::NodePtr current = first;
    while (current != nullptr) {
        out << current->data << " ";
        current = current->next;
    }
}

template<class ElementType>
int LinkedList<ElementType>::size() const {
    return mySize;
}

template<class ElementType>
int LinkedList<ElementType>::search(ElementType dataVal) {
    LinkedList<ElementType>::NodePtr current = first;
    int index = 0;
    while (current != nullptr) {
        if (current->data == dataVal) return index;
        current = current->next;
        index++;
    }
    return -1;
}

template<class ElementType>
typename LinkedList<ElementType>::NodePtr LinkedList<ElementType>::getFirst() const {
    return first;
}
#endif // !LINKEDLIST_H