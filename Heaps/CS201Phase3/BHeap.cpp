using namespace std;
#include <iostream>

template <typename keytype, typename valuetype>
class BNode {
public:
	keytype key;
	valuetype value;
	int degree;
	BNode<keytype, valuetype> *child, *sibling, *parent;
	BNode() {
		degree = 0;
		child = sibling = parent = nullptr;
	};
	BNode(keytype key, valuetype value) : BNode() {
		degree = 0;
		this->key = key;
		this->value = value;
	};
	~BNode(){
		delete child;
		delete sibling;
	};
};

#include "CircularDynamicArray.cpp"

template<typename keytype, typename valuetype>
class BHeap
{
public: 
	BHeap();
	BHeap(keytype k[], valuetype V[], int s);
	~BHeap();
	BHeap<keytype, valuetype>& operator= (const BHeap<keytype, valuetype> &other);
	keytype peakKey();
	valuetype peakValue();
	keytype extractMin();
	void insert(keytype k, valuetype v);
	void insert(BNode<keytype, valuetype>* node);
	void merge(BHeap<keytype, valuetype>& H2);
	int arrayLength();
	void printKey();
	BNode<keytype, valuetype>* GetNodeByIndex(int i); //warning: dangerous
	void RemovePointerAtIndex(int i);
private:
	CircularDynamicArray<BNode<keytype, valuetype>*> array;
	BNode<keytype, valuetype>* mergeTrees(BNode<keytype, valuetype>* b1, BNode<keytype, valuetype>* b2);
	void insertExistingNode(BNode<keytype, valuetype>* node);
	void fixBHeap();
	void shiftArrayDownAt(int i);
	void traversePrintTree(BNode<keytype, valuetype>* node);
};

template<typename keytype, typename valuetype>
BHeap<keytype, valuetype>::BHeap() : array() {
}

template <typename keytype, typename valuetype>
BHeap<keytype, valuetype>::BHeap(keytype k[], valuetype V[], int s) : array()
{
	for (int i = 0; i < s; i++) {
		insert(k[i], V[i]);
	}
}

template <typename keytype, typename valuetype>
BHeap<keytype, valuetype>::~BHeap() {
	for (int i = 0; i < array.length(); i++) {
		delete array[i];
	}
}

template <typename keytype, typename valuetype>
BHeap<keytype, valuetype>& BHeap<keytype, valuetype>::operator= (const BHeap<keytype, valuetype> &other) {
	if (this == &other) {
		return *this;
	}

	for (int i = 0; i < array.length(); i++) {
		delete array[i];
	}
	array.clearCompletely();
	for (int i = 0; i < other.arrayLength(); i++) {
		array.addEnd(other.GetNodeByIndex(i));
	}
	return *this;
}

template <typename keytype, typename valuetype>
void BHeap<keytype, valuetype>::insert(keytype k, valuetype v) {
	//cout << "Adding B0 Tree with a Key of " << k << endl;
	BNode<keytype, valuetype> *node = new BNode<keytype, valuetype>(k, v);
	array.addEnd(node);
	fixBHeap();
}

template <typename keytype, typename valuetype>
void BHeap<keytype, valuetype>::insert(BNode<keytype, valuetype>* node) {
	//cout << "Adding B0 Tree with a Key of " << node->key << endl;
	array.addEnd(node);
	fixBHeap();
}

template <typename keytype, typename valuetype>
void BHeap<keytype, valuetype>::insertExistingNode(BNode<keytype, valuetype>* node)
{
	node->parent = nullptr;
	node->sibling = nullptr;
	array.addEnd(node);
	fixBHeap();
}

template <typename keytype, typename valuetype>
void BHeap<keytype, valuetype>::fixBHeap() {
	if (array.length() <= 1) return;

	//printKey();

	for (int i = array.length() - 1; i > 0; i--) {
		if (array[i]->degree == array[i - 1]->degree) {
			//cout << "Found two bTrees with the same degree, merging them." << endl;
			array[i - 1] = mergeTrees(array[i], array[i - 1]);
			shiftArrayDownAt(i); //shouldn't be popping off the array, should be deleting the ith element.
		}
	}
}

template <typename keytype, typename valuetype>
BNode<keytype, valuetype>* BHeap<keytype, valuetype>::mergeTrees(BNode<keytype, valuetype>* b1, BNode<keytype, valuetype>* b2) {
	if (b1->key > b2->key) {
		array.swap(b1, b2);
		//cout << "Most recent tree added has a bigger key of " << b2->key << ", switching them in the array." << endl;
	}

	b2->parent = b1;
	b2->sibling = b1->child;
	b1->child = b2;
	b1->degree++;
	//cout << "New bTree has a degree of " << b1->degree << endl;

	return b1;
}

