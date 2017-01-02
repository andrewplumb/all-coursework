/*
 * File: pqueue-heap.cpp
 * -----------------------
 * This file implements the HeapPQueue Class using a min heap represented by an array.
 * Parent elements are always higher or equal priority to their children.
 * Elements are added to the end of the array, and then are bubbled-up until they reach their
 * appropriate position.  The root of the binary heap will always be the highest priority element.
 */

#include "pqueue-heap.h"
#include <string>
#include "error.h"
using namespace std;

HeapPQueue::HeapPQueue() {
    capacity = INITIAL_CAPACITY;
    array = new string[capacity];
    logSize = 0;
}
HeapPQueue::~HeapPQueue() {
    delete[] array;
}

/*
 * The peak method returns the root element of the binary heap (0th element of the
 * array) without altering the heap.
 */
string HeapPQueue::peek() const {
    if (isEmpty()) error("peek: Attempting to peek at an empty queue.");
    return array[0];
}

/*
 * The extractMin method returns the root element of the binary heap and moves the last
 * element in the array to the root position.  The new root element then trickles down
 * through the heap by swaping it with the higher priority of it's two children if one or more
 * children are higher priority than it.  This is repeatd until the element is in the proper
 * position.
 */
string HeapPQueue::extractMin() {
    if (isEmpty()) error("extract: Attempting to extract from an empty queue.");
    string min = array[0];
    logSize--;
    array[0] = array[logSize]; /* moves the last element to the root position */
    int i = 0;
    while (i*2+1 < logSize){ /* checks if the element has children */
        if (i*2+2 >= logSize){ /* enter this loop only if the right child is outside
                                  the array */
            if (array[i] > array[i*2+1]){ /* if the parent is lower priority than the */
                swap(array[i], array[i*2+1]); /* left child, swap and keep it active */
                i = i*2+1;
            }else break;
        }else if (array[i] > array[i*2+1] || array[i] > array[i*2+2]){
            if (array[i*2+1] < array[i*2+2]){ /* if the parent is lower priority than either */
                swap(array[i], array[i*2+1]); /* child, switch with the higher priority child*/
                i = i*2+1;
            }else {
                swap(array[i], array[i*2+2]);
                i = i*2+2;
            }
        }else break; /* break if the parent is higher priority than both children */
    }
    return min;
}

/*
 * the enqueue method enqueues a new element into the min heap. The element is placed as the
 * last element in the array, and then is bubbled up as necessary to be less than or equal
 * priority as its parent, and greater than or equal priority to its children.
 */
void HeapPQueue::enqueue(const string& elem) {
    if (logSize == capacity) expandCapacity();
    array[logSize++] = elem;
    int i = logSize-1;
    while (array[i] < array[(i-1)/2]){
        swap(array[i], array[(i-1)/2]);
        i = (i-1)/2;
    }
}
/*
 * the merge method combines two min heaps into a new min heap that contains all the elements
 * of the two input heaps.  The elements of each heap are concatenated together.  Starting with
 * the last element, each element is heapified, as in the extractMin method.
 */
HeapPQueue *HeapPQueue::merge(HeapPQueue *one, HeapPQueue *two) {
    HeapPQueue *merged = new HeapPQueue();
    merged->array = new string[one->logSize+two->logSize];
    for (int i = 0; i < one->logSize; i++){
        merged->array[i] = one->array[i];
    }
    for (int j = 0; j < two->logSize; j++){
        merged->array[one->logSize+j] = two->array[j];
    }
    merged->logSize = one->logSize + two->logSize;
    merged->capacity = one->capacity + two->capacity;
    for (int j = merged->logSize-1; j >= 0 ; j--){
        int i = j;
        while (i*2+1 < merged->logSize){
            if (i*2+2 >= merged->logSize){
                if (merged->array[i] > merged->array[i*2+1]){
                    swap(merged->array[i], merged->array[i*2+1]);
                    i = i*2+1;
                }else break;
            }else if (merged->array[i] > merged->array[i*2+1] || merged->array[i] > merged->array[i*2+2]){
                if (merged->array[i*2+1] < merged->array[i*2+2]){
                    swap(merged->array[i], merged->array[i*2+1]);
                    i = i*2+1;
                }else {
                    swap(merged->array[i], merged->array[i*2+2]);
                    i = i*2+2;
                }
            }else break;
        }
    }
    return merged;
}


/*
 * The copy constructor and assignment operator make it possible to pass a HeapPQueue by
 * value or assign one HeapPQueue to another.  The work is done by the deepCopy method.
 */
HeapPQueue::HeapPQueue(const HeapPQueue & elements) {
    deepCopy(elements);
}

HeapPQueue & HeapPQueue::operator=(const HeapPQueue & elements) {
    if (this != &elements){
        delete[] array;
        deepCopy(elements);
    }
    return *this;
}

/*
 * The deepCopy method copys all data from the elements parameter into
 * the current object.  All dynamic memory is reallocated to ensure both
 * the current object and source object are independent.
 */
void HeapPQueue::deepCopy(const HeapPQueue &elements){
    array = new string[elements.logSize];
    for (int i = 0; i < elements.logSize; i++){
        array[i] = elements.array[i];
    }
    logSize = elements.logSize;
    capacity = elements.capacity;
}

/*
 * The expandCapacity method doubles the capacaty of the elemnts array whenever it runs out of space.
 * It does so by copying the pointer to the old array, allocating a new array with twice the capacity,
 * copying the characters from the old array into the new one, and freeing the memory from the old
 * array.
 */
void HeapPQueue::expandCapacity(){
    string *oldArray = array;
    capacity *= 2;
    array = new string[capacity];
    for (int i = 0; i < logSize; i++){
        array[i] = oldArray[i];
    }
    delete[] oldArray;
}
