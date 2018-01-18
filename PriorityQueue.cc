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
#include <iostream>
#include "PriorityQueue.h"

using namespace std;
typedef PriorityQueue::HTreePtr ItemTy;
typedef PriorityQueue::HTree HTree;
typedef char byte;


PriorityQueue::HTree::HTree(const size_t key, const byte value,            
                                  HTree* left,  HTree* right) {

  //Initialize HTree 
  Key = key;
  Value = value;
  Left = left;
  Right = right;
}

 PriorityQueue::HTree::~HTree() {

   //free memory allocated to HTree
   delete Left;
   delete Right;
 }

PriorityQueue::HTreePtr::HTreePtr(HTree* ptr) {

  //Initialize HTreePtr
  Pointer = ptr;
}

bool PriorityQueue::HTreePtr:: operator < (const HTreePtr& HtP1) const {

  //If the key of the HTree that the left HTreePtr points to is less than
  //that of the right, return true, else return false
  if(HtP1.Pointer != NULL and Pointer != NULL) {
  if(Pointer -> Key < HtP1.Pointer -> Key)
    return true;
  else
    return false;
  }
  else {
    return false;
  }
}

void PriorityQueue::HTreePtr:: operator =(const HTreePtr& HtP1) {

  //Have the pointer of the left HtreePtr point to that which the right
  //points to
  Pointer = HtP1.Pointer;
}

size_t parentTree(size_t index) {

  //Return the index of the parent node
  if(index % 2 != 0)
    return index / 2;
  else
    return (index - 1) / 2;
}

size_t leftChild(size_t index) {

  //Return the index of the left child node
  return (index * 2) + 1;
}

size_t rightChild(size_t index) {

  //Return the index of the right child node
  return (index * 2) + 2;
}

PriorityQueue::PriorityQueue(size_t capacity) {
  
  //Initialize a PriorityQueue of size capacity
  PQueue = new ItemTy[capacity];
  Size = 0;
  Capacity = capacity;
}
  
PriorityQueue::~PriorityQueue() {
  
  //Free up all memory allocated to PriorityQueue
  delete[] PQueue;
}

size_t PriorityQueue::length() const {
  
  //Return the number of objects in PriorityQueue
  return Size;
}

void PriorityQueue::enqueue(const ItemTy& item) {

  //if PriorityQueue is not full 
  if(Size != Capacity) {

    //Place item at the bottom of the heap
    PQueue[Size] = item;
    size_t workingSize = Size;
    size_t laggingSize = Size;
    ItemTy temporaryItemTy = NULL;
    bool trigger = true;
    
    //keep looking at the parent node in the tree
    //and compare their values
    if(Size != 0) {
      while(trigger and item < PQueue[parentTree(workingSize)] 
            and workingSize > 0) {
        workingSize = parentTree(workingSize);
        
        
        //if value is less than that of parent node
        //have them swap places
        temporaryItemTy = PQueue[workingSize];
        PQueue[workingSize] = item;
        PQueue[laggingSize] = temporaryItemTy;
        laggingSize = parentTree(laggingSize);
        if(workingSize == 0)
          trigger = false;
      }
    }
    
    //increment the size of the PriorityQueue
    Size++;
  }
}

void PriorityQueue::dequeue() {
  
  //If PriorityQueue is not empty
  if(Size!=0) {
    
    //Take out the item at the top of the heap and replace it
    //with the very last item
    PQueue[0] = PQueue[Size - 1];
    PQueue[Size - 1] = NULL;
    Size--;
    size_t index = 0;
    ItemTy temporaryItemTy = NULL;
    bool Trigger = true;
    
    //as long as the item (now at the top) has a value larger than 
    //that of its children, have it swap places with its smaller child
    while(Trigger and (PQueue[leftChild(index)] < PQueue[index]
                       or PQueue[rightChild(index)] < PQueue[index])) {
      if(Size - 1 < leftChild(index))
        Trigger = false;
      if(Trigger) {
        if(rightChild(index) <= Size - 1) {
          //if the right child is smaller, have it swap places with it
          if(PQueue[rightChild(index)] < PQueue[leftChild(index)]) {
            temporaryItemTy = PQueue[rightChild(index)];
            PQueue[rightChild(index)] = PQueue[index];
            PQueue[index] = temporaryItemTy;
            index = rightChild(index);
          }
          else {
            //else swap places with the left child
            temporaryItemTy = PQueue[leftChild(index)];
            PQueue[leftChild(index)] = PQueue[index];
            PQueue[index] = temporaryItemTy;
            index = leftChild(index);
          }
        }
        else {
          //swap with left child if swapping with right child might
          //distort the heap structure 
          temporaryItemTy = PQueue[leftChild(index)];
          PQueue[leftChild(index)] = PQueue[index];
          PQueue[index] = temporaryItemTy;
          index = leftChild(index);
        }
      }
    }        
  }
}


ItemTy PriorityQueue::front() const {
  

  //return the ItemTy at the top of the heap
  return PQueue[0];
}

bool PriorityQueue::full() const {
  
  //if PriorityQueue is full return true else return false
  if(Size == Capacity)
    return true;
  else
    return false;
}

bool PriorityQueue::empty() const {
  
  //if PriorityQueue is empty return true else return false
  if(Size == 0)
    return true;
  else
    return false;
}

