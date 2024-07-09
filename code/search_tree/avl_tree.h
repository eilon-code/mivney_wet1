#ifndef AVLTREE_H
#define AVLTREE_H

#include "../wet1util.h"
#include "comparable.h"
#include <memory>
#include <iostream>

using namespace std;

class AVLTreeException : public std::exception {
private:
    std::string message;
public:
    // Constructor
    explicit AVLTreeException(const std::string& msg) : message(msg) {}

    // Override the what() method
    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};

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
    output_t<T*> get(const T& key) const;
    output_t<T*> getMax() const;
    StatusType insert(const T& key);
    StatusType remove(const T& key);
    void printInOrder() const;

private:
    int m_size;
    Node<T> *m_root;
    Node<T>* insertNode(Node<T> *root, Node<T> *node);
    Node<T>* removeNode(Node<T> *root,const T& key);
    Node<T>* minValueNode(Node<T>* node) const;
    Node<T>* maxValueNode(Node<T>* node) const;
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
    } catch (const AVLTreeException& e){
        return StatusType::FAILURE;
    } catch (...) {
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
    if (node->key == root->key) {
        throw AVLTreeException("Node key matches root key");
    }
    if(node->key < root->key) root->left = insertNode(root->left, node);
    else root->right = insertNode(root->right, node);
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
        m_root=removeNode(m_root,key);
    } catch(const AVLTreeException& e) {
        return StatusType::FAILURE;
    } catch(...) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

template<typename T>
Node<T>* AVLTree<T>::removeNode(Node<T> *root,const T& key){
    if(!root) throw AVLTreeException("Error: key is not in any node of the tree.");
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
            Node<T>* newRoot = minValueNode(root->right);
            root->key=newRoot->key;
            root->right=removeNode(root->right,root->key);
        }
    }else if(root->key < key){
        root->right=removeNode(root->right,key);
    }else{
        root->left=removeNode(root->left,key);
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
Node<T>* AVLTree<T>::minValueNode(Node<T> *node) const
{
    Node<T>* result = node;
    while(result->left != nullptr)result=result->left;
    return result;
}

template <typename T>
Node<T>* AVLTree<T>::maxValueNode(Node<T> *node) const
{
    Node<T>* result = node;
    while(result->right != nullptr)result=result->right;
    return result;
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
output_t<T*> AVLTree<T>::get(const T& key) const {
    try {
        return &(search(m_root,key)->key);
    } catch(const AVLTreeException& e) {
        return StatusType::FAILURE;
    } catch(...) {
        return StatusType::ALLOCATION_ERROR;
    }
}

template <typename T>
output_t<T*> AVLTree<T>::getMax() const
{
    if (m_root == nullptr) {
        return StatusType::FAILURE;
    }
    return &(maxValueNode(m_root)->key);// this is a safe-operation
}

template<typename T>
Node<T>* AVLTree<T>::search(Node<T> *node, const T& key) const {
    if(!node) throw AVLTreeException("Error: key was not found in the tree.");
    if(node->key==key){
        return node;
    }
    else if(node->key < key){
        return search(node->right, key);
    }
    else{
        return search(node->left, key);
    }
}

#endif // AVLTREE_H
