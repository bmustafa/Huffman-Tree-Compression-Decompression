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
#include <bitset>
#include <cstdlib>
#include <string>
#include <cstring>
#include <fstream>
#include "PriorityQueue.h"

using namespace std;
typedef PriorityQueue::HTreePtr ItemTy;
typedef PriorityQueue::HTree HTree;


//function to insert char from frequency queue into pqueue
void insertIntoPQueue(const size_t* freqArray, PriorityQueue& Pq) {

  //go through freqArray 
  for(int i = 0; i < 256; i++) {
    char charToInsert;

    //if an index has a value greater than zero
    if(freqArray[i] > 0) {

      //create a ItemTy for that index - to insert into PriorityQueue 
      charToInsert = i;
      HTree* HtreePointer = new HTree(freqArray[i], charToInsert);
      ItemTy temporaryItem(HtreePointer);
      Pq.enqueue(temporaryItem);
    }   
  }
}

//gets the character representation of a tree
string charRepOfTree(HTree* Htree, string CarrierString = "") {

  //if the HTree is empty return and empty string
  string carryString = CarrierString;
  if(Htree ==  NULL) {
    carryString = carryString + "";
    return carryString;
  }
  else {

    //if the node holds char '*' add 'I' to the string and call the
    //function recursively with the left and right children nodes
    if(Htree -> Value == '*') {
      carryString = carryString + 'I';
      carryString = charRepOfTree(Htree -> Left, carryString);
      carryString = charRepOfTree(Htree -> Right, carryString);
      return carryString;
    }
    else {

      //if the node holds any char other than '*' , add 'L' and that
      //char to the string and return the string 
      if(Htree -> Value != '*')
        carryString = carryString + 'L' + (Htree -> Value);
      return carryString;
    }
  }
}


//convert a char into is ascii
string intToBitString(char value) {

  //converts a character into a string of '1' and '0's based on its
  //ascii value
  string bs = "";
  for(int i = 0; i<8; i++) { 
    bs = (value %2 > 0 ? "1" : "0") + bs;
    value >>= 1;
  }
  return bs;
}


//converts ascii into a byte
char returnAByte(string bs) {
 
  //using multiples of two, convert a string of '1's and '0's into a 
  //character 
  int j = 128;
  int asKeyTracker = 0;
  char AsKeyFinal = 0;
  for(int i = 0; bs[i] != '\0'; i++) {
    if(bs[i] == '1')
      asKeyTracker = asKeyTracker + j;
    else
      ;
    j = j / 2;
  }
  AsKeyFinal = asKeyTracker;
  return AsKeyFinal;
}

//utility for freqArray
size_t sizeUtility(string inputS) {

  //return the size of a string by calling itself recursively
  if(inputS == "")
    return 0;
  return 1 + sizeUtility(inputS.substr(1));
}


//create an array with frequencies from a string
size_t freqArray(const string& inputString, size_t* freqArray) {

  //find size of the provided string
  size_t size = sizeUtility(inputString);

  //traverse through the string
  for(size_t i = 0; i < size; i++) {

    //for every char in string, increment its index in the frequency array
    //*freq array is changed by reference*
    int placeHolderInt = inputString[i];
    freqArray[placeHolderInt]++;
  }

  //return size
  return size;
}


//creates an array of encodings from a tree
void encodingArrayFunc(const HTree* firstNode, string* encodingArray, 
                   string cumulative = "") {

  //if the node is null do nothing
  if(firstNode == NULL) {
    ;
  }
  else { 

    //if the node holds char '*' then function call itself recursively
    //giving its left and right children nodes and adding '0' or '1' 
    //to the string respectively
    if(firstNode -> Value == '*') {
      encodingArrayFunc(firstNode -> Left, encodingArray, cumulative + '0');
      encodingArrayFunc(firstNode -> Right, encodingArray, cumulative + '1');
    }
    else {

      //if the node holds any other char, add the binary string to the
      //char's index in the array
      int placeHolderInt = firstNode -> Value;
      encodingArray[placeHolderInt] = cumulative;
    }
  }
}


