/******************************************************************************
 *
 * Name:Bilal Mustafa	
 *
 * Date:5/7/2017	
 *
 * Assignment: Project 5	
 *
 * Purpose:Compression and Decompression via Huffman Encoding - Good Grades	
 *
 *****************************************************************************/
#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <iostream>

using namespace std;
typedef char byte;

class PriorityQueue{
public:
  //typedef HTreePtr ItemTy;

class HTree{
 public:
   
   // Purpose: Constructor
   //
   // Pre: None.
   // Post: A HTree object is initialized.
   HTree(const size_t key, const byte value, HTree* left = NULL,  
         HTree* right = NULL);

   ~HTree();
  
   size_t Key;
   HTree* Right;
   byte Value;
   HTree* Left;
};

 class HTreePtr{
 public:
   
   // Purpose: Constructor
   //
   // Pre: None.
   // Post: An Object contraining a HTree Pointer is initialized.
   HTreePtr(HTree* ptr = NULL);

   //the operator < is defined for the HTreePtr object
   bool operator<(const HTreePtr& HtP1) const;

   //the operator = is defined for the HtreePtr object
   void operator =(const HTreePtr& HtP1);

   HTree* Pointer;
 };
 
 typedef HTreePtr ItemTy;
 
 // Purpose: Constructor
 //
 // Pre: None
 // Post: An empty priority queue of given capacity is created
 PriorityQueue(size_t capacity);
 
 // Purpose: Destructor
 //
 // Pre: None.
 // Post: All memory associated with the queue has been freed.
 ~PriorityQueue();
 
 // Purpose: Returns the number of items in the queue
 //
 // Pre: None.
 // Post: The number of items in the queue is returned.
 size_t length() const;
 
 // Purpose: Adds an item to the end of the queue
 //
 // Pre: The queue is not full.
 // Post: The item is inserted at the end of the queue.
 void enqueue(const ItemTy& item);
 
 // Purpose: Removes an item from the front of the queue
 //
 // Pre: The queue is not empty.
 // Post: The front item in the queue is removed.
 void dequeue();
 
 // Purpose: Inspect the front item in the queue.
 //
 // Pre: The queue is not empty.
 // Post: The item at the front of the queue is returned.
 ItemTy front() const;
 
 // Purpose: Determines if the queue is full
 //
 // Pre: None.
 // Post: If the queue contains the maximum capacity of items true is returned.
 // Otherwise, false.
 bool full() const;
 
 // Purpose: Determines if the queue is empty
 //
 // Pre: None.
 // Post: If the queue contains items, true is returned. Otherwise, false.
 bool empty() const;

 private:
 size_t Capacity;
 size_t Size;
 ItemTy* PQueue;
};


#endif
