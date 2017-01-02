/*
 * File: pqueue-linked-list.cpp
 * -----------------------
 * This file implements the LinkedListPQueue class using a linked list as the base data
 * structure.  Elements are added to the linked list in the location that the prev element is
 * equal to or higher priorety, and the next element is equal to or lower priority.  The head
 * element is then always the highest priority.
 */

#include "pqueue-linked-list.h"
#include "error.h"
using namespace std;

LinkedListPQueue::LinkedListPQueue() {
    head = NULL;
}
LinkedListPQueue::~LinkedListPQueue() {}

/*
 * The peek method returns an error if the queue is empty.  Otherwise, it returns
 * the data value of the head element.
 */
string LinkedListPQueue::peek() const {
    if (isEmpty()) error("peek: Attempting to peek at an empty queue.");
    string min = head->data;
    return min;
}


/*
 * The extractMin method returns the data value of the head element, and also
 * removes that element from the linked list.
 */
string LinkedListPQueue::extractMin() {
    if (isEmpty()) error("extract: Attempting to extract from an empty queue.");
    string min = head->data; /* the highest priority string is always the head */
    LinkedNode * current = head;
    head = head->next; /* point the head to the next element in the list */
    logSize--;
    delete current; /* delete the hold head */
    return min;
}

/* The enqueue method adds a new element to the linked list in a location that maintains the order
 * of highest to lowest priority.
 */
void LinkedListPQueue::enqueue(const string& elem) {
    if (head == NULL){ /* if the linked list is empty, the head then points to the new element */
        head = new LinkedNode(elem, NULL, NULL);
        logSize++;
        return;
    }
    if (elem <= head->data){ /* if the new element is smaller than or equal to the smallest in the
                                linked list, it becomes the new head and points to the old head */
        head = new LinkedNode(elem, head, head->prev);
        head->next->prev = head;
        logSize++;
        return;
    }
    if (head->next == NULL){ /* if the head element is the only element, and the new element has
                                lower priority than the head, put it after the head*/
        head->next = new LinkedNode(elem, NULL, head);
        logSize++;
        return;
    }
    LinkedNode * current = head->next;
    while (true){ /* this moves through the linked list until the correct spot for the new
                     element is found. */
        if (elem < current->data){
            current->prev = new LinkedNode(elem, current, current->prev);
            current->prev->prev->next = current->prev;
            logSize++;
            return;
        }
        if (current->next == NULL){ /* If the new element has lower priority than all
                                       the other elements, it is added to the end.*/
            current->next = new LinkedNode(elem, NULL, current);
            logSize++;
            return;
        }
        current = current->next;
    }
}

/* The merge method combines two sorted linked lists into a single sorted linked list.
 * The head elements of each linked list are compared, and the higher priority head is
 * enqueued into the new linked list.  Because the elements are enqueued in order from
 * highest to lowest priority, the enqueue method puts each added element on the end of
 * the new linked list.
 */
LinkedListPQueue *LinkedListPQueue::merge(LinkedListPQueue *one, LinkedListPQueue *two) {
    LinkedListPQueue *merged = new LinkedListPQueue();
    while (!one->isEmpty() && !two->isEmpty()){
        if (one->peek() <= two->peek()){
            merged->enqueue(one->extractMin());
        }else merged->enqueue(two->extractMin());
    }
    while (!one->isEmpty()) merged->enqueue(one->extractMin()); /* when the first linked list runs out of */
    while (!two->isEmpty()) merged->enqueue(two->extractMin()); /* elements, the rest of the elements from the
                                                                   other linked list are added to the new queue*/
    return merged;
}
