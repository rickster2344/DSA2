#include "heap.h"
#include <iostream>
#include <vector> //TODO: remove this

//constructor for heap object
//: is a member initializer which initializes the hashTable data member prior to the constructor body executes
// constructs a hashTable with at least twice the capacity of the heap to reduce the chances of rehashing
heap::heap(int capacity) : mapping(capacity *2){ 
   data.resize(capacity +1); //initially allocating memory for data vector, +1 since the heap is 1 indexed. 
   h_capacity = capacity; //initializing h_capacity to the capacity
   h_filled = 0; //initializing the size of the heap to 0
}

void heap::percolateDown(int posCur){
   //if we are at a leaf, we're immediately done. this also ensures poscur is within the heap 
   if(leftChild(posCur) > h_filled){
      return;
   }

   node tempNode = data[posCur];
   //compare child w parent and bubble down if HOP not satisfied
   //swap w smaller of two children if both are larger
   //if both children are equal, will end up swapping w left node
   //if we get to a leaf, exit the loop

   int tempPos; 

   while(leftChild(posCur) <= h_filled){//continue as long as we are not in a leaf
      tempPos = posCur;
      //check if a right child exists, if it does check if it is bigger
      if(rightChild(tempPos) <= h_filled){
         if(tempNode.key > data[rightChild(tempPos)].key){
            posCur = rightChild(tempPos);
         }
      }

      //definitely has a left child, check if left child is bigger than its parent, and not bigger or equal to  than its sibling
      if(tempNode.key > data[leftChild(tempPos)].key && data[leftChild(tempPos)].key <= data[rightChild(tempPos)].key){
         posCur = leftChild(tempPos);
         //note if left = right, we want to swap left
      }

      if(tempPos != posCur){//if the current position has changed, we know one of its children are bigger
            data[parent(posCur)] = data[posCur]; //make value of parent = smaller child
            mapping.setPointer(data[parent(posCur)].id, &data[parent(posCur)]); 
            tempPos = posCur;
      }
      else{// if the current position has not changed, the parent is smaller than both children and this is the right spot
         break;
      }
   }

   //insert node to the spot found
   data[posCur] = tempNode;
   mapping.setPointer(data[posCur].id, &data[posCur]);
}

void heap:: percolateUp(int posCur){
   // if we are at the root then we finished
   if (posCur == 1){
      return;
   }
   node tempNode = data[posCur];
   //compare parent w child and bubble up if HOP not satisfied
   while (tempNode.key < data[parent(posCur)].key && posCur!= 1){
      data[posCur] = data[parent(posCur)];
      mapping.setPointer(data[parent(posCur)].id, &data[posCur]); //moving the pointer in the hashtable to the child
      
      posCur = parent(posCur);
   }
   //insert node into the spot found 
   data[posCur] = tempNode;
   mapping.setPointer(data[posCur].id, &data[posCur]);
}

int heap::parent(int i){return i/2;}
int heap::leftChild(int i){return 2*i;}
int heap::rightChild(int i){return 2*i+1;}

int heap::getPos(node *pn){// requires pointer to be valid to work. 
   // pointer arithmetic
   //(pointer to node - address of the array)/sizeof(node) = index
   int pos = pn - &data[0];
   return pos;
}

int heap::insert(const std::string &id, int key, void *pv){
   if (h_filled == h_capacity){//heap is full 
      return 1;
   }

   if (mapping.contains(id)){//a node with the given id already exists
      return 2;
   }
   //passes both, insert into heap by placing at the bottom of the heap, and then percolating up until HOP is satisfied
   
   // //Create new node
   // node newNode;
   // newNode.id = id;
   // newNode.key = key;
   // newNode.pData = pv;

   //insert into heap then percolate up
   h_filled++;
   data[h_filled].id = id;
   data[h_filled].key = key;
   data[h_filled].pData = pv;
   
   // data[h_filled] = newNode; // placing the newNode at the end of the heap
   mapping.insert(id, &data[h_filled]); //mapping to that item in the hashtable

   percolateUp(h_filled); //percolate up the bottom pos
   return 0;//returns 0 if successful!
}