//creates one huffman tree from a freq array
void buildOneHuf(const size_t* freqArray, PriorityQueue& Pqueue) {

  //takes a freq array and fills Pqueue with ItemTy holding the
  //characters and their frequencies
  insertIntoPQueue(freqArray, Pqueue);

  //Dequeue's twice and then combines the dequeue'd nodes to make a new
  //node which is then put back into the PriorityQueue
  HTree* placeHolder1;
  HTree* placeHolder2;
  while(Pqueue.length() > 1) {
    placeHolder1 = Pqueue.front().Pointer;
    Pqueue.dequeue();
    placeHolder2 = Pqueue.front().Pointer;
    Pqueue.dequeue();
    HTree* HtreePlaceHolder= new HTree((placeHolder1 -> Key + placeHolder2
                             -> Key), '*', placeHolder2, 
                            placeHolder1);
    ItemTy placeHolder3 = HtreePlaceHolder;
    Pqueue.enqueue(placeHolder3);
  }
}


//create a huffman tree from its character representation
HTree* huffFromCharRep(string& stringRep, HTree* htreeptr = NULL) {
  //if string is the empty string, return 
  if(stringRep == "")
    return htreeptr;

  //if the first char in the string is 'I', create a node and call the
  //function recursively to fill the left and right children nodes
  //also substract first char from the string
  if(stringRep[0] == 'I') {
    stringRep = stringRep.substr(1);
      return htreeptr = new HTree(0, '*', huffFromCharRep(stringRep), 
                           huffFromCharRep(stringRep));
  }
  else {

    //if the first char in the string is not 'I', create a node containing
    //second char of the string and subtract the first two chars from
    //the string
    char tempChar = stringRep[1];
    stringRep = stringRep.substr(2);
    return htreeptr = new HTree(0, tempChar);
      }
} 

//take a string of char, character by character swap it with its encoding
//then output that encoding as characters, one at a time 
void bufferAndOutput(char originalString, string* encodingArray,bool trigger, 
                     string& bufferString) {
  
  int placeHolder = originalString;
 
  //look up the encoding of the first char and add it to the
  //buffer string
  bufferString = bufferString + encodingArray[placeHolder];

  //if the buffer string is more than 8 characters long
  while(sizeUtility(bufferString) > 8) {

    //output a character from the first eight '1' and '0's of the string
    //by converting the string from binary to ascii
    cout << returnAByte(bufferString.substr(0,8));
    
    //and then substract the first 8 characters from the string
    bufferString = bufferString.substr(8);
  }
  
  //if the bufferstring is not 8 characters long and the input string has
  //been used up; add padding to the end of the buffer string till it is
  //8 characters long and then output it
  if(bufferString != "" and trigger == false) {
    while(sizeUtility(bufferString) != 8)
      bufferString = bufferString + '0';
    cout << returnAByte(bufferString);
  }
}

string char_to_bin( char bin_out ) {

  // Convert ASCII to binary
  string newS = "";
  bitset<8> b(bin_out);
  newS += b.to_string();
  return newS;
	
}
    
//take bufferstring by ref and adds new string to it
//and then starts decoding using HTree and outputs it
void CompressedToOriginal(char inputChar, HTree* encodingTree, 
                          string& buffer, size_t& size) {

  //binary string representation of the input character is added
  // to the buffer  
  bool trigger = true;
  buffer = buffer + char_to_bin(inputChar);
  //cout << char_to_bin(inputChar);
  while(trigger) { 
    string temporaryStr = buffer;
    HTree* temporaryPtr = encodingTree;
    int i = 0;
   
    //traverses the encoding Tree as long as it is passing through
    //internal nodes and the buffer has characters to spare
    while(temporaryPtr -> Value == '*' and temporaryStr != "") {
      if(temporaryStr[0] == '0') 
        temporaryPtr = temporaryPtr -> Left;
      if(temporaryStr[0] == '1')
          temporaryPtr = temporaryPtr -> Right;

      //with each traversal the string is shortened
      if(sizeUtility(temporaryStr) != 1)
        temporaryStr = temporaryStr.substr(1);
      else
        temporaryStr = "";
      i++;
    }

    //if the string has been used up but we are at an interior node, the 
    //trigger is set false, stopping the function from running
    if(temporaryPtr -> Value == '*')
      trigger = false;

    //if the traversal has us at a node with a character other than '*'
    //the buffer is shortened at the front, the character is output
    //and size is decremeneted (stops us from outputting the buffer '0's)
    if(size > 0) {
        if(temporaryPtr -> Value != '*') {
          buffer = buffer.substr(i);
          size--;
          cout << temporaryPtr -> Value;
        }
    }
    else {

      //if size is too small, trigger is set to false and function stops
      trigger = false;
    }
  }
}


