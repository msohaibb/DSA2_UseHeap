//
// Created by Sohaib Bhatti on 9/22/2021.
//

#include "heap.h"
heap::heap(int capacity):mapping(capacity * 2){
    // Allocate space for the nodes (0 slot is not used)
    data.resize(capacity + 1);
}

int heap::insert(const std::string &id, int key, void *pv) {
    if(fullSpaces == size){
        return 1;
    }

    if(mapping.contains(id)){
        return 2;
    }

    data[++fullSpaces].stringID = id;
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

    if(pn->key > key){
        percolateUp(getPos(pn));
    }

    else if(pn->key < key){
        percolateDown(getPos(pn));
    }
    pn->key = key;
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


    data[1] = std::move(data[ fullSpaces--]);
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

    int old = pn->key;
    *pn = data[fullSpaces--];

    if(old > pn -> key){
        percolateUp(getPos(pn));
    }

    else if(old < pn -> key){
        percolateDown(getPos(pn));
    }

    return 0;
}


void heap::percolateUp(int posCur){
    int hole = ++fullSpaces;
    node temp = std::move(data[posCur]);

    data[0] = std::move(temp);
    for(; data[hole].key < data[hole/2].key; hole /= 2) {
        data[hole] = std::move(data[hole / 2]);
    }
    data[hole] = std::move(data[ 0 ]);
}

// adapted from textbook
void heap::percolateDown(int posCur) {
    int child;
    node temp = std::move(data[posCur]);

    for(; posCur * 2 <= fullSpaces; posCur = child){
        child = posCur * 2;
        if(child != fullSpaces && data[child + 1].key < data[child].key){
            ++child;
        }
        if(data[child].key < temp.key){
            data[posCur] = std::move(data[child]);
        }
        else {
            break;
        }
    }
    data[posCur] = std::move(temp);
}

int heap::getPos(node *pn){
    int pos = pn - &data[0];
    return pos;
}