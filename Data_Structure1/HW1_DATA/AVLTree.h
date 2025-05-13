#ifndef AVL_TREE
#define AVL_TREE

#include "AVLNode.h"

enum class TreeResult {
    SUCCESS,
    NODE_DOESNT_EXISTS,
    NODE_ALREADY_EXISTS,
    OUT_OF_MEMORY,
    nullptr_ARGUMENT,
    FAILURE
};

/// <summary>
/// AVLTree generic class, that works with Data and uses Comparator for insertion/deletion.
/// </summary>
/// <typeparam name="Data">Data class type that wraps the generic data
/// Data is required to support < > == on:
///                                        Data other*
///                                        int id
/// </typeparam>
template<typename Data>
class AVLTree {
    AVLNode<Data> *root;
    int nodes_count;

    AVLNode<Data> *highest;

    //if found, return node, else, return father node
    AVLNode<Data> *findNode(Data *data, bool &found);
    AVLNode<Data> *findNode(const int id, bool &found);

    AVLNode<Data> *llRotation(AVLNode<Data> *node);
    AVLNode<Data> *rrRotation(AVLNode<Data> *node);
    AVLNode<Data> *rlRotation(AVLNode<Data> *node);
    AVLNode<Data> *lrRotation(AVLNode<Data> *node);
    TreeResult balanceTree(AVLNode<Data> *node); //check parent

public:
    AVLTree();
    AVLTree(AVLNode<Data> *r, int size);
    ~AVLTree();

    bool isEmpty() {
        if (root != nullptr) {
            return false;
        }
        return true;
    }

    //if not found, return nullptr
    Data *findData(const int identifier);
    AVLNode<Data> *findNode(const int id);

    const TreeResult insertNode(Data *data, bool deleteMemory = true);
    const TreeResult deleteNode(int id);
    const TreeResult
    deleteByPointer(AVLNode<Data> *node);
    const int getSize();
    void updateHighest();

    Data *getHighest() {
        if (!highest) { return nullptr; }
        return highest->getData();
    }

    AVLNode<Data> *getRoot() { return root; }

    int inorder(AVLNode<Data> *p, Data **arr, int numOfNodes, int i = 0);
    Data **orderedArray();

    //int treeHeight() { return root->getHeight(); }

    //class nullptrArg : public std::exception {};
    //class AllocError : public std::exception {};
};


template<typename Data>
AVLNode<Data> *AVLTree<Data>::findNode(Data *data, bool &found) {
    if (data == nullptr) {
        return nullptr;
    }

    AVLNode<Data> *node = this->root;
    while (node != nullptr) {
        if (*node->getData() == *data) {
            found = true;
            break;
        } else if (*node->getData() > *data) //search within left subtree
        {
            if (node->getLChild() == nullptr) {
                found = false;
                return node;
            }

            node = node->getLChild();
        } else //search within right subtree
        {
            if (node->getRChild() == nullptr) {
                found = false;
                return node;
            }

            node = node->getRChild();
        }
    }

    return node;
}

template<typename Data>
AVLNode<Data> *AVLTree<Data>::findNode(const int id, bool &found) {
    AVLNode<Data> *node = this->root;
    while (node != nullptr) {
        if (*node == id) {
            found = true;
            break;
        } else if (*(node->getData()) > id) //search within left subtree
        {
            if (node->getLChild() == nullptr) {
                found = false;
                return node;
            }

            node = node->getLChild();
        } else //search within right subtree
        {
            if (node == nullptr) {
                found = false;
                return node;
            }

            node = node->getRChild();
        }
    }

    return node;
}

template<typename Data>
AVLNode<Data> *AVLTree<Data>::findNode(const int id) {
    bool found;
    AVLNode<Data> *node = findNode(id, found);
    if (found) {
        return node;
    }
    return nullptr;
}

template<typename Data>
static bool is_left_child(AVLNode<Data> *parent, AVLNode<Data> *child) {
    if (parent == nullptr) { return false; }
    return parent->getLChild() == child;
}

template<typename Data>
static bool is_right_child(AVLNode<Data> *parent, AVLNode<Data> *child) {
    if (parent == nullptr) { return false; }
    return parent->getRChild() == child;
}

template<typename Data>
AVLNode<Data> *AVLTree<Data>::llRotation(AVLNode<Data> *node) {
    if (node == nullptr) {
        return nullptr;
    }

    AVLNode<Data> *new_node = node->getLChild();
    node->setLChild(new_node->getRChild());

    //update grandpa link
    auto *grandpa = node->getParent();
    if (is_left_child(grandpa, node)) {
        grandpa->setLChild(new_node);
    } else if (is_right_child(grandpa, node)) {
        grandpa->setRChild(new_node);
    }

    new_node->setRChild(node);
    if (this->root == node) { //update root if needed
        this->root = new_node;
    }
    return new_node;
}

