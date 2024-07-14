#ifndef LIST_H
#define LIST_H

#include "../wet1util.h"
#include <iostream>

template<typename T>
class List {
public:
    struct Node {
        T key;
        Node *left;
        Node *right;
        Node(const T& key) : key(key), left(nullptr), right(nullptr) {}
    };
    List();
    ~List();
    int size() const;
    output_t<T> getLast() const;
    Node* getFirstNode() const{
        return m_start;
    }
    output_t<Node*> insert(const T& key);
    StatusType remove(Node* node);

private:
    int m_size;
    Node *m_start;
    Node *m_end;
};

template<typename T>
List<T>::List() : m_size(0), m_start(new Node(T())), m_end(new Node(T())) {
    m_start->right = m_end;
    m_end->left = m_start;
}

template<typename T>
List<T>::~List() {
    while (m_size > 0) {
        remove(m_start->right);
    }
    
    delete m_start;
    delete m_end;
}

template<typename T>
int List<T>::size() const {
    return m_size;
}

template <typename T>
output_t<T> List<T>::getLast() const {
    if (size() == 0) {
        return StatusType::FAILURE;
    }
    return m_end->left->key;
}

template <typename T>
output_t<typename List<T>::Node *> List<T>::insert(const T &key) {
    try {
        Node *newNode = new Node(key);
        Node *second = m_start->right;

        m_start->right = newNode;
        newNode->left = m_start;

        newNode->right = second;
        second->left = newNode;

        ++m_size;
        return newNode;
    } catch (...) {
        return StatusType::ALLOCATION_ERROR;
    }
}

template <typename T>
StatusType List<T>::remove(Node *node) {
    if (node == nullptr || node->right == nullptr || node->left == nullptr) return StatusType::FAILURE;
    try {
        Node *right = node->right;
        Node *left = node->left;
        
        delete node;

        right->left = left;
        left->right = right;

        --m_size;
    } catch (...) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

#endif // LIST_H
