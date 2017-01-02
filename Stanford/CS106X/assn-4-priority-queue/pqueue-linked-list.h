/*
 * File: pqueue-linked-list.h
 * ---------------------
 * This interface defines the LinkedListPQueue class, which implements a
 * linked list based absraction of a priority queue.
 */


#ifndef _linked_list_pqueue_
#define _linked_list_pqueue_

#include "pqueue.h"
#include <string>


/*
 * Class: LinkedListPQueue
 * ---------------------
 * This class models a priority queue for strings.
 */

class LinkedListPQueue : public PQueue {
public:

/*
 * Constructor: LinedListPQueue()
 * usage: LinkedListPQueue llpqueue;
 * --------------------------------
 * Initializes a new empty queue.
 */
    LinkedListPQueue();

/*
 * Destructor: ~VectorPQueue()
 * usage: (usually implicit)
 * --------------------------------
 * Frees any heap storage associated with this character stack.
 */
	~LinkedListPQueue();
	
/*
 * Method: merge
 * usage: LinkedListPQueue *LinkedListPQueue(*one, *two);
 * ----------------------------------------------
 * Merges two LinkedListPQueues into a single LinkedListPQueue.
 */
	static LinkedListPQueue *merge(LinkedListPQueue *one, LinkedListPQueue *two);
	
/*
 * Method: enqueue
 * usage: llpqueue.enqueue(elem);
 * ----------------------------------
 * enqueues string elem to the queue.
 */
	void enqueue(const std::string& elem);

/*
 * Method: extractMin
 * usage: llpqueue.extractMin();
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
 * This version of priority queue uses a doubly linked list to sort the elements
 * of the queue as they are added to it.  extractMin and peek can then simply
 * return the head node.
 */
private:
/* Private Structures */

    struct LinkedNode {
        std::string data;
        LinkedNode *next;
        LinkedNode *prev;

        LinkedNode() {
            data = "";
            next = NULL;
            prev = NULL;
        }
        LinkedNode(std::string d, LinkedNode* n, LinkedNode* p){
            data = d;
            next = n;
            prev = p;
        }
    };


    LinkedNode * head;
};

#endif
