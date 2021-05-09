#ifndef NODE_H
#define NODE_H
#include <iostream>

// Creates a segment of a list pointing to the next part of the list and storing an integer value
struct Node
{
private:
	Node* next;
	int data;
public:
	//constructor
	Node();
	//getters
	int getData();
	Node* getNext();
	//setters
	void setData(int d);
	void setNext(Node *n);
};
#endif