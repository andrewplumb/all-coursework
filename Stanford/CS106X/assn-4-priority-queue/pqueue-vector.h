/*
 * File: pqueue-vector.h
 * ---------------------
 * This interface defines the VectorPQueue class, which implements a vector based
 * absraction of a priority queue.
 */

#ifndef _vector_pqueue_
#define _vector_pqueue_

#include "pqueue.h"
#include <string>
#include "vector.h"

/*
 * Class: VectorPQueue
 * ---------------------
 * This class models a priority queue for strings.
 */

class VectorPQueue : public PQueue {
public:

/*
 * Constructor: VectorPQueue()
 * usage: VectorPQueue vectorpqueue;
 * --------------------------------
 * Initializes a new empty queue that can contain strings.
 */
	VectorPQueue();

/*
 * Destructor: ~VectorPQueue()
 * usage: (usually implicit)
 * --------------------------------
 * Frees any heap storage associated with this character stack.
 */
	~VectorPQueue();

/*
 * Method: merge
 * usage: VectorPQueue *VectorPQueue(*one, *two);
 * ----------------------------------------------
 * Merges two VectorPQueues into a single VectorPQueue.
 */
	static VectorPQueue *merge(VectorPQueue *one, VectorPQueue *two);

/*
 * Method: enqueue
 * usage: vectorpqueue.enqueue(elem);
 * ----------------------------------
 * enqueues string elem to the queue by adding it on to the end of the vector.
 */
	void enqueue(const std::string& elem);

/*
 * Method: extractMin
 * usage: vectorpqueue.extractMin();
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


/* Private Section */

/*
 * Implementation Notes
 * --------------------
 * This version of priority queue uses a Vector<string> as its underlying data
 * structure.  Strings are stored unsorted, with the newly added strings going
 * on the end of the vector. The peek and extractMin methods search through the
 * vector to find the highest priority string.
 */
private:
    /* Instance variables */
    Vector<string> elements; /* Vector to hold the priority queue elements. */

};

#endif
