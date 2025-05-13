#ifndef AVL_NODE
#define AVL_NODE

typedef enum {
    LEAF,
    HAS_LEFT_CHILD,
    HAS_RIGHT_CHILD,
    HAS_CHILDREN
} NodeType;

template<typename Data>
class AVLNode {
    Data *data;
    bool deleteMemory;
    AVLNode *parentNode;
    AVLNode *rightChild;
    AVLNode *leftChild;

    NodeType type;
    int height;
    int bf;

    void updateType();
public:
    AVLNode(Data *data, bool deleteMemory = true);
    ~AVLNode();

    Data *getData();
    void setData(Data *);

    void setRChild(AVLNode *);
    void setLChild(AVLNode *);
    void setParent(AVLNode *);

    AVLNode *getLChild();
    AVLNode *getRChild();
    AVLNode *getParent();

    //returns true if the child to be killed is the left one
    const bool removeChild(AVLNode *);

    int getBF();
    int getHeight();
    NodeType getType();
    void calculateStats();

    int getId() {
        if (data == nullptr) {
            return 0;
        }
        return data->getId();
    }

    const bool operator<(AVLNode &rhs); //uses comparator
    const bool operator<(Data *data);
    const bool operator<(int id);
    const bool operator>(AVLNode &rhs); //uses comparator
    const bool operator>(Data *data);
    const bool operator>(int id);
    const bool operator==(AVLNode &rhs); //uses comparator
    const bool operator==(Data *data);
    const bool operator==(int id);

};


template<typename Data>
AVLNode<Data>::AVLNode(Data *data, bool deleteMemory) {
    this->data = data;
    this->deleteMemory = deleteMemory;
    this->parentNode = nullptr;
    this->leftChild = nullptr;
    this->rightChild = nullptr;
    this->bf = 0;
    this->height = 0;
    this->type = LEAF;
}


template<typename Data>
AVLNode<Data>::~AVLNode() {
    if (deleteMemory) {
        if (data != nullptr) {
            delete data;
            data = nullptr;
        }
    }
}

template<typename Data>
AVLNode<Data> *AVLNode<Data>::getLChild() {
    return this->leftChild;
}

template<typename Data>
AVLNode<Data> *AVLNode<Data>::getRChild() {
    return this->rightChild;
}

template<typename Data>
AVLNode<Data> *AVLNode<Data>::getParent() {
    return this->parentNode;
}

template<typename Data>
const bool AVLNode<Data>::removeChild(AVLNode *node) {
    if (this->leftChild == node) {
        this->leftChild = nullptr;
        this->calculateStats();
        return true;
    } else if (this->rightChild == node) {
        this->rightChild = nullptr;
        this->calculateStats();
    }

    return false;
}

template<typename Data>
const bool AVLNode<Data>::operator<(AVLNode &rhs) {
    return *(this->data) < *(rhs->getData());
}

template<typename Data>
const bool AVLNode<Data>::operator<(Data *otherData) {
    if (otherData == nullptr) {
        return false;
    }

    return *(this->data) < *otherData;
}

template<typename Data>
const bool AVLNode<Data>::operator<(int id) {
    return *(this->data) < id;
}

template<typename Data>
const bool AVLNode<Data>::operator>(AVLNode &rhs) {
    return *(this->data) > *(rhs->getData());
}

template<typename Data>
const bool AVLNode<Data>::operator>(Data *otherData) {
    if (otherData == nullptr) {
        return true;
    }

    return *(this->data) > *otherData;
}

template<typename Data>
const bool AVLNode<Data>::operator>(int id) {
    return *(this->data) > id;
}

template<typename Data>
const bool AVLNode<Data>::operator==(AVLNode &rhs) {
    return *data == *(rhs->getData());
}

template<typename Data>
const bool AVLNode<Data>::operator==(Data *otherData) {
    if (otherData == nullptr) {
        return false;
    }

    return *(this->data) == *otherData;
}

template<typename Data>
const bool AVLNode<Data>::operator==(int id) {
    return this->getId() == id;
}

template<class Data>
void AVLNode<Data>::calculateStats() {
    this->updateType();

    switch (this->type) {
        case LEAF:
            this->bf = 0;
            this->height = 1;
            break;
        case HAS_CHILDREN:
            this->height = this->leftChild->height >= this->rightChild->height ?
                           this->leftChild->height + 1 :
                           this->rightChild->height + 1;
            this->bf = this->leftChild->height - this->rightChild->height;
            break;
        case HAS_LEFT_CHILD:
            this->height = this->leftChild->height + 1;
            this->bf = this->leftChild->height;
            break;
        case HAS_RIGHT_CHILD:
            this->height = this->rightChild->height + 1;
            this->bf = -1 * this->rightChild->height;
            break;
    }
}

template<typename Data>
int AVLNode<Data>::getBF() {
    calculateStats();
    return this->bf;
}

template<typename Data>
int AVLNode<Data>::getHeight() {
    calculateStats();
    return this->height;
}

template<typename Data>
NodeType AVLNode<Data>::getType() {
    return this->type;
}

template<typename Data>
void AVLNode<Data>::updateType() {
    auto *lchild = this->leftChild;
    auto *rchild = this->rightChild;

    if (lchild == nullptr && rchild == nullptr) {
        type = LEAF;
        return;
    } else if (lchild != nullptr && rchild != nullptr) {
        type = HAS_CHILDREN;
        return;
    } else if (lchild != nullptr) {
        type = HAS_LEFT_CHILD;
        return;
    } else { type = HAS_RIGHT_CHILD; }
}

template<typename Data>
Data *AVLNode<Data>::getData() {
    return this->data;
}

template<typename Data>
void AVLNode<Data>::setData(Data *d) {
    this->data = d;
}


template<typename Data>
void AVLNode<Data>::setRChild(AVLNode<Data> *node) {
    if (this->parentNode == node && node != nullptr) {
        this->setParent(node->getParent());
    }

    this->rightChild = node;
    if (node != nullptr) {
        node->setParent(this);
    }

    calculateStats();

}

template<typename Data>
void AVLNode<Data>::setLChild(AVLNode<Data> *node) {
    if (this->parentNode == node && node != nullptr) {
        this->setParent(node->getParent());
    }

    this->leftChild = node;
    if (node != nullptr) {
        node->setParent(this);
    }

    calculateStats();

}

template<typename Data>
void AVLNode<Data>::setParent(AVLNode *node) {
    this->parentNode = node;
}


#endif //AVL_NODE