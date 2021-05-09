#include "Node.h"

#ifndef LINKEDLIST
#define LINKEDLIST

class LinkedList {
public:
	LinkedList();
	LinkedList(int*, int);
    void swap(Node * nodeBefore1, Node * nodeBefore2); //The swap function: Assumes that if one of the nodes that are going to be swapped is the first node of the list, then the corresponding nodeBefore is a nullptr.
    void printItems();
    void bubbleSort();
private:
	Node * head;
};

#endif //LINKEDLIST
