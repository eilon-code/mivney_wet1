#ifndef AVLTREE_H
#define AVLTREE_H

#include "wet1util.h"
#include "comparable.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

template<typename T>
class AVLTree {
public:
    AVLTree();
    ~AVLTree();
    int size() const;
    output_t<T*> get(const T& key) const;
    output_t<T*> getMax() const;
    output_t<T*> getMin() const;
    output_t<T*> insert(const T& key);
    StatusType remove(const T& key);
    void printInOrder(std::ostream& os) const;
    void print() const;

private:
    struct Node {
        T* key;
        bool removable;
        Node *left;
        Node *right;
        int height;
        Node(T* key) : key(key), removable(true), left(nullptr), right(nullptr), height(1) {}
    };
    int m_size;
    Node *m_root;
    output_t<Node*> insertNode(Node *root, Node *node);
    output_t<Node*> removeNode(Node *root,const T& key);
    Node* minValueNode(Node* node) const;
    Node* maxValueNode(Node* node) const;
    int getHeight(const Node *node) const;
    int getBalance(const Node *node) const;
    Node* rotateRight(Node *node);
    Node* rotateLeft(Node *node);
    void deleteTree(Node *node);
    void updateHeight(Node* node);
    void printInOrderRecursive(std::ostream& os, const Node *node) const;
    void printTree(Node *node, int space) const;
    output_t<Node*> search(Node* node, const T& key) const;
};

template<typename T>
AVLTree<T>::AVLTree() : m_size(0),m_root(nullptr) {}

template<typename T>
AVLTree<T>::~AVLTree() {
    deleteTree(m_root);
}

template<typename T>
int AVLTree<T>::size() const{
    return m_size;
}

template<typename T>
void AVLTree<T>::printInOrder(std::ostream& os)const{
    os << "Tree (with size " << m_size << ") { ";
    printInOrderRecursive(os, m_root);
    os << "}";
}

template<typename T>
void AVLTree<T>::deleteTree(Node *node){
    if(!node)return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete(node->key);
    delete(node);
}

