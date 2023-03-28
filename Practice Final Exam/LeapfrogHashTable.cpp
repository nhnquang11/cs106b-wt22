#include "LeapfrogHashTable.h"
using namespace std;

/* Stash the hash function for later, and mark all spots empty. */
LeapfrogHashTable::LeapfrogHashTable(HashFunction<string> hashFn) {
    this->hashFn = hashFn;

    numElems = 0;
    numSlots = hashFn.numSlots();
    elems = new Slot[numSlots];
    for (int i = 0; i < numSlots; ++i) {
        elems[i].link = NOT_LINKED;
        elems[i].type = SlotType::EMPTY;
    }
}

/* Clean our memory. */
LeapfrogHashTable::~LeapfrogHashTable() {
    delete[] elems;
}

/* Jump to the initial spot and walk the chain to see if the element is there. */
bool LeapfrogHashTable::contains(const string& key) const {
    /* There are definitely no elements there if the table is empty. */
    if (isEmpty()) return false;

    int i = hashFn(key);
    /* Walk through chain. */
    while (elems[i].type == SlotType::FILLED) {
        /* Found it. */
        if (elems[i].value == key) return true;
        /* The end of the chain and not found an element yet -> give up. */
        if (elems[i].link == NOT_LINKED) return false;
        i = elems[i].link;
    }
    return false;
}

bool LeapfrogHashTable::insert(const string& key) {
    /* Can't insert ket if the table is full. */
    if (size() == numSlots) return false;

    int hashCode = hashFn(key);
    int curr = hashCode;
    int prev = hashCode;

    /* Walk through chain. */
    while (elems[curr].type == SlotType::FILLED && elems[curr].link != NOT_LINKED) {
        /* The element is already in the table. */
        if (elems[curr].value == key) return false;
        /* Move to next position in the chain. */
        int next = elems[curr].link;
        prev = curr;
        curr = next;
    }
    /* Check the end of the chain out of the loop due to fencepost problem. */
    if (elems[curr].value == key) return false;
    /* Walking forward until fiding an empty slot. */
    if (elems[curr].type == SlotType::FILLED) {
        prev = curr;
        while (curr != hashCode - 1) {
            if (elems[curr].type == SlotType::EMPTY) break;
            curr = (curr + 1) % numSlots;
        }
    }

    /* Update current spot and the link to it from last slot in the chain. */
    if (prev !=  curr) {
        elems[prev].link = curr;
    }
    elems[curr].type = SlotType::FILLED;
    elems[curr].value = key;
    elems[curr].link = NOT_LINKED;
    ++numElems;
    return true;
}

/* Return the cached size. */
int LeapfrogHashTable::size() const {
    return numElems;
}

/* We're empty if our size is zero. */
bool LeapfrogHashTable::isEmpty() const {
    return size() == 0;
}

/* Print out the table. */
void LeapfrogHashTable::printDebugInfo() const {
    for (int i = 0; i < numSlots; ++i) {
        if (elems[i].type == SlotType::FILLED) {
            cout << elems[i].value << " ";
        } else {
            cout << ". ";
        }
    }
    cout << endl;
    for (int i = 0; i < numSlots; ++i) {
        if (elems[i].type == SlotType::FILLED) {
            cout << i << ": ";
            cout << elems[i].value << " ";
            if (elems[i].link != NOT_LINKED) {
                cout << "(" << elems[i].link << ")";
            }
            cout << endl;
        }
    }
}
