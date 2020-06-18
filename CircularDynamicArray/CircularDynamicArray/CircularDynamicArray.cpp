/**
 * Data Structure that includes amortized O(1) array growth and also
 * adding to the front and back in O(1).
 * 
 * Support includes different sorts and algorithms relating to order statistics. 
 * 
 * Author: Colin Sanders
 * Version: 1.0
 */

using namespace std;

template <typename T>
class CircularDynamicArray
{
public:
    CircularDynamicArray();
    CircularDynamicArray(int s);
    CircularDynamicArray(const CircularDynamicArray &other);
    CircularDynamicArray& operator=(const CircularDynamicArray &other);
    ~CircularDynamicArray();
    T &operator[](int index);
    void addEnd(T element);
    void addFront(T element);
    void delEnd();
    void delFront();
    int length() const;
    int capacity() const;
    void clear();
    T QuickSelect(int k);
    T WCSelect(int k);
    void stableSort();
    void radixSort(int i);
    int linearSearch(T element);
    int binSearch(T element);
    void print();
    T &getElement(int index);
private:
    //private variables
    T *array;
    int frontIndex = 0;
    int endIndex = 0;
    int m_length;   //same as count, num of elements in array
    int m_capacity; //possible amount of elements in array
    T errorElem;

    //accessor functions
    int correctIndex(int i);

    //size change functions
    void growArray();
    void shrinkArray();
    void clearCompletely();

    //quickselect recursive
    T qsRecursive(int left, int right, int k);

    //wcselect recursive
    T wcRecursive(int left, int right, int k);

    //mergesort recursive
    void mergeSort(int left, int right);
    void merge(int left, int middle, int right);
    //next two are overloads to sort ANY array. used to sort medians in wcSelect
    void mergeSort(T* array, int left, int right);
    void merge(T* array, int left, int middle, int right);

    //sorting and selecting helper functions
    int partition(int left, int right);
    int partition(int left, int right, T partitionElement);
    void swap(T &a, T &b);

    //binarysearch recursive
    int binSearchRecursive(int left, int right, T key);

    //countingSort for radix
    void bitCountingSort(int bit);
};

#pragma region Constuctors

//default constructor, should default to size 2
template <typename T>
CircularDynamicArray<T>::CircularDynamicArray()
{
    array = new T[2];
    m_length = 0;
    m_capacity = 2;
}

//
template <typename T>
CircularDynamicArray<T>::CircularDynamicArray(int s)
{
    array = new T[s];
    m_length = s;
    m_capacity = s;
}

template <typename T>
CircularDynamicArray<T>::CircularDynamicArray(const CircularDynamicArray<T> &other)
{
    array = new T[other.capacity()];
    m_length = other.length();
    m_capacity = other.capacity();
    for(int i = 0; i < m_length; i++){
        array[i] = other.array[(other.frontIndex + i) % other.m_capacity];
        endIndex++;
    }
}

template <typename T>
CircularDynamicArray<T>& CircularDynamicArray<T>::operator= (const CircularDynamicArray<T> &other){
    if(this == &other){
        return *this;
    }

    delete[] array;
    array = new T[other.capacity()];
    m_length = other.length();
    m_capacity = other.capacity();
    frontIndex = 0;
    endIndex = 0;
    for(int i = 0; i < m_length; i++){
        array[i] = other.array[(other.frontIndex + i) % other.m_capacity];
        endIndex++;
    }
	return *this;
}

//destructor
template <typename T>
CircularDynamicArray<T>::~CircularDynamicArray()
{
    delete[] array;
}

#pragma endregion Constructors

#pragma region ArrayAccess

//returns a reference to an object in the array
template <typename T>
T &CircularDynamicArray<T>::operator[](int index)
{
    if(index < 0 || index >= m_length){
        cout << endl << "Error: Out of bounds index." << endl << endl;
        return errorElem;
    }
    return array[(frontIndex + index) % m_capacity];
}

template <typename T>
int CircularDynamicArray<T>::correctIndex(int i)
{
    return ((i + m_capacity) % m_capacity);
}

template <typename T>
T &CircularDynamicArray<T>::getElement(int index)
{
    return array[(index + frontIndex) % m_capacity];
}

#pragma endregion ArrayAccess

#pragma region AdjustSize

