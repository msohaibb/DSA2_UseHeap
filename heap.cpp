/*
 * Created by Sohaib Bhatti
 * Data Structures and Algorithms II
 * Program 2 - 10/19/2021
 * This defines the heap class.
 * */


#include "heap.h"

#include <string>

heap::heap(int size):mapping(size * 2){
    // Allocate space for the nodes (0 slot is not used)
    data.resize(size + 1);
    this->size = size;
    // since it starts out empty, make fullSpaces 0
    this->fullSpaces = 0;
}
// inserts node with specified ID, key, and pointer (if given)
int heap::insert(const std::string &id, int key, void *pv) {
    if(fullSpaces == size){
        return 1;
    }
    // checks to see if heap has the ID through the hash table
    if(mapping.contains(id)){
        return 2;
    }

    fullSpaces++;

    // fills a node
    data[fullSpaces].stringID = id;
    data[fullSpaces].key = key;
    data[fullSpaces].p = pv;

    // inserts it into the end of the heap and percolates it up
    mapping.insert(id, &data[fullSpaces]);
    percolateUp(fullSpaces);

    return 0;

}
int heap::setKey(const std::string &id, int key){
    bool b;
    node *pn = static_cast<node *> (mapping.getPointer(id, &b));

    // checks to see if heap has the ID through the hash table
    if(!b){
        return 1;
    }
    // stores the previous key to check whether to percolate up or down, and changes key
    int temp = pn->key;
    pn->key = key;

    if(temp > key){
        percolateUp(getPos(pn));
    }

    else if(temp < key){
        percolateDown(getPos(pn));
    }

    return 0;
}

int heap::deleteMin(std::string *pId , int *pKey, void *ppData){
    if (fullSpaces == 0){
        return 1;
    }
    // min value percolates up, so it must be at the beginning of the array
    node min = data[1];
    if (pId != nullptr){
        *pId = min.stringID;
    }
    if (pKey != nullptr) {
        *pKey = min.key;
    }
    if (ppData != nullptr) {
        *(static_cast<void **> (ppData)) = min.p;
    }
    // move last object up and percolate it down to ensure everything is sorted
    fullSpaces--;
    data[1] = data[fullSpaces];
    percolateDown(1);

    // remove ID from hash table
    mapping.remove(min.stringID);

    return 0;
}

int heap::remove(const std::string &id, int *pKey, void *ppData){
    bool b;
    node *pn = static_cast<node *> (mapping.getPointer(id, &b));

    if(!b){
        return 1;
    }
    if(pKey != nullptr) {
        *pKey = pn->key;
    }
    if(ppData != nullptr){
        *(static_cast<void **> (ppData)) = pn->p;
    }

    // stores the previous key to check whether to percolate up or down, and changes key
    int temp = pn->key;
    *pn = data[fullSpaces--];

    if(temp > pn -> key){
        percolateUp(getPos(pn));
    }
    else if(temp < pn -> key){
        percolateDown(getPos(pn));
    }

    return 0;
}

void heap::percolateUp(int posCur){
    // keeps iterating until the child is greater than the parent
    while(data[posCur].key < data[posCur/2].key) {
        // keep swapping
        node temp = (data[posCur/2]);
        data[posCur/2] = data[posCur];
        data[posCur] = temp;

        // set new pointers for both the parent and the child
        mapping.setPointer(data[posCur].stringID, &data[posCur]);
        posCur/=2;
        mapping.setPointer(data[posCur].stringID, &data[posCur]);
    }
}

// adapted from textbook
void heap::percolateDown(int posCur) {
    int child;
    node temp = data[posCur];

    for(; posCur * 2 <= fullSpaces; posCur = child){
        child = posCur * 2;
        if(child != fullSpaces && data[child + 1].key < data[child].key){
            ++child;
        }
        if(data[child].key < temp.key){
            data[posCur] = data[child];
            mapping.setPointer(data[posCur].stringID, &data[posCur]);
        }
        else{
            break;
        }
    }
    data[posCur] = temp;
    mapping.setPointer(data[posCur].stringID, &data[posCur]);
}

// provided
int heap::getPos(node *pn){
    int pos = pn - &data[0];
    return pos;
}