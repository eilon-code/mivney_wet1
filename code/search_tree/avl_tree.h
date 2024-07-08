#ifndef AVLTREE_H
#define AVLTREE_H

#include "wet1util.h"
#include "comparable.h"
#include <memory>
#include <iostream>

template<typename T>
struct Node {
    T key;
    std::unique_ptr<Node<T>> left;
    std::unique_ptr<Node<T>> right;
    int height;

    Node(const T& key) : key(key), left(nullptr), right(nullptr), height(1) {}
};

template<typename T>
class AVLTree {
    // static_assert(std::is_base_of<Comparable, T>::value, "T must inherit from Comparable");

public:
    AVLTree() : m_size(0), m_root(nullptr) {}
    ~AVLTree();

    int size() const {
        return m_size;
    }

    output_t<T*> get(const T& key) const;
    output_t<T*> getMax() const;
    StatusType insert(const T& key);
    StatusType remove(const T& key);
    StatusType search(const T& key) const;
    void printInorder() const;

private:
    int m_size;
    std::unique_ptr<Node<T>> m_root;

    std::unique_ptr<Node<T>> insertNode(std::unique_ptr<Node<T>> node, const T& key);
    std::unique_ptr<Node<T>> removeNode(std::unique_ptr<Node<T>> node, const T& key);
    Node<T>* minValueNode(Node<T>* node) const;
    int getHeight(const std::unique_ptr<Node<T>>& node) const;
    int getBalance(const std::unique_ptr<Node<T>>& node) const;
    std::unique_ptr<Node<T>> rotateRight(std::unique_ptr<Node<T>> y);
    std::unique_ptr<Node<T>> rotateLeft(std::unique_ptr<Node<T>> x);
    void printInorderRecursive(const std::unique_ptr<Node<T>>& node) const;
    void deleteTree(std::unique_ptr<Node<T>>& node);
    void updateHeight(Node<T>* node);
};

#endif // AVLTREE_H