template<typename Data>
AVLNode<Data> *AVLTree<Data>::rrRotation(AVLNode<Data> *node) {
    if (node == nullptr) {
        return nullptr;
    }

    AVLNode<Data> *new_node = node->getRChild();
    node->setRChild(new_node->getLChild());

    //update grandpa link
    auto *grandpa = node->getParent();
    if (is_left_child(grandpa, node)) {
        grandpa->setLChild(new_node);
    } else if (is_right_child(grandpa, node)) {
        grandpa->setRChild(new_node);
    }

    new_node->setLChild(node);
    if (this->root == node) { //update root if needed
        this->root = new_node;
    }
    return new_node;
}

template<typename Data>
AVLNode<Data> *AVLTree<Data>::rlRotation(AVLNode<Data> *node) {
    if (node == nullptr) {
        return nullptr;
    }

    AVLNode<Data> *new_node = node->getRChild();
    node->setRChild(llRotation(new_node));
    return rrRotation(node);
}

template<typename Data>
AVLNode<Data> *AVLTree<Data>::lrRotation(AVLNode<Data> *node) {
    if (node == nullptr) {
        return nullptr;
    }

    AVLNode<Data> *new_node = node->getLChild();
    node->setLChild(rrRotation(new_node));
    return llRotation(node);
}

template<typename Data>
TreeResult AVLTree<Data>::balanceTree(AVLNode<Data> *node) {
    if (node == nullptr) { return TreeResult::nullptr_ARGUMENT; }

    int bf = node->getBF();
    if (bf > 1) {
        int left_bf = node->getLChild()->getBF();
        if (left_bf > 0) {
            node = llRotation(node);
            if (node == root) {
                return TreeResult::SUCCESS;
            } else {
                return balanceTree(node->getParent());
            }
        } else {
            node = lrRotation(node);
            if (node == root) {
                return TreeResult::SUCCESS;
            } else {
                return balanceTree(node->getParent());
            }
        }
    } else if (bf < -1) {
        int right_bf = node->getRChild()->getBF();
        if (right_bf > 0) {
            node = rlRotation(node);
            if (node == root) {
                return TreeResult::SUCCESS;
            } else {
                return balanceTree(node->getParent());
            }
        } else {
            node = rrRotation(node);
            if (node == root) {
                return TreeResult::SUCCESS;
            } else {
                return balanceTree(node->getParent());
            }
        }
    } else {
        if (node == root) {
            return TreeResult::SUCCESS;
        } else {
            return balanceTree(node->getParent());
        }

    }
    return TreeResult::FAILURE;
}

template<typename Data>
static AVLNode<Data> *get_replacement(AVLNode<Data> *root) {
    AVLNode<Data> *node = root->getRChild();
    while (node->getLChild() != nullptr) {
        node = node->getLChild();
    }

    return node;
}

template<typename Data>
static void swap_nodes(AVLNode<Data> *n1, AVLNode<Data> *n2) {
    auto *data = n1->getData();
    n1->setData(n2->getData());
    n2->setData(data);

}

template<typename Data>
static void delete_leaf_or_onechild_node(AVLNode<Data> *node) {
    auto *parent = node->getParent();

    switch (node->getType()) {
        case LEAF:
            parent->removeChild(node);
            delete node;
            node = nullptr;
            break;

        case HAS_LEFT_CHILD:
            if (parent->removeChild(node) ==
                true) { //node is parent's left child
                parent->setLChild(node->getLChild());
            } else { parent->setRChild(node->getLChild()); }
            delete node;
            node = nullptr;
            break;

        case HAS_RIGHT_CHILD:
            if (parent->removeChild(node) ==
                true) { //node is father's left child
                parent->setLChild(node->getRChild());
            } else { parent->setRChild(node->getRChild()); }
            delete node;
            node = nullptr;
            break;

        default:
            break;
    }
}

template<typename Data>
AVLTree<Data>::AVLTree() {
    root = nullptr;
    nodes_count = 0;
    highest = nullptr;
}

template<typename Data>
inline AVLTree<Data>::AVLTree(AVLNode<Data> *r, int size) {
    this->root = r;
    this->nodes_count = size;
    this->updateHighest();
}

