/*
 * File: pqueue-heap.h
 * ---------------------
 * This interface defines the HeapPQueue class, which implements a
 * linked list based absraction of a priority queue.
 */

#ifndef _binary_heap_pqueue_
#define _binary_heap_pqueue_

#include "pqueue.h"
#include <string>

/*
 * Class: HeapPQueue
 * ---------------------
 * This class models a priority queue for strings.
 */


class HeapPQueue : public PQueue {
public:

/*
 * Constructor: HeapPQueue()
 * usage: HeapPQueue hpqueue;
 * --------------------------------
 * Initializes a new empty queue.
 */
	HeapPQueue();

/*
 * Destructor: ~VectorPQueue()
 * usage: (usually implicit)
 * --------------------------------
 * Frees any heap storage associated with this character stack.
 */
	~HeapPQueue();
	
/*
 * Method: merge
 * usage: HeapPQueue *HeapPQueue(*one, *two);
 * ----------------------------------------------
 * Merges two HeapPQueues into a single HeapPQueue.
 */
	static HeapPQueue *merge(HeapPQueue *one, HeapPQueue *two);
	
/*
 * Method: enqueue
 * usage: hpqueue.enqueue(elem);
 * ----------------------------------
 * enqueues string elem to the queue.
 */
	void enqueue(const std::string& elem);

/*
 * Method: extractMin
 * usage: hpqueue.extractMin();
 * ---------------------------------
 * returns the highest priority string in the queue, and removes it. Raises an
 * error if called on an empty queue.
 */
    std::string extractMin();

/*
 * Method: peek
 * usage: vectorpqueue.peek();
 * ---------------------------------
 * returns the highest priority string in the queue without removing it. Raises an
 * error if called on an empty queue.
 */
    std::string peek() const;

/*
 * Copy constructor: HeapPQueue
 * usage: (usually implicit);
 * --------------------------------
 * Initializes the current object to a deep copy of the specified source.
 */
    HeapPQueue(const HeapPQueue & elements);

/*
 * Operator: =
 * usage: nhpqueue = hpqueue;
 * --------------------------
 * Assigns hpqueue to nhpqueue so the two are independent copies.
 */
    HeapPQueue & operator=(const HeapPQueue & elements);
    
private:
    /* Private constants */
    static const int INITIAL_CAPACITY = 10;

    /* Instance variables */
    std::string *array; /* dynamic array of strings */
    int capacity;       /* allocated size of that array*/
                        /* currernt count is tracked in logSize */
    /* Private function prototypes */
    void expandCapacity();
    void deepCopy(const HeapPQueue & elements);
};

#endif