template <typename keytype, typename valuetype>
void BHeap<keytype, valuetype>::printKey() {
	for (int i = array.length()-1; i >= 0; i--) {
		cout << "B" << array[i]->degree << endl;
		traversePrintTree(array[i]);
		cout << endl << endl;
	}
	cout << endl;
}

template <typename keytype, typename valuetype>
void BHeap <keytype, valuetype>::traversePrintTree(BNode<keytype, valuetype>* node) {
	cout << node->key << " ";
	if (node->child != nullptr) {
		traversePrintTree(node->child);
	}
	
	if (node->sibling != nullptr) {
		traversePrintTree(node->sibling);
	}
}

template <typename keytype, typename valuetype>
keytype BHeap<keytype, valuetype>::peakKey() {
	BNode<keytype, valuetype>* smallestNode = array[0];
	for (int i = 0; i < array.length(); i++) {
		if (array[i]->key < smallestNode->key) {
			smallestNode = array[i];
		}
	}

	return smallestNode->key;
}

template <typename keytype, typename valuetype>
keytype BHeap<keytype, valuetype>::extractMin() {
	//find the smallest node
	BNode<keytype, valuetype>* smallestNode = array[0];
	int indexOfSmallest = 0;
	for (int i = 0; i < array.length(); i++) {
		if (array[i]->key < smallestNode->key) {
			smallestNode = array[i];
			indexOfSmallest = i;
		}
	}

	shiftArrayDownAt(indexOfSmallest);
	//cout << "smallest node key: " << smallestNode->key << " at index " << indexOfSmallest << endl;

	//add all of the children to the list, 
	//remove the reference to the children from the min and vice versa
	BNode<keytype, valuetype>* child = smallestNode->child;
	BNode<keytype, valuetype>* tempChild;
	BHeap<keytype, valuetype> tempHeap;

	while (child != nullptr) {
		tempChild = child; //this extra pointer is so we can remove the reference to the sibling once we move on to the sibling
		child = child->sibling;

		tempChild->sibling = nullptr;
		tempChild->parent = nullptr;

		tempHeap.insert(tempChild);
	}


	//cout << "temp heap " << endl;
	//tempHeap.printKey();
	//cout << "current heap pre merge" << endl;
	//printKey();

	merge(tempHeap);

	//cout << "current heap post merge" << endl;
	//printKey();

	//get the key, delete the node, return the key. 
	keytype k = smallestNode->key;
	smallestNode->child = nullptr;
	delete smallestNode;
	return k;
}

template <typename keytype, typename valuetype>
void BHeap<keytype, valuetype>::merge(BHeap<keytype, valuetype> &H2) {
	CircularDynamicArray<BNode<keytype, valuetype>*> temp;

	int i = 0;
	int j = 0;
	while (i < arrayLength() && j < H2.arrayLength()) {
		if (GetNodeByIndex(i)->degree >= H2.GetNodeByIndex(j)->degree) {
			//cout << GetNodeByIndex(i)->degree << endl;
			temp.addEnd(GetNodeByIndex(i));
			i++;
		}
		else {
			//cout << H2.GetNodeByIndex(j)->degree << endl;
			temp.addEnd(H2.GetNodeByIndex(j));
			H2.RemovePointerAtIndex(j);
			j++;
		}
	}

	while (i < arrayLength()) {
		//cout << GetNodeByIndex(i)->degree << endl;
		temp.addEnd(GetNodeByIndex(i));
		i++;
	}

	while (j < H2.arrayLength()) {
		//cout << H2.GetNodeByIndex(j)->degree << endl;
		temp.addEnd(H2.GetNodeByIndex(j));
		H2.RemovePointerAtIndex(j);
		j++;
	}

	array = temp;
	fixBHeap();	
	//printKey();
}

template <typename keytype, typename valuetype>
valuetype BHeap<keytype, valuetype>::peakValue() {
	BNode<keytype, valuetype>* smallestNode = array[0];
	for (int i = 0; i < array.length(); i++) {
		if (array[i]->key < smallestNode->key) {
			smallestNode = array[i];
		}
	}

	return smallestNode->value;
}

template <typename keytype, typename valuetype>
void BHeap<keytype, valuetype>::shiftArrayDownAt(int i) {
	for (int j = i; j < array.length() - 1; j++) {
		array[j] = array[j + 1];
	}
	array.delEnd();
}

template <typename keytype, typename valuetype>
BNode<keytype, valuetype>* BHeap<keytype, valuetype>::GetNodeByIndex(int i) {
	return array[i];
}

template <typename keytype, typename valuetype>
void BHeap<keytype, valuetype>::RemovePointerAtIndex(int i) {
	array[i] = nullptr;
}

template <typename keytype, typename valuetype>
int BHeap<keytype, valuetype>::arrayLength() {
	return array.length();
}