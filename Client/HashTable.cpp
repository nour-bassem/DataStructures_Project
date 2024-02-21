#include <iostream>
#include "HashTable.h"
HashTable::HashTable() {
    for (int i = 0; i < SIZE; ++i) {
        myTable[i] = nullptr;
    }
}

HashTable::~HashTable() {
    for (int i = 0; i < SIZE; ++i) {
        NodePtr entry = myTable[i];
        while (entry != nullptr) {
            NodePtr prev = entry;
            entry = entry->next;
            delete prev;
        }
    }
}

int HashTable::hashfunc(const Key& key) const {
    int hashVal = 0;
    for (char c : key) {
        hashVal = 37 * hashVal + c;
    }
    return hashVal % SIZE;
}

HashTable::Node* HashTable::find(const Key& key) {
    int bucket = hashfunc(key);
    NodePtr entry = myTable[bucket];
    while (entry != nullptr) {
        if (entry->key == key) {
            return entry;
        }
        entry = entry->next;
    }
    return nullptr;
}

bool HashTable::empty() const {
    for (int i = 0; i < SIZE; ++i) {
        if (myTable[i] != nullptr) {
            return false;
        }
    }
    return true;
}

void HashTable::insert(const Key& key, const Value& value) {
    int bucket = hashfunc(key);
    NodePtr newnode = new Node(key, value);
    newnode->next = myTable[bucket];
    myTable[bucket] = newnode;
}

HashTable::Value* HashTable::getValue(const Key& key) {
    Node* node = find(key);
    if (node == nullptr) {
        return nullptr;
    }
    return &(node->value);
}

bool HashTable::remove(const Key& key) {
    int bucket = hashfunc(key);
    NodePtr entry = myTable[bucket];
    NodePtr prev = nullptr;

    while (entry != nullptr && entry->key != key) {
        prev = entry;
        entry = entry->next;
    }

    if (entry == nullptr) {
        // Key not found
        return false;
    }

    if (prev == nullptr) {
        // Remove first bucket of the list
        myTable[bucket] = entry->next;
    }
    else {
        prev->next = entry->next;
    }

    delete entry;
    return true;
}

void HashTable::display(ostream& out) const {
    for (int i = 0; i < SIZE; ++i) {
        NodePtr entry = myTable[i];
        out << "[" << i << "]";
        while (entry != nullptr) {
            out << " -> " << entry->key << ":" << entry->value;
            entry = entry->next;
        }
        out << endl;
    }
}