void printtree(HTree* htree) {

  //prints out the structure of the HTree.
  if(htree == NULL){
    ;
  }
  else {
    cout << " value " << htree -> Value << " key " << htree -> Key << endl;
    if(htree -> Left != NULL) {
      cout << "left" << endl;
      printtree(htree -> Left);
    }
    if( htree -> Right != NULL) {
      cout << "right" << endl;
      printtree(htree -> Right);
    }
  }
}



int main(int argc, char *argv[]) {

  //compression
  //initialize arrays, strings and priorityQueue
  PriorityQueue Pq(256);
  size_t* frequencyArray = new size_t[256];
  string* encodingArray = new string[256];
  string charOfTree;
  string buffer = "";
  string bufferCompress = "";
  int placeHolder;

  if (argc < 2) {
    cerr << "Usage: wc <filename>" << endl;
    return 1;
    }
  
  //if compress command is called
  if((string)argv[0] == "compress" or (string)argv[0] == ".\\compress") {
    ifstream in(argv[1]);
    
    char c;
    size_t chars = 0;
    
    //read in characters and increment their index in frequency array
    while (in.read(&c, 1)) {
      placeHolder = c;
      chars++;
      frequencyArray[placeHolder]++;
    }

    //build a huffman tree from frequency array and using priority queue
    buildOneHuf(frequencyArray, Pq);

    //use huffman tree to give encodings to the characters and place
    //encodings in array
    encodingArrayFunc(Pq.front().Pointer, encodingArray);

    //get the string representation of the huffman tree
    charOfTree =  charRepOfTree(Pq.front().Pointer);

    //output size and string representation of huffman tree
    cout << chars;
    cout << charOfTree;
    cout << "i";
    bool trigger = true;
    size_t i = 1;
    in.close();
    in.open(argv[1]);

    //output characters after compression
    while (in.read(&c, 1)) {
      if(i == chars)
        trigger = false;
      bufferAndOutput(c, encodingArray, trigger, bufferCompress);
      i++;
    }
  }

  //if decompress is called
  else  if((string)argv[0] == "decompress" or (string)argv[0] ==
           ".\\decompress") {
    ifstream in(argv[1]);
    
    char c;
    char cPrev;
    size_t chars = 0;
    bool trigger2 = true;
    bool trigger3 = true;
    string stringSize = "";
    charOfTree = "";
    HTree* newEncoding;
    size_t temporarySize = 0;
    
    while (in.read(&c, 1)) {  
      if(trigger3 == false) {
       
        //decompression
        //once the you have the size and the reconstructed huffman tree
        //convert the compressed file back into its decompresssed version
        CompressedToOriginal(c, newEncoding, buffer, temporarySize);
      }

      //calculate size using input until 'I' character appears
      if(c == 'I')
        trigger2 = false;    
      if(trigger2)
        stringSize = stringSize + c;
      if(trigger2 == false and trigger3) {
        chars = atoi(stringSize.c_str());
        temporarySize = chars;

        //once size has been calculated, reconstruct the huffman tree using 
        //the string representation
        if(cPrev != 'L' and c == 'i') {
          trigger3 = false;
          newEncoding = huffFromCharRep(charOfTree);
        }
        if(trigger3)
          charOfTree = charOfTree + c;
        }
      cPrev = c; 
    }    
  }
}
