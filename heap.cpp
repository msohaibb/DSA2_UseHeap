//
// Created by Sohaib Bhatti on 9/22/2021.
//

#include "heap.h"
heap::heap(int size):mapping(size * 2){
    // Allocate space for the nodes (0 slot is not used)
    data.resize(size + 1);
    this->size = size;
    this->fullSpaces = 0;
}

int heap::insert(const std::string &id, int key, void *pv) {
    if(fullSpaces == size){
        return 1;
    }

    bool hello = mapping.contains(id);
    if(hello){
        return 2;
    }

    fullSpaces++;

    data[fullSpaces].stringID = id;
    data[fullSpaces].key = key;
    data[fullSpaces].p = pv;

    mapping.insert(id, &data[fullSpaces]);
    percolateUp(fullSpaces);

    return 0;

}
int heap::setKey(const std::string &id, int key){
    bool b;
    node *pn = static_cast<node *> (mapping.getPointer(id, &b));

    if(!b){
        return 1;
    }
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


    data[1] = std::move(data[fullSpaces--]);
    percolateDown(1);

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
    while(data[posCur].key < data[posCur/2].key) {
        node temp = (data[posCur/2]);
        data[posCur/2] = data[posCur];
        data[posCur] = temp;
        mapping.setPointer(data[posCur].stringID, &data[posCur]);
        posCur/=2;

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
        else {
            break;
        }
    }
    data[posCur] = temp;
    mapping.setPointer(data[posCur].stringID, &data[posCur]);
}



int heap::getPos(node *pn){
    int pos = pn - &data[0];
    return pos;
}