template <typename T>
void CircularDynamicArray<T>::growArray()
{
    if(m_capacity == 0){
        m_capacity = 1;
    }
    int newCapacity = m_capacity * 2;
    T *newArray = new T[newCapacity];
    for (int i = 0; i < m_length; i++)
    {
        newArray[i] = getElement(i);
    }
    array = newArray;
    m_capacity = newCapacity;
    frontIndex = 0;
    endIndex = m_length; //maybe needs to be m_length+1 or -1;
}

template <typename T>
void CircularDynamicArray<T>::shrinkArray()
{
    int newCapacity = m_capacity / 2;
    T *newArray = new T[newCapacity];
    for (int i = 0; i < m_length; i++)
    {
        newArray[i] = array[(frontIndex + i) % m_capacity];
    }
    array = newArray;
    m_capacity = newCapacity;
    frontIndex = 0;
    endIndex = m_length;
}

#pragma endregion AdjustSize

#pragma region AddDeleteElements

template <typename T>
void CircularDynamicArray<T>::addEnd(T element)
{
    if (m_length == m_capacity)
    {
        growArray();
    }

    array[endIndex] = element;
    endIndex = correctIndex(endIndex + 1);
    m_length++;
}

template <typename T>
void CircularDynamicArray<T>::addFront(T element)
{
    if (m_length == 0)
    {
        addEnd(element);
        return;
    }

    if (m_length == m_capacity)
    {
        growArray();
    }

    frontIndex = correctIndex(frontIndex - 1);
    array[frontIndex] = element;
    m_length++;
}

template <typename T>
void CircularDynamicArray<T>::delEnd()
{
    if (m_length == 0)
    {
        cout << "Trying to delete element from an empty array! Aborting." << endl;
        return;
    }

    m_length--;
    endIndex = correctIndex(endIndex - 1);

    if (m_length < m_capacity * .25)
    {
        shrinkArray();
    }
}

template <typename T>
void CircularDynamicArray<T>::delFront()
{
    if (m_length == 0)
    {
        cout << "Trying to delete element from an empty array! Aborting." << endl;
        return;
    }

    m_length--;
    frontIndex = correctIndex(frontIndex + 1);

    if (m_length < m_capacity * .25)
    {
        shrinkArray();
    }
}

#pragma endregion AddDeleteElements

#pragma region PropertyGetters

template <typename T>
int CircularDynamicArray<T>::length() const
{
    return m_length;
}

template <typename T>
int CircularDynamicArray<T>::capacity() const
{
    return m_capacity;
}

#pragma endregion PropertyGetters

#pragma region Clear

template <typename T>
void CircularDynamicArray<T>::clear()
{
    delete[] array;
    array = new T[2];
    m_length = 2;
    m_capacity = 2;
    frontIndex = 0;
    endIndex = 2;
}

template <typename T>
void CircularDynamicArray<T>::clearCompletely(){
    delete[] array;
    array = new T[2];
    m_length = 0;
    m_capacity = 2;
    frontIndex = 0;
    endIndex = 0;
}

#pragma endregion Clear

#pragma region QuickSelect

template <typename T>
T CircularDynamicArray<T>::QuickSelect(int k)
{
    return qsRecursive(0, m_length - 1, k - 1);
}

template <typename T>
T CircularDynamicArray<T>::qsRecursive(int left, int right, int k)
{
    int pivot = partition(left, right);

    if (k < pivot)
    {
        return qsRecursive(left, pivot - 1, k);
    }
    else if (k > pivot)
    {
        return qsRecursive(pivot + 1, right, k);
    }
    else
    {
        return array[(pivot + frontIndex) % m_capacity];
    }
}

#pragma endregion QuickSelect

#pragma region WorstCaseSelect

template <typename T>
T CircularDynamicArray<T>::WCSelect(int k)
{
    //this is called expecting to return the kth smallest element in worst case O(n)

    //step 1: logically break up into groups of 5
    //step 2: sort the groups of 5 (relatively small number of comparisons. consider the middle element (in 0-4, its element 2))
    //step 3: find the median of the medians
    //step 4: partition on the median of medians
    //step 5: recurse on left or right of partitions

    return wcRecursive(0, m_length - 1, k - 1);
}

