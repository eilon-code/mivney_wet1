#ifndef AVLTREE_H
#define AVLTREE_H

#include "../wet1util.h"
#include "comparable.h"
#include <memory>
#include <iostream>

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
    void printInOrder() const;

private:
    class AVLTreeException : std::exception {
    private:
        std::string n_message;
    public:
        // Constructor
        explicit AVLTreeException(const std::string& msg) : n_message(msg) {}

        // Override the what() method
        virtual const char* what() const noexcept override {
            return n_message.c_str();
        }
    };
    struct Node {
        T key;
        Node *left;
        Node *right;
        int height;
        Node(const T& key) : key(key), left(nullptr), right(nullptr), height(1) {}
    };
    int m_size;
    Node *m_root;
    Node* insertNode(Node *root, Node *node);
    Node* removeNode(Node *root,const T& key);
    Node* minValueNode(Node* node) const;
    Node* maxValueNode(Node* node) const;
    int getHeight(const Node *node) const;
    int getBalance(const Node *node) const;
    Node* rotateRight(Node *node);
    Node* rotateLeft(Node *node);
    void deleteTree(Node *node);
    void updateHeight(Node* node);
    void printInOrderRecursive(const Node *node) const;
    Node* search(Node* node, const T& key) const;
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
void AVLTree<T>::deleteTree(Node *node){
    if(!node)return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete(node);
}

template<typename T>
output_t<T*> AVLTree<T>::insert(const T& key){
    try{
        Node* newNode = new Node(key);
        m_root=insertNode(m_root,newNode);
        m_size += 1;
        return &(newNode->key);
    } catch (const AVLTreeException& e){
        return StatusType::FAILURE;
    } catch (...) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::insertNode(Node *root, Node *node){
    if(!root) return node;
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
        m_size -= 1;
    } catch(const AVLTreeException& e) {
        return StatusType::FAILURE;
    } catch(...) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::removeNode(Node *root,const T& key){
    if(!root) throw AVLTreeException("Error: key is not in any node of the tree.");
    if(root->key == key){
        if (!(root->key.isRemovable())) throw AVLTreeException("Cannot remove");
        if(root->left==nullptr && root->right==nullptr){
            delete(root);
            return nullptr;
        }else if(root->left==nullptr){
            Node *temp = root->right;
            delete(root);
            root=temp;
        }else if(root->right==nullptr){
            Node *temp = root->left;
            delete(root);
            root=temp;
        }else{
            //changing value of the root to the value of min node in the right tree
            //then removing the duplicate
            Node* newRoot = minValueNode(root->right);
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
void AVLTree<T>::printInOrderRecursive(const Node *node)const{
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

template <typename T>
output_t<T *> AVLTree<T>::getMin() const
{
    if (m_root == nullptr) {
        return StatusType::FAILURE;
    }
    return &(minValueNode(m_root)->key);// this is a safe-operation
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::search(Node *node, const T& key) const {
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
