#include "LinkedList.h"
#include <iostream>
#include <limits>
LinkedList::LinkedList(){
	// points to NULL
	head = NULL;
}

LinkedList::LinkedList(int* arr, int length){
	// creates a node in memory
	Node* newNode = new Node();
	// "grounds" the list at a starting point and makes that arr[0]
	head = new Node();
	head->setData(arr[0]);
	head->setNext(newNode);

	// loops through the array, creating a new node after setting the value of the previous one
	// so the setNext function has somethign to point to
	Node* temp = head;
	newNode = new Node();
	for(int i =1; i < length; i++){
		temp = temp->getNext();
		temp->setNext(newNode);
		temp->setData(arr[i]);
		newNode = new Node();
	}
	// sets the last setNext to NULL, ending the list
	temp->setNext(NULL);
}

void LinkedList::addFront(int newItem){
	// Creating new node
	Node* newNode = new Node();
	newNode->setData(newItem);
	newNode->setNext(head);
	head = newNode;
}

void LinkedList::addEnd(int newItem){
	// Creating new node
	Node* newNode = new Node();
	newNode->setData(newItem);
	newNode->setNext(NULL);
	// swapping head pointer to point to this node if head == null
	Node* temp = head;
	if(head == NULL){
		head = newNode;
	} else {
		//finding last value in the list
		while(temp->getNext() != NULL){
			temp = temp->getNext();
		}
		temp->setNext(newNode);
	}
}

void LinkedList::addAtPosition(int position, int newItem){
	// Creating new node
	Node* newNode = new Node();
	newNode->setData(newItem);
	

	// Finding where to put it
	Node* temp = head;
	int i =1; // Because position starts at one (and not zero)
	// Puts at start if position is less than one
	if(position <= 1){
		newNode->setNext(head);
		head=newNode;
		return;
	}
	while(i != position-1){
		i++;
		// Checks if index is inside the list. Adds to end if not.
		if(temp->getNext() == NULL){
			newNode->setNext(NULL);
			temp->setNext(newNode);
			return;
		}
		temp = temp->getNext();
	}
	// Merges values into list
	newNode->setNext(temp->getNext());
	temp->setNext(newNode);
}

int LinkedList::search(int item){
	Node* temp = head;
	int soln = 1;
	while(temp->getData() != item){
		// If out of range
		if(temp->getNext() == NULL){
			std::cout << 0 << " ";
			return 0;
		}
		// iterates through the loop
		soln++;
		temp = temp->getNext();
	}
	//couts the result and returns it
	std::cout << soln << " ";
	return soln;
}

void LinkedList::deleteFront(){
	if(head != NULL){
		head = head->getNext();
	}
}

void LinkedList::deleteEnd(){
	// loops through to the end of the list and deletes the last node as well as pointing the new last node to NULL
	Node* temp = head;
	// 2 base cases because were pointing to a pointer to a pointer
	if (head == NULL){
	} else if (head->getNext() == NULL){
		head =  NULL;
	} else {
		// finds the second to last node, destroys the last node and sets the new last node to point to NULL
		while(temp != NULL){
			if((temp->getNext())->getNext() == NULL){
				temp->setNext(NULL);
			}
			temp = temp->getNext();
		}
	}
}

void LinkedList::deletePosition(int position){
	Node* rmNode = head;
	int index = 1;
	
	if(position == 1){
		// Checks base case input = 1
		rmNode = head->getNext();
		delete head;
		std::cout << "outside range";
		head = rmNode;
		return;
	}

	while(index != position-1){
		// checks for bad input
		if(rmNode->getNext() == NULL){
			std::cout << "outside range";
			return;
		}
		rmNode = rmNode->getNext();
		index++;
	}
	// modifies list
	Node* temp = rmNode->getNext();
	rmNode->setNext(temp->getNext());
	delete temp;
}

int LinkedList::getItem(int position){
	Node* temp = head;
	int index = 1;
	//loops through list until it finds what its looking for then grabs the data out of the Node
	while(index != position){
		index++;
		// Breaks loop if the position is out of range
		if(temp->getNext() == NULL){
			// only God knows why
			std::cout << std::numeric_limits<int>::max() << " ";
			return std::numeric_limits<int>::max();
		}
		temp = temp->getNext();
	}
	std::cout << temp->getData() << " ";
	return temp->getData();
}

void LinkedList::printItems(){
	// loops through the list then couts the data
	Node* temp = head;
	while(temp != NULL){
		int printer = temp->getData();
		std::cout << printer << " ";
		temp = temp->getNext();
	}
	std::cout << std::endl;
}
LinkedList::~LinkedList(){
	// loops through each node and deletes them from memory
	while(head!=NULL){
		Node* temp = head;
		head = head->getNext();
		delete temp;
	}
	delete head;
}