int heap::setKey(const std::string &id, int key){
   if(!mapping.contains(id)){
      return 1; //returns 1 if the node with the given id does not exist in the heap
   }
   node *nodePtr = static_cast<node *>(mapping.getPointer(id)); // converts datatypes, goes from <void *> to type <node *>
   //refresher on pointers, int dog=2; int* cat = &dog; *cot=2;
   //(*node_ptr).key is the same as node_ptr->key
   // this is getting the value of the storage variable "key" in an object, which is pointed to by nodeptr. () is important because it first dereferences nodeptr then gets the .key, 
   // otherwise it does nodeptr.key first and then tries to dereference it and it gets sad. 
   // & = address of, * is dereference (get data at)

   // //set the node's key to the new key
   int oldKey = nodePtr->key; 
   nodePtr->key = key;

   // if the key is smaller, it would move up the tree, otherwise it would move down
   if (oldKey < key){
      percolateUp(getPos(nodePtr));
   }
   else{
      percolateDown(getPos(nodePtr));
   }

   //could also just checks in the functions since they check internally anyways;
   // percolateUp(getPos(nodePtr));
   // percolateDown(getPos(nodePtr));
   return 0; //success!
}

int heap::deleteMin(std::string *pId, int *pKey, void *ppData){
   //deletes the root of the heap since the root is the minimum number.
   
   if(h_filled == 0){
      return 1; //heap empty
   }

   if(pId != nullptr){
      *pId = data[1].id;// set pId to the key of root if pId is provided
   }
   if(pKey!= nullptr){
      *pKey = data[1].key;// set pKey to the key of root if pKey is provided
   }
    if(ppData!= nullptr){
      *(static_cast<void **> (ppData)) = data[1].pData;
   }  
  
   mapping.remove(data[1].id); // remove the id of the root from the tree
   data[1] = data[h_filled]; // replace root with the bottom of the tree
   h_filled--; //don't actually need to clear the stuff stored in data[h_filled], since it will just be treated as empty and written over 
   
   percolateDown(1);// percolate down from the root to satisfy HOP
   return 0; //successful deleteMin!
}

int heap::remove(const std::string &id, int *pKey, void *ppData){
   if(!mapping.contains(id)){
      return 1; //if the id doesnt exist in the heap
   }
   node *nodePtr = static_cast<node *>(mapping.getPointer(id)); // converts datatypes, goes from <void *> to type <node *>
  
   if(pKey!= nullptr){
      *pKey = nodePtr->key;// set pKey to the key of root if pKey is provided
   }
    if(ppData!= nullptr){
      *(static_cast<void **>(ppData)) = nodePtr-> pData;// set ppData to the ptr of root if ppData is provided
   }  
   
   mapping.remove(nodePtr->id);// remove the id of the removed item from the mapping
   data[getPos(nodePtr)] = data[h_filled]; //move the bottom most item into the position of the removed item
   h_filled--;//must decrement before the percolate down is performed 


   //percolate up or down as necessary, internal checks will deal with which one is necessary
   percolateDown(getPos(nodePtr));
   percolateUp(getPos(nodePtr));
   return 0; //if it is successful
}


void heap::printHeap()
{
    //print the heap so that it looks like a binary tree
    int level = 0;
    int levelSize = 1;
    int levelCount = 0;

    int numLevels = std::log2(h_filled) + 1;
    for (int i = 1; i <= h_filled; i++)
    {

        if(!mapping.contains(data[i].id))
        {
            // the data is in the heap, but it's not in the hash table
            throw "THIS IS BREAKING HERE";
        }
        // print appropriate beginning tabs where the first level has the most tabs and the last level has none
        if(levelCount == 0)
        {
            for (int j = 0; j < std::pow(2,numLevels - (level+1)); j++)
            {
                std::cout << "\t";
            }
        }
        std::cout << data[i].id << ": " << data[i].key;
        // print appropriate intermediate tabs
        if (levelCount < levelSize - 1)
        {
            for (int j = 0; j < std::pow(2,numLevels - (level+1)); j++)
            {
                // if we are in the middle
                if( levelCount == levelSize/2 -1)
                {
                    for (int j = 0; j < std::pow(2,numLevels - 1); j++)
                    {
                        std::cout << "\t";
                    }
                    break;
                }
                else
                {
                    std::cout << "\t";
                }
            }
        }
        levelCount++;
        if (levelCount == levelSize)
        {
            std::cout << std::endl;
            level++;
            levelSize = std::pow(2, level);
            levelCount = 0;
        }
    }
    std::cout << std::endl;

}


// An example of a call to the hash table's setPointer member function:
//   mapping.setPointer(data[posCur].id, &data[posCur]);

// An example of a call to the hash table's getPointer member function:
//   node *pn = static_cast<node *> (mapping.getPointer(id, &b));

// Filling in ppData in deleteMin:
//   *(static_cast<void **> (ppData)) = data[1].pData;
