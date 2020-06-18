using namespace std;

#include <iostream>
#include "CircularDynamicArray.cpp"

template <typename keytype, typename valuetype>
class Node {
public:
	keytype key;
	valuetype value;
	Node() {
		
	};
	Node(keytype key, valuetype value) {
		this->key = key;
		this->value = value;
	};
};



template <typename keytype, typename valuetype>
class Heap
{
public:
	Heap();
	Heap(keytype k[], valuetype V[], int s);
	~Heap();
	keytype peakKey();
	valuetype peakValue();
	keytype extractMin(); //remove min key and return it
	void insert(keytype k, valuetype v);
	void printKey();
private:
	int m_size = 0;
	CircularDynamicArray<Node<keytype, valuetype>> array;
	int parentIndex(int i);
	int leftChildIndex(int i);
	int rightChildIndex(int i);
	void heapify(int i);
};

template <typename keytype, typename valuetype>
Heap<keytype, valuetype>::Heap() : array()
{
	m_size = 0;
}

template <typename keytype, typename valuetype>
Heap<keytype, valuetype>::Heap(keytype k[], valuetype V[], int s) : array()
{
	//bottom up heap building from clrs for O(n) runtimes

	m_size = s;
	
	//add nodes in order that they are in the array
	for (int i = 0; i < s; i++) {
		Node<keytype, valuetype> node(k[i], V[i]);
		array.addEnd(node);
	}

	//for every non-leaf node, heapify (find the smallest between a node and it's children, flip it if necesary. )
	for (int i = ((s - 1) / 2); i >= 0; i--) {
		heapify(i);
	}
}

template <typename keytype, typename valuetype>
Heap<keytype, valuetype>::~Heap()
{

}

template <typename keytype, typename valuetype>
void Heap<keytype, valuetype>::insert(keytype k, valuetype v) {
	m_size++;
	Node<keytype, valuetype> node(k, v);
	array.addEnd(node);

	int i = m_size - 1;
	while (i != 0 && array[parentIndex(i)].key > array[i].key) {
		//cout << "Swapping key " << array[i].key << " with parent " << array[parentIndex(i)].key << endl;
		array.swap(parentIndex(i), i);
		i = parentIndex(i);
	}
}

template<typename keytype, typename valuetype>
keytype Heap<keytype, valuetype>::peakKey() {
	return array[0].key;
}

template<typename keytype, typename valuetype>
valuetype Heap<keytype, valuetype>::peakValue() {
	return array[0].value;
}

template<typename keytype, typename valuetype>
keytype Heap<keytype, valuetype>::extractMin() {
	//pop the front, delete it. 
	Node<keytype, valuetype> front = array[0];
	array.delFront();
	m_size--;
	
	//move the end to the front
	Node<keytype, valuetype> end = array[m_size - 1];
	array.addFront(end);
	array.delEnd();

	heapify(0);

	return front.key;
}

template<typename keytype, typename valuetype>
void Heap<keytype, valuetype>::heapify(int i) {
	int smallest = i;
	int left = leftChildIndex(i);
	int right = rightChildIndex(i);

	if (left < m_size && array[left].key < array[smallest].key) {
		smallest = left;
	}

	if (right < m_size && array[right].key < array[smallest].key) {
		smallest = right;
	}

	if (smallest != i) {
		array.swap(i, smallest);
		heapify(smallest);
	}
}

//returns the index of the parent given a current child index in the array.
template <typename keytype, typename valuetype>
int Heap<keytype, valuetype>::parentIndex(int i) {
	return (i - 1) / 2;
}

//returns the index of the left child given a current parent index in the array.
template <typename keytype, typename valuetype>
int Heap<keytype, valuetype>::leftChildIndex(int i) {
	return (2 * i) + 1;
}

//returns the index of the right child given a current parent index in the array.
template <typename keytype, typename valuetype>
int Heap<keytype, valuetype>::rightChildIndex(int i) {
	return (2 * i) + 2;
}

template <typename keytype, typename valuetype>
void Heap<keytype, valuetype>::printKey() {
	for (int i = 0; i < m_size; i++) {
		cout << array[i].key << " ";
	}
	cout << endl;
}