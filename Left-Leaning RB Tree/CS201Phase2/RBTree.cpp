#include <iostream>
#include <string>

using namespace std;

#pragma region NodeClass

//define the node templated class
template <typename keytype, typename valuetype>
class Node
{
public:
    keytype key;
    valuetype value;
    int size = 0;
    Node *left; 
    Node *right;
	bool color = true; //red is true, black is false

    Node()
    {
        left = nullptr;
        right = nullptr;
        color = true;
    }

    Node(keytype key, valuetype value)
    {
        this->key = key;
        this->value = value;
        left = nullptr;
        right = nullptr;
        color = true;
    }
    
    Node(keytype key, valuetype value, bool color)
    {
        this->key = key;
        this->value = value;
        this->color = color;
        left = nullptr;
        right = nullptr;
    }
    ~Node()
    {
        delete left;
        delete right;
    }

    void print()
    {
        cout << "Key: " << key << ", Value: " << value << ", Color: " << color << ", Size of Subtree: " << size << endl;
    }
};

#pragma endregion NodeClass

#pragma region RBTClassDeclarations

template <typename keytype, typename valuetype>
class RBTree
{
#pragma region Constants

    static const bool RED = true;
    static const bool BLACK = false;

#pragma endregion Constants

public:
    RBTree();
    RBTree(keytype K[], valuetype V[], int s);
    RBTree(const RBTree &other);
    RBTree &operator=(const RBTree &other);
    ~RBTree();
    valuetype *search(keytype k);
    valuetype *search(Node<keytype, valuetype>* h, keytype k);
    keytype min();
    keytype min(Node<keytype, valuetype>* h);
    void insert(keytype k, valuetype v);
    int remove(keytype k);
    void deleteMax();
    void deleteMin();
    int rank(keytype k);
    keytype select(int pos);
    //void split(keytype k, RBTree<keytype, valuetype>& T1, RBTree<keytype, valuetype>& T2);
    int size();
    void preorder();
    void inorder();
    void postorder();
    Node<keytype, valuetype> *root;

private:
    int m_size;
    bool isRed(Node<keytype, valuetype> *x);
	Node<keytype, valuetype> *insert(Node<keytype, valuetype> *h, keytype k, valuetype v);
    Node<keytype, valuetype> *removeRecursively(Node<keytype, valuetype> *h, keytype k);
    Node<keytype, valuetype> *deleteMax(Node<keytype, valuetype>* h);
    Node<keytype, valuetype> *deleteMin(Node<keytype, valuetype>* h);
    Node<keytype, valuetype> *rotateLeft(Node<keytype, valuetype> *h);
    Node<keytype, valuetype> *rotateRight(Node<keytype, valuetype> *h);
    Node<keytype, valuetype> *moveRedRight(Node<keytype, valuetype> *h);
    Node<keytype, valuetype> *moveRedLeft(Node<keytype, valuetype> *h);
    Node<keytype, valuetype> *colorFlip(Node<keytype, valuetype> *h);
    Node<keytype, valuetype> *fixUp(Node<keytype, valuetype> *h);
	void calcSizeOfNode(Node<keytype, valuetype> *x);
	int	sizeOfNode(Node<keytype, valuetype> *x);
    void preorder(Node<keytype, valuetype> *h);
    void inorder(Node<keytype, valuetype> *h);
    void postorder(Node<keytype, valuetype> *h);
	keytype errorKey;
	valuetype errorValue;
};

#pragma endregion RBTClassDeclarations

#pragma region Constructors

template <typename keytype, typename valuetype>
RBTree<keytype, valuetype>::RBTree()
{
    root = nullptr;
    m_size = 0;
}

template <typename keytype, typename valuetype>
RBTree<keytype, valuetype>::RBTree(keytype K[], valuetype V[], int s) : RBTree<keytype, valuetype>()
{
	for (int i = 0; i < s; i++)
    {
        insert(K[i], V[i]);
    }

    m_size = s;
}

template <typename keytype, typename valuetype>
RBTree<keytype, valuetype>::RBTree(const RBTree &other)
{
    delete root;
    root = other->root;
    m_size = other.size();
}

template <typename keytype, typename valuetype>
RBTree<keytype, valuetype> &RBTree<keytype, valuetype>::operator=(const RBTree &other)
{
    if (this == &other)
    {
        return *this;
    }

    delete root;
    root = other->root;
    m_size = other.size();

	return *this;
}

template <typename keytype, typename valuetype>
RBTree<keytype, valuetype>::~RBTree()
{
    delete root;
}

#pragma endregion Constructors

#pragma region Search

template <typename keytype, typename valuetype>
valuetype *RBTree<keytype, valuetype>::search(keytype k)
{
    Node<keytype, valuetype>* x = root;
    while (x != nullptr)
    {
        if (k == x->key)
        {
			return &(x->value);
        }
        else if (k < x->key)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }

    return nullptr;
}

