#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <iostream>
#include "Node.h"

// Creates a Linked list made up of a series of nodes where the last node points to NULL
struct LinkedList
{
private:
	Node* head;
public:
	//create and destory
	LinkedList();
	~LinkedList();
	LinkedList(int* arr, int length);

	// modifies the list to have somethign added at the start, end or at a selected index
	void addFront(int newItem);
	void addEnd(int newItem);
	void addAtPosition(int position, int newItem);

	// finds the index of said item in the list
	int search(int item);

	// modifies the list to remove something at the start, end or at a selected index
	void deleteFront();
	void deleteEnd();
	void deletePosition(int position);
	// gets the item at a selected index like arr[i] for index i
	int getItem(int position);
	// prints all the items in the list to console
	void printItems();

};
#endif