template<typename Data>
Data *AVLTree<Data>::findData(const int identifier) {
    bool found = false;
    AVLNode<Data> *node = findNode(identifier, found);

    if (found) {
        return node->getData();
    }

    return nullptr;
}

template<typename Data>
const TreeResult
AVLTree<Data>::insertNode(Data *data, bool deleteMemory) {

    if (data == nullptr) { return TreeResult::nullptr_ARGUMENT; }

    if (root == nullptr) {
        root = new AVLNode<Data>(data, deleteMemory);
        this->nodes_count++;
        this->highest = root;
        return TreeResult::SUCCESS;
    }

    bool found = false;
    AVLNode<Data> *node = findNode(data, found);

    if (found) {
        return TreeResult::NODE_ALREADY_EXISTS;
    } else if (*node < data) {
        AVLNode<Data> *newNode = new AVLNode<Data>(data, deleteMemory);

        this->nodes_count++;
        node->setRChild(newNode);
        if (balanceTree(newNode) != TreeResult::SUCCESS) {
            return TreeResult::FAILURE;
        }

        if (*highest < data) {
            this->highest = newNode;
        }

        return TreeResult::SUCCESS;
    } else if (*node > data) {
        AVLNode<Data> *newNode = new AVLNode<Data>(data, deleteMemory);
        this->nodes_count++;
        node->setLChild(newNode);
        if (balanceTree(newNode) != TreeResult::SUCCESS) {
            return TreeResult::FAILURE;
        }

        return TreeResult::SUCCESS;
    }
    return TreeResult::FAILURE;

}

template<typename Data>
inline const TreeResult AVLTree<Data>::deleteNode(int id) {
    bool found = false;
    AVLNode<Data> *node = findNode(id, found);
    if (!found) {
        return TreeResult::NODE_DOESNT_EXISTS;
    }

    return deleteByPointer(node);
}

template<typename Data>
inline const TreeResult
AVLTree<Data>::deleteByPointer(AVLNode<Data> *node) {
    auto *parent = node->getParent();

    switch (node->getType()) {
        case LEAF:

            if (this->root == node) {
                this->root = nullptr;
                delete node;
                node = nullptr;
            } else {
                delete_leaf_or_onechild_node(node);
            }
            break;
        case HAS_LEFT_CHILD:

            if (this->root == node) {
                this->root = node->getLChild();
                this->root->setParent(nullptr);
                delete node;
                node = nullptr;
            } else {
                delete_leaf_or_onechild_node(node);
            }
            break;
        case HAS_RIGHT_CHILD:

            if (this->root == node) {
                this->root = node->getRChild();
                this->root->setParent(nullptr);
                delete node;
                node = nullptr;
            } else {
                delete_leaf_or_onechild_node(node);
            }
            break;
        case HAS_CHILDREN:
            AVLNode<Data> *replacement = get_replacement(node);
            swap_nodes(node, replacement);
            parent = replacement->getParent();
            delete_leaf_or_onechild_node(replacement);
            break;
    }

    balanceTree(parent);
    this->nodes_count--;
    updateHighest();
    return TreeResult::SUCCESS;
}

template<typename Data>
const int AVLTree<Data>::getSize() {
    return this->nodes_count;
}

template<typename Data>
inline void AVLTree<Data>::updateHighest() {
    if (this->nodes_count == 0) {
        this->highest = nullptr;
        return;
    }

    AVLNode<Data> *temp = this->root;
    while (temp->getRChild() != nullptr) {
        temp = temp->getRChild();
    }

    this->highest = temp;
}

template<typename Data>
int
AVLTree<Data>::inorder(AVLNode<Data> *p, Data **arr, int numOfNodes, int i) {
    if (p == nullptr || !numOfNodes) { return i; }

    i = AVLTree<Data>::inorder(p->getLChild(), arr, numOfNodes, i);
    --numOfNodes;
    arr[i++] = (p->getData());
    i = AVLTree<Data>::inorder(p->getRChild(), arr, numOfNodes, i);

    return i;
}

template<typename Data>
Data **AVLTree<Data>::orderedArray() {
    const int size = this->getSize();
    Data **arr = new Data *[size];
    this->inorder(this->root, arr, size);
    return arr;
}

template<typename Data>
static void deleteNodes(AVLNode<Data> *root) {
    if (root == nullptr) {
        return;
    }

    deleteNodes(root->getLChild());
    deleteNodes(root->getRChild());
    if (root != nullptr) {
        delete root;
        root = nullptr;
    }
}

template<typename Data>
AVLTree<Data>::~AVLTree() {
    deleteNodes(this->root);
}

#endif //AVL_TREE