template <typename keytype, typename valuetype>
valuetype *RBTree<keytype, valuetype>::search(Node<keytype, valuetype>* h, keytype k)
{
    Node<keytype, valuetype>* x = h;
    while (x != nullptr)
    {
        if (k == x->key)
        {
            return &(x->value);
        }
        else if (k < x->key)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }

    return nullptr;
}

//finds the minimum key in the tree. could be modified to return the data stored in the node
template <typename keytype, typename valuetype>
keytype RBTree<keytype, valuetype>::min()
{
    Node<keytype, valuetype>* x = root;
    while (x->left != nullptr)
    {
        x = x->left;
    }

	return x->key;
}

template <typename keytype, typename valuetype>
keytype RBTree<keytype, valuetype>::min(Node<keytype, valuetype>* h)
{
	Node<keytype, valuetype>* x = h;
    while (x->left != nullptr)
    {
        x = x->left;
    }

	return x->key;
}

#pragma endregion Search

#pragma region OrderStatistics

//DOESNT WORK COMPLETELY
//should return the rank of the key k in the tree, or 0 if its not found
template <typename keytype, typename valuetype>
int RBTree<keytype, valuetype>::rank(keytype k){
	//start by calculating size of nodes. don't need to do this during insert or delete
	if (root->size != m_size) {
		calcSizeOfNode(root);
	}
	
	if (search(k) == nullptr) {
		return 0;
	}

	int rank = root->size;
	bool foundRank = false;
	Node<keytype, valuetype>* x = root;
	while (!foundRank)
	{
		if (k == x->key)
		{
			if (x->right != nullptr) {
				rank = rank - x->right->size;
			}

			foundRank = true;
		}
		else if (k < x->key)
		{
			if (x->right != nullptr) {
				rank = rank - x->right->size - 1;
			}
			x = x->left;
		}
		else
		{
			x = x->right;
		}
	}

	return rank;
}

//DOESNT WORK COMPLETELY
//returns the key of the of the node at position pos in the tree.
//calling pos = 1 should return smallest key
//calling pos = n should return the largest

template <typename keytype, typename valuetype>
keytype RBTree<keytype, valuetype>::select(int pos)
{
	if (root->size != m_size) {
		calcSizeOfNode(root);
	}

	if (pos < 1 || pos > m_size) {
		cout << "Error, trying to access a position out of bounds the tree." << endl;
		return errorKey;
	}

	bool foundKey = false;
	Node<keytype, valuetype>* x = root;
	while (!foundKey)
	{
		int rank = sizeOfNode(x->left) + 1;
		if (pos == rank)
		{
			foundKey = true;
		}
		else if (pos < rank)
		{
			x = x->left;
		}
		else
		{
			pos = pos - rank;
			x = x->right;
		}

	}
	return x->key;
}

#pragma endregion OrderStatistics

#pragma region Insertion

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::insert(keytype k, valuetype v)
{
    root = insert(root, k, v);
    root->color = BLACK;
}

template <typename keytype, typename valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::insert(Node<keytype, valuetype> *h, keytype k, valuetype v)
{
    if (h == nullptr)
    {
		m_size++;
		return new Node<keytype, valuetype>(k, v, RED);
    }

    //split four nodes on the way down the tree
    if (isRed(h->left) && isRed(h->right))
    {
        colorFlip(h);
    }

    if (k == h->key)
    {
        //we have discovered a duplicate key. write over the value
        h->value = v;
    }
    else if (k < h->key)
    {
        h->left = insert(h->left, k, v);
    }
    else
    {
        h->right = insert(h->right, k, v);
    }

    //fix right leaning reds on the way up the tree
    if (isRed(h->right))
    {
        h = rotateLeft(h);
    }

    if (isRed(h->left) && isRed(h->left->left))
    {
        h = rotateRight(h);
    }

	return h;
}

#pragma endregion Insertion

#pragma region Deletion

//removes the node with key k and returns 1. should return 0 if the key k is not found.
template <typename keytype, typename valuetype>
int RBTree<keytype, valuetype>::remove(keytype k)
{
	//TODO: change delete to search first, if it can't find it then return 0
	if (search(k) == nullptr) {
		return 0;
	}

	root = removeRecursively(root, k);
	root->color = BLACK;
	return 1;
}

template <typename keytype, typename valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::removeRecursively(Node<keytype, valuetype> *h, keytype k)
{
    if (k < h->key)
    {
        if (!isRed(h->left) && !isRed(h->left->left))
        {
            h = moveRedLeft(h);
        }
        h->left = removeRecursively(h->left, k);
    }
    else
    {
        if (isRed(h->left))
        {
            h = rotateRight(h);
        }

        if (k == h->key && (h->right == nullptr))
        {
			m_size--;
			return nullptr;
        }

        if(!isRed(h->right) && !isRed(h->right->left)){
            h = moveRedRight(h);
        }

        if(k == h->key){
            h->key = min(h->right);
            h->value = *(search(h->right, h->key));
            h->right = deleteMin(h->right);
        }else{
			h->right = removeRecursively(h->right, k);
        }
    }

    return fixUp(h);
}

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::deleteMin(){
    root = deleteMin(root);
    root->color = BLACK;
}