template<typename T>
output_t<T*> AVLTree<T>::insert(const T& key){
    try{
        T* output = new T(key);
        Node* newNode = new Node(output);
        output_t<Node*> result=insertNode(m_root,newNode);
        if (result.status() != StatusType::SUCCESS) {
            delete output;
            delete newNode;
            return result.status();
        }
        m_root=result.ans();
        m_size += 1;
        return output;
    } catch (...) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

template<typename T>
output_t<typename AVLTree<T>::Node*> AVLTree<T>::insertNode(Node *root, Node *node){
    if(!root) return node;
    if (*(node->key) == *(root->key)) {
        return StatusType::FAILURE;
    }
    if(*(node->key) < *(root->key)) {
        output_t<Node*> result = insertNode(root->left, node);
        if (result.status() != StatusType::SUCCESS) return result.status();
        root->left = result.ans();
    }
    else {
        output_t<Node*> result = insertNode(root->right, node);
        if (result.status() != StatusType::SUCCESS) return result.status();
        root->right = result.ans();
    }
    updateHeight(root);

    int balance = getBalance(root);
    if (balance>1){//left heavy
        if(getBalance(root->left)<0){//right child of left tree is heavy
            root->left=rotateLeft(root->left);
        }
        return rotateRight(root);
    }
    if(balance<-1){//right heavy
        if(getBalance(root->right)>0){//left child of right tree is heavy
            root->right=rotateRight(root->right);
        }
        return rotateLeft(root);
    }
    return root;
}

template<typename T>
StatusType AVLTree<T>::remove(const T& key){
    try{
        output_t<Node*> result = removeNode(m_root,key);
        if (result.status() != StatusType::SUCCESS) return result.status();
        --m_size;
        m_root = result.ans();
    } catch(...) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

template<typename T>
output_t<typename AVLTree<T>::Node*> AVLTree<T>::removeNode(Node *root,const T& key){
    if(!root) return StatusType::FAILURE;
    if(*(root->key) == key){
        if (!(root->key->isRemovable())) return StatusType::FAILURE;
        if(root->left==nullptr && root->right==nullptr){
            if (root->removable) delete(root->key);
            delete(root);
            return nullptr;
        }else if(root->left==nullptr){
            Node *temp = root->right;
            if (root->removable) delete(root->key);
            delete(root);
            root=temp;
        }else if(root->right==nullptr){
            Node *temp = root->left;
            if (root->removable) delete(root->key);
            delete(root);
            root=temp;
        }else{
            //changing value of the root to the value of min node in the right tree
            //then removing the duplicate
            Node* newRoot = minValueNode(root->right);
            T* temp = newRoot->key;
            newRoot->removable = false;
            output_t<Node*> result = removeNode(root->right, *temp);
            if (result.status() != StatusType::SUCCESS) {
                newRoot->removable = true;
                return result.status();
            }
            delete root->key;
            root->key = temp;
            root->right= result.ans();
        }
    }else if(*(root->key) < key){
        output_t<Node*> result = removeNode(root->right,key);
        if (result.status() != StatusType::SUCCESS) return result.status();
        root->right=result.ans();
    }else{
        output_t<Node*> result = removeNode(root->left,key);
        if (result.status() != StatusType::SUCCESS) return result.status();
        root->left=result.ans();
    }

    updateHeight(root);
    int balance=getBalance(root);
    if (balance>1){//left heavy
        if(getBalance(root->left)<0){//right child of left tree is heavy
            root->left=rotateLeft(root->left);
        }
        return rotateRight(root);
    }
    if(balance<-1){//right heavy
        if(getBalance(root->right)>0){//left child of right tree is heavy
            root->right=rotateRight(root->right);
        }
        return rotateLeft(root);
    }
    return root;
}

template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::minValueNode(Node *node) const
{
    Node* result = node;
    while(result->left != nullptr)result=result->left;
    return result;
}

template <typename T>
typename AVLTree<T>::Node* AVLTree<T>::maxValueNode(Node *node) const
{
    Node* result = node;
    while(result->right != nullptr)result=result->right;
    return result;
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::rotateRight(Node* node){
    Node *newNode = node->left;
    node->left=newNode->right;
    newNode->right=node;
    updateHeight(node);
    updateHeight(newNode);
    return newNode;
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::rotateLeft(Node* node){
    Node *newNode = node->right;
    node->right=newNode->left;
    newNode->left=node;
    updateHeight(node);
    updateHeight(newNode);
    return newNode;
}

template<typename T>
void AVLTree<T>::updateHeight(Node* node){
    node->height=(getHeight(node->left)>getHeight(node->right))?getHeight(node->left):getHeight(node->right);
    node->height+=1;
}

template<typename T>
int AVLTree<T>::getHeight(const Node* node)const{
    if(!node){
        return 0;
    }
    return node->height;
}

template<typename T>
int AVLTree<T>::getBalance(const Node *node)const{
    return getHeight(node->left)-getHeight(node->right);
}

template<typename T>
void AVLTree<T>::printInOrderRecursive(std::ostream& os, const Node *node)const{
    if(node==nullptr){
        return;
    }
    printInOrderRecursive(os, node->left);
    os << *(node->key);
    printInOrderRecursive(os, node->right);
}

template <typename T>
inline void AVLTree<T>::print() const
{
    if (!m_root)
    {
        std::cout << "Tree is empty.\n";
        return;
    }
    printTree(m_root, 0);
}

template <typename T>
void AVLTree<T>::printTree(Node *node, int space) const
{
    if (!node) return;

    const int COUNT = 5;
    space += COUNT;

    printTree(node->right, space);

    std::cout << std::endl;
    for (int i = COUNT; i < space; ++i)
    {
        std::cout << " ";
    }
    std::cout << (node->key)->getId() << "\n";

    printTree(node->left, space);
}

template<typename T>
output_t<T*> AVLTree<T>::get(const T& key) const {
    try {
        output_t<Node*> result = search(m_root,key);
        if (result.status() != StatusType::SUCCESS) {
            return result.status();
        }
        return result.ans()->key;
    } catch(...) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::INVALID_INPUT;
}

template <typename T>
output_t<T*> AVLTree<T>::getMax() const
{
    if (m_root == nullptr) {
        return StatusType::FAILURE;
    }
    return maxValueNode(m_root)->key;// this is a safe-operation
}

template <typename T>
output_t<T*> AVLTree<T>::getMin() const
{
    if (m_root == nullptr) {
        return StatusType::FAILURE;
    }
    return minValueNode(m_root)->key;// this is a safe-operation
}

template<typename T>
output_t<typename AVLTree<T>::Node*> AVLTree<T>::search(Node *node, const T& key) const {
    if(!node) return StatusType::FAILURE;
    if(*(node->key)==key){
        return node;
    }
    else if(*(node->key) < key){
        return search(node->right, key);
    }
    else{
        return search(node->left, key);
    }
}

#endif // AVLTREE_H
