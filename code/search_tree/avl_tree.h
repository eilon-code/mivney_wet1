#ifndef AVLTREE_H
#define AVLTREE_H

#include "../wet1util.h"
#include "comparable.h"
#include <memory>
#include <iostream>

using namespace std;

template<typename T>
struct Node {
    T key;
    Node *left;
    Node *right;
    int height;
    Node(const T& key) : key(key), left(nullptr), right(nullptr), height(1) {}
};

template<typename T>
class AVLTree {
public:
    AVLTree();
    ~AVLTree();
    int size() const;
    Node<T>* get(const T& key) const;
    Node<T>* getMax() const;
    StatusType insert(const T& key);
    StatusType remove(const T& key);
    void printInOrder() const;

private:
    int m_size;
    Node<T> *m_root;
    Node<T>* insertNode(Node<T> *root, Node<T> *node);
    Node<T>* removeNode(Node<T> *root,const T& key);
    Node<T>* minValueNode(Node<T>* node) const;
    int getHeight(const Node<T> *node) const;
    int getBalance(const Node<T> *node) const;
    Node<T>* rotateRight(Node<T> *node);
    Node<T>* rotateLeft(Node<T> *node);
    void deleteTree(Node<T> *node);
    void updateHeight(Node<T>* node);
    void printInOrderRecursive(const Node<T> *node) const;
    Node<T>* search(Node<T>* node, const T& key) const;
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
void AVLTree<T>::printInOrder()const{
    printInOrderRecursive(m_root);
    std::cout << std::endl;
}

template<typename T>
void AVLTree<T>::deleteTree(Node<T> *node){
    if(!node)return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete(node);
}

template<typename T>
StatusType AVLTree<T>::insert(const T& key){
    try{
        Node<T>* newNode = new Node<T>(key);
        m_root=insertNode(m_root,newNode);
    } catch (...){
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

template<typename T>
Node<T>* AVLTree<T>::insertNode(Node<T> *root, Node<T> *node){
    if(!root){
        m_size+=1;
        return node;
    }
    if(node->key < root->key) root->left = insertNode(root->left, node);
    else if(node->key > root->key) root->right = insertNode(root->right, node);
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
    if(get(key)==nullptr){
        return StatusType::INVALID_INPUT;
    }
    m_root=removeNode(m_root,key);
    return StatusType::SUCCESS;
}

template<typename T>
Node<T>* AVLTree<T>::removeNode(Node<T> *root,const T& key){
    if(!root) return nullptr;
    if(root->key == key){
        if(root->left==nullptr && root->right==nullptr){
            delete(root);
            return nullptr;
        }else if(root->left==nullptr){
            Node<T> *temp = root->right;
            delete(root);
            root=temp;
        }else if(root->right==nullptr){
            Node<T> *temp = root->left;
            delete(root);
            root=temp;
        }else{
            //changing value of the root to the value of min node in the right tree
            //then removing the duplicate
            Node<T> *right = root->right;
            while(right->left != nullptr)right=right->left;
            root->key=right->key;
            root->right=removeNode(root->right,right->key);
        }

    }else if(root->key > key){
        root->left=removeNode(root->left,key);
    }else{
        root->right=removeNode(root->right,key);
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

template<typename T>
Node<T>* AVLTree<T>::rotateRight(Node<T>* node){
    Node<T> *newNode = node->left;
    node->left=newNode->right;
    newNode->right=node;
    updateHeight(node);
    updateHeight(newNode);
    return newNode;
}

template<typename T>
Node<T>* AVLTree<T>::rotateLeft(Node<T>* node){
    Node<T> *newNode = node->right;
    node->right=newNode->left;
    newNode->left=node;
    updateHeight(node);
    updateHeight(newNode);
    return newNode;
}

template<typename T>
void AVLTree<T>::updateHeight(Node<T>* node){
    node->height=(getHeight(node->left)>getHeight(node->right))?getHeight(node->left):getHeight(node->right);
    node->height+=1;
}

template<typename T>
int AVLTree<T>::getHeight(const Node<T>* node)const{
    if(!node){
        return 0;
    }
    return node->height;
}

template<typename T>
int AVLTree<T>::getBalance(const Node<T> *node)const{
    return getHeight(node->left)-getHeight(node->right);
}

template<typename T>
void AVLTree<T>::printInOrderRecursive(const Node<T> *node)const{
    if(node==nullptr){
        return;
    }
    printInOrderRecursive(node->left);
    std::cout << node->key << " ";
    printInOrderRecursive(node->right);
}

template<typename T>
Node<T>* AVLTree<T>::get(const T& key) const {
    return search(m_root,key);
}

template<typename T>
Node<T>* AVLTree<T>::search(Node<T> *node, const T& key) const {
    if(!node)return nullptr;
    if(node->key==key){
        return node;
    }
    else if(node->key > key){
        return search(node->left, key);
    }
    else{
        return search(node->right, key);
    }
}

#endif // AVLTREE_H