template <typename keytype, typename valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::deleteMin(Node<keytype, valuetype>* h){
    if(h->left == nullptr){
		m_size--;
		return nullptr;
    }

    if(!isRed(h->left) && !isRed(h->left->left)){
        h = moveRedLeft(h);
    }

    h->left = deleteMin(h->left);

    return fixUp(h);
}

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::deleteMax(){
    root = deleteMax(root);
    root->color = BLACK;
}

template <typename keytype, typename valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::deleteMax(Node<keytype, valuetype>* h){
    if(isRed(h->left)){
        h = rotateRight(h);
    }

    if(h->right == nullptr){
		m_size--;
		return nullptr;
    }

    if(!isRed(h->right) && !isRed(h->right->left)){
        h = moveRedRight(h);
    }

    h->left = deleteMax(h->left);
    return fixUp(h);
}

#pragma endregion Deletion

#pragma region Rotations

template <typename keytype, typename valuetype>
Node<keytype, valuetype> *RBTree<keytype, valuetype>::rotateLeft(Node<keytype, valuetype> *h)
{
    Node<keytype, valuetype> *x = h->right;
    h->right = x->left;
    x->left = h;
    x->color = x->left->color;
    x->left->color = RED;
    return x;
}

template <typename keytype, typename valuetype>
Node<keytype, valuetype> *RBTree<keytype, valuetype>::rotateRight(Node<keytype, valuetype> *h)
{
    Node<keytype, valuetype> *x = h->left;
    h->left = x->right;
    x->right = h;
    x->color = x->right->color;
    x->right->color = RED;
    return x;
}

template <typename keytype, typename valuetype>
Node<keytype, valuetype> *RBTree<keytype, valuetype>::colorFlip(Node<keytype, valuetype> *x)
{
	if (x != nullptr) {
		x->color = !x->color;
	}
	if (x->right != nullptr) {
		x->right->color = !x->right->color;
	}
	if (x->left != nullptr) {
		x->left->color = !x->left->color;
	}

    return x;
}

template <typename keytype, typename valuetype>
Node<keytype, valuetype> *RBTree<keytype, valuetype>::moveRedRight(Node<keytype, valuetype> *h)
{
    colorFlip(h);
    if (h->left != nullptr && isRed(h->left->left))
    {
        h = rotateRight(h);
        colorFlip(h);
    }
    return h;
}

template <typename keytype, typename valuetype>
Node<keytype, valuetype> *RBTree<keytype, valuetype>::moveRedLeft(Node<keytype, valuetype> *h)
{
    colorFlip(h);
	if (h->right != nullptr && isRed(h->right->left)) {
		h->right = rotateRight(h->right);
		h = rotateLeft(h);
		colorFlip(h);
	}
    return h;
}

template <typename keytype, typename valuetype>
Node<keytype, valuetype> *RBTree<keytype, valuetype>::fixUp(Node<keytype, valuetype> *h)
{
    if (isRed(h->right))
    {
        h = rotateLeft(h);
    }
    if (isRed(h->left) && isRed(h->left->left))
    {
        h = rotateRight(h);
    }
    if (isRed(h->left) && isRed(h->right))
    {
        colorFlip(h);
    }
    return h;
}

#pragma endregion Rotations

#pragma region Helpers

template <typename keytype, typename valuetype>
bool RBTree<keytype, valuetype>::isRed(Node<keytype, valuetype> *x)
{
	if (x == nullptr)
    {
        return false;
    }
    return x->color == RED;
}

template <typename keytype, typename valuetype>
int RBTree<keytype, valuetype>::size()
{
    return m_size;
}

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::calcSizeOfNode(Node<keytype, valuetype> *x) {
	if (x != nullptr) {
		x->size = 1;
		
		if (x->left != nullptr) {
			calcSizeOfNode(x->left);
			x->size += x->left->size;
		}

		if (x->right != nullptr) {
			calcSizeOfNode(x->right);
			x->size += x->right->size;
		}
	}
}

template <typename keytype, typename valuetype>
int RBTree<keytype, valuetype>::sizeOfNode(Node<keytype, valuetype> *x) {
	if (x == nullptr) {
		return 0;
	}
	return x->size;
}

#pragma endregion Helpers

#pragma region Traversals

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::preorder()
{
    preorder(root);
    cout << endl;
}

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::inorder()
{
    inorder(root);
    cout << endl;
}

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::postorder()
{
    postorder(root);
    cout << endl;
}

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::preorder(Node<keytype, valuetype> *h)
{
    if (h == nullptr)
    {
        return;
    }

    cout << h->key << " ";
    preorder(h->left);
    preorder(h->right);
}

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::inorder(Node<keytype, valuetype> *h)
{
    if (h == nullptr)
    {
        return;
    }

    inorder(h->left);
    cout << h->key << " ";
    inorder(h->right);
}

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::postorder(Node<keytype, valuetype> *h)
{
    if (h == nullptr)
    {
        return;
    }

    postorder(h->left);
    postorder(h->right);
    cout << h->key << " ";
}

#pragma endregion Traversals