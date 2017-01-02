/*
 * File: pqueue-vector.cpp
 * -----------------------
 * This file implements the VectorPQueue Class using a Vector<string> as the
 * underlying representation.  Elements are stored in the vector unsorted, and the
 * vector is searched through to find the highest priority element when needed.
 */

#include "pqueue-vector.h"
#include "error.h"
using namespace std;

VectorPQueue::VectorPQueue() {}

VectorPQueue::~VectorPQueue() {}

/*
 * The peek method returns an error if the queue is empty.  Otherwise, it searches
 * the Vector for the highest priority string and returns it.
 */
string VectorPQueue::peek() const {
    if (isEmpty()) error("peek: Attempting to peek at an empty queue.");
    string min = elements[0];
    for (int i = 1; i < elements.size(); i++){
        if (elements[i] < min) min = elements[i];
    }
    return min;
}

/*
 * The extractMin method finds the highest priority string in the same manner as
 * peek, returns it, and also removes it from the Vector.
 */
string VectorPQueue::extractMin() {
    if (isEmpty()) error("extract: Attempting to extract from an empty queue.");
    string min = elements[0];
    int minLocation = 0; // to keep track of the location of the highest priority string
    for (int i = 1; i < elements.size(); i++){
        if (elements[i] < min){
            min = elements[i];
            minLocation = i;
        }
    }
    elements.remove(minLocation);
    logSize--;
    return min;
}

void VectorPQueue::enqueue(const string& elem) {
    elements.add(elem);
    logSize++;
}

/*
 * the merge method merges two priority queues together.  Because they are not sorted,
 * the new queue is made from the logical concactenation of the elements of the two
 * input queues.
 */
VectorPQueue *VectorPQueue::merge(VectorPQueue *one, VectorPQueue *two) {
    VectorPQueue *merged = new VectorPQueue();
    merged->elements = one->elements + two->elements;
    merged->logSize = one->logSize + two->logSize;
    return merged;
}
