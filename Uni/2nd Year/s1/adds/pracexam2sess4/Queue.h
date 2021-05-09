

#ifndef QUEUE_H
#define QUEUE_H

#define CAP 8

class Queue{
 public:
  // constructor
  Queue();

  // operators

  void add(int val); // adds val to the  queue
  int remove(); // removes an item from the queue

 private:
  int content[CAP]; // contents of the queue
  int start; // head of queue
  int end; // end of queue (next spare spot - if any)
  int count; // number of items in the queue
};

#endif //QUEUE_H