//specifically for the main array
template <typename T>
T CircularDynamicArray<T>::wcRecursive(int left, int right, int k)
{	
	//make sure that k is smaller than the number of elements in the array  
    if (k >= 0 && k <= right - left + 1)
    {
        int numElements = right - left + 1;

        int i;
        T *medians = new T[(numElements + 4) / 5];
        for (i = 0; i < numElements / 5; i++)
        { //there are i groups of 5
            mergeSort(i * 5, (i * 5) + 4);
            medians[i] = getElement((i * 5) + 2);
        }
        
        //todo: catch case where elements are not exactly n % 5 = 0.
        if(i * 5 < numElements){
            mergeSort(i * 5, (i * 5) + (numElements % 5) - 1);
            medians[i] = getElement((i * 5) + ((numElements % 5) / 2));
            i++;
        }

        //get the median of medians
        mergeSort(medians, 0, i - 1);

        T medianOfMedians;
        if(i == 1){
            medianOfMedians = medians[i - 1];
        }else{
            medianOfMedians = medians[i / 2];
        }
        
        int positionOfMOM = partition(left, right, medianOfMedians);

        if (k < positionOfMOM)
        {
            return wcRecursive(left, positionOfMOM - 1, k);
        }
        else if (k > positionOfMOM)
        {
            return wcRecursive(positionOfMOM + 1, right, k);
        }
        else
        {
            return getElement(positionOfMOM);
        }
    }

    if(left == right){
        return getElement(left);
	}
	else if (k > left - right + 1) {
		return getElement(k);
	}

	return -1;
}

#pragma endregion WorstCaseSelect

#pragma region Partition_and_Swap_Functions

//partitions the array around the right most index (which could be anywhere, since the array is circular)
template <typename T>
int CircularDynamicArray<T>::partition(int left, int right)
{
    T pivotElement = getElement(right);
    int partitionIndex = left;
    for (int i = left; i < right; i++)
    {
        if (getElement(i) < pivotElement)
        {
            swap(getElement(i), getElement(partitionIndex));
            partitionIndex++;
        }
    }
    swap(getElement(partitionIndex), getElement(right));
    return partitionIndex;
}

template <typename T>
int CircularDynamicArray<T>::partition(int left, int right, T partitionElement)
{
    int partitionIndex;
    for (partitionIndex = left; partitionIndex < right; partitionIndex++)
    {
        if (getElement(partitionIndex) == partitionElement)
        {
            break;
        }
    }
    swap(getElement(partitionIndex), getElement(right));

    partitionIndex = left;
    for (int i = left; i < right; i++)
    {
        if (getElement(i) < partitionElement)
        {
            swap(getElement(i), getElement(partitionIndex));
            partitionIndex++;
        }
    }
    swap(getElement(partitionIndex), getElement(right));
    return partitionIndex;
}

template <typename T>
void CircularDynamicArray<T>::swap(T &a, T &b)
{
    T temp = a;
    a = b;
    b = temp;
}

#pragma endregion Partition_and_Swap_Functions

#pragma region StableSort

template <typename T>
void CircularDynamicArray<T>::stableSort()
{
    //the stable sort is going to be merge sort
    mergeSort(0, m_length - 1);
}

//splits the array in half over and over until it gets to two elements, sorts those two elements, and then merges them together.
template <typename T>
void CircularDynamicArray<T>::mergeSort(int left, int right)
{
    if (left < right)
    {
        //get the middle index
        int middle = (left + right) / 2;

        //sort the left and right halves
        mergeSort(left, middle);
        mergeSort(middle + 1, right);
        //merge the halves together
        merge(left, middle, right);
    }
}

//merges elements at the indices given. does not adjust the circular array, so it's almost in place
template <typename T>
void CircularDynamicArray<T>::merge(int left, int middle, int right)
{
    //create temp arrays for each half
    T *tempArray = new T[right - left + 1];
    int i = left, j = middle + 1, k = 0;

    while (i <= middle && j <= right)
    {
        if (getElement(i) <= getElement(j))
        {
            tempArray[k] = getElement(i);
            k++;
            i++;
        }
        else
        {
            tempArray[k] = getElement(j);
            k++;
            j++;
        }
    }

    //any elements that weren't compared are appended in, as they are in order already or will be compared later (left over elements)
    while (i <= middle)
    {
        tempArray[k] = getElement(i);
        k++;
        i++;
    }
    while (j <= right)
    {
        tempArray[k] = getElement(j);
        k++;
        j++;
    }

    //take everything in the temp array and put it where it goes.
    for (i = left; i <= right; i++)
    {
        getElement(i) = tempArray[i - left];
    }
}

template <typename T>
void CircularDynamicArray<T>::mergeSort(T* array, int left, int right)
{
    if (left < right)
    {
        //get the middle index
        int middle = (left + right) / 2;

        //sort the left and right halves
        mergeSort(array, left, middle);
        mergeSort(array, middle + 1, right);
        //merge the halves together
        merge(array, left, middle, right);
    }
}

//merges elements at the indices given. does not adjust the circular array, so it's almost in place
template <typename T>
void CircularDynamicArray<T>::merge(T* array, int left, int middle, int right)
{
    //create temp arrays for each half
    T *tempArray = new T[right - left + 1];
    int i = left, j = middle + 1, k = 0;

    while (i <= middle && j <= right)
    {
        if (array[i] <= array[j])
        {
            tempArray[k] = array[i];
            k++;
            i++;
        }
        else
        {
            tempArray[k] = array[j];
            k++;
            j++;
        }
    }

    //any elements that weren't compared are appended in, as they are in order already or will be compared later (left over elements)
    while (i <= middle)
    {
        tempArray[k] = array[i];
        k++;
        i++;
    }
    while (j <= right)
    {
        tempArray[k] = array[j];
        k++;
        j++;
    }

    //take everything in the temp array and put it where it goes.
    for (i = left; i <= right; i++)
    {
        array[i] = tempArray[i - left];
    }
}

#pragma endregion StableSort

#pragma region RadixSort

//sorts the values in the array using radix sort on the low order i bits of the elm type
template <typename T>
void CircularDynamicArray<T>::radixSort(int i)
{
    for(int b = 0; b < i; b++){ //b is the current bit being sorted.
        bitCountingSort(b);
    }

    //TODO: Test negative numbers? 
}

//counting sort (stable) for radix sort, which takes in the argument b for which bit of the values of the array is being sorted on. 
template <typename T>
void CircularDynamicArray<T>::bitCountingSort(int b){
    T* finalArray = new T[m_length];
    int count[2] = {0};

    //get the counts of the 0s and 1s of the unsigned int cast 
    for(int i = 0; i < m_length; i++){
        unsigned int castedElement = (unsigned int)getElement(i);
        unsigned int bitValue = (castedElement >> b) & 1; //shifts the unsigned int to the right b times, and bitwise ands it with 1
        //cout << "current bit value of the " << b << "th bit for element " << getElement(i) << " is " << bitValue << endl;
        count[bitValue]++;
    }

    //adjust the count array to be cumulative
    count[1] += count[0];

    for(int i = m_length - 1; i >= 0; i--){
        unsigned int castedElement = (unsigned int)getElement(i);
        unsigned int bitValue = (castedElement >> b) & 1;
        int locationOfElement = count[bitValue] - 1;
        finalArray[locationOfElement] = getElement(i);
        count[bitValue]--;
    }

    for(int i = 0; i < m_length; i++){
        getElement(i) = finalArray[i];
    }

	delete[] finalArray;
}

#pragma endregion RadixSort

#pragma region SearchAlgos

template <typename T>
int CircularDynamicArray<T>::linearSearch(T key)
{
    int index;
    for(index = 0; index < m_length; index++){
        if(getElement(index) == key){
            return index;
        }
    }
    return -1;
}

//assumes the array is sorted.
template <typename T>
int CircularDynamicArray<T>::binSearch(T key)
{
    return binSearchRecursive(0, m_length - 1, key);
}

template <typename T>
int CircularDynamicArray<T>::binSearchRecursive(int left, int right, T key){
    if(right >= left){
        int middle = (left + right) / 2;
        if(getElement(middle) == key){
            return middle;
        }else if(getElement(middle) > key){
            return binSearchRecursive(left, middle - 1, key);
        }else{
            return binSearchRecursive(middle + 1, right, key);
        }
    }
    return -1;
}

#pragma endregion SearchAlgos

#pragma region Print

template <typename T>
void CircularDynamicArray<T>::print()
{
    cout << "size is : " << m_length << endl
         << "capacity is : " << m_capacity << endl
         << "front index is: " << frontIndex << endl
         << "end index is: " << endIndex << endl;
    for (int i = 0; i < m_length; i++)
        cout << array[(i + frontIndex) % m_capacity] << " ";
    cout << endl
         << endl;
}

#pragma endregion Print