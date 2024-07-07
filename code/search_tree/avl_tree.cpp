#include "avl_tree.h"
#include <algorithm>

template<typename T>
AVLTree<T>::AVLTree() : root(nullptr) {}

template<typename T>
AVLTree<T>::~AVLTree() {
    deleteTree(root);
}

template<typename T>
output_t<const T&> AVLTree<T>::get(const T& key) const {
    Node<T>* current = root.get();
    while (current != nullptr) {
        if (key < current->key) {
            current = current->left.get();
        } else if (key > current->key) {
            current = current->right.get();
        } else {
            return output_t<const T&>(current->key);
        }
    }
    return output_t<const T&>(StatusType::FAILURE);
}

template<typename T>
output_t<const T&> AVLTree<T>::getMax() const {
    Node<T>* current = root.get();
    if (current == nullptr) return output_t<const T&>(StatusType::FAILURE);

    while (current->right != nullptr) {
        current = current->right.get();
    }
    return output_t<const T&>(current->key);
}

template<typename T>
StatusType AVLTree<T>::insert(const T& key) {
    try {
        root = insertNode(std::move(root), key);
        return StatusType::SUCCESS;
    } catch (...) {
        return StatusType::ALLOCATION_ERROR;
    }
}

template<typename T>
StatusType AVLTree<T>::remove(const T& key) {
    try {
        root = removeNode(std::move(root), key);
        return StatusType::SUCCESS;
    } catch (...) {
        return StatusType::FAILURE;
    }
}

template<typename T>
StatusType AVLTree<T>::search(const T& key) const {
    return get(key).status() == StatusType::SUCCESS ? StatusType::SUCCESS : StatusType::FAILURE;
}

template<typename T>
void AVLTree<T>::printInorder() const {
    printInorderRecursive(root);
    std::cout << std::endl;
}

template<typename T>
std::unique_ptr<Node<T>> AVLTree<T>::insertNode(std::unique_ptr<Node<T>> node, const T& key) {
    if (!node) {
        return std::make_unique<Node<T>>(key);
    }

    if (key < node->key) {
        node->left = insertNode(std::move(node->left), key);
    } else if (key > node->key) {
        node->right = insertNode(std::move(node->right), key);
    } else {
        return node;
    }

    updateHeight(node.get());
    int balance = getBalance(node);

    if (balance > 1 && key < node->left->key) {
        return rotateRight(std::move(node));
    }

    if (balance < -1 && key > node->right->key) {
        return rotateLeft(std::move(node));
    }

    if (balance > 1 && key > node->left->key) {
        node->left = rotateLeft(std::move(node->left));
        return rotateRight(std::move(node));
    }

    if (balance < -1 && key < node->right->key) {
        node->right = rotateRight(std::move(node->right));
        return rotateLeft(std::move(node));
    }

    return node;
}

template<typename T>
std::unique_ptr<Node<T>> AVLTree<T>::removeNode(std::unique_ptr<Node<T>> node, const T& key) {
    if (!node) {
        return node;
    }

    if (key < node->key) {
        node->left = removeNode(std::move(node->left), key);
    } else if (key > node->key) {
        node->right = removeNode(std::move(node->right), key);
    } else {
        if (!node->left || !node->right) {
            node = std::move((node->left) ? node->left : node->right);
        } else {
            Node<T>* minNode = minValueNode(node->right.get());
            node->key = minNode->key;
            node->right = removeNode(std::move(node->right), minNode->key);
        }
    }

    if (!node) return node;

    updateHeight(node.get());
    int balance = getBalance(node);

    if (balance > 1 && getBalance(node->left) >= 0) {
        return rotateRight(std::move(node));
    }

    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(std::move(node->left));
        return rotateRight(std::move(node));
    }

    if (balance < -1 && getBalance(node->right) <= 0) {
        return rotateLeft(std::move(node));
    }

    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rotateRight(std::move(node->right));
        return rotateLeft(std::move(node));
    }

    return node;
}

template<typename T>
Node<T>* AVLTree<T>::minValueNode(Node<T>* node) const {
    Node<T>* current = node;
    while (current->left != nullptr) {
        current = current->left.get();
    }
    return current;
}

template<typename T>
int AVLTree<T>::getHeight(const std::unique_ptr<Node<T>>& node) const {
    return node ? node->height : 0;
}

template<typename T>
int AVLTree<T>::getBalance(const std::unique_ptr<Node<T>>& node) const {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

template<typename T>
std::unique_ptr<Node<T>> AVLTree<T>::rotateRight(std::unique_ptr<Node<T>> y) {
    std::unique_ptr<Node<T>> x = std::move(y->left);
    std::unique_ptr<Node<T>> T2 = std::move(x->right);

    x->right = std::move(y);
    x->right->left = std::move(T2);

    updateHeight(x->right.get());
    updateHeight(x.get());

    return x;
}

template<typename T>
std::unique_ptr<Node<T>> AVLTree<T>::rotateLeft(std::unique_ptr<Node<T>> x) {
    std::unique_ptr<Node<T>> y = std::move(x->right);
    std::unique_ptr<Node<T>> T2 = std::move(y->left);

    y->left = std::move(x);
    y->left->right = std::move(T2);

    updateHeight(y->left.get());
    updateHeight(y.get());

    return y;
}

template<typename T>
void AVLTree<T>::updateHeight(Node<T>* node) {
    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
}

template<typename T>
void AVLTree<T>::printInorderRecursive(const std::unique_ptr<Node<T>>& node) const {
    if (!node) return;
    printInorderRecursive(node->left);
    std::cout << node->key << " ";
    printInorderRecursive(node->right);
}

template<typename T>
void AVLTree<T>::deleteTree(std::unique_ptr<Node<T>>& node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    node.reset();
}
