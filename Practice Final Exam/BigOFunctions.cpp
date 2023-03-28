/* The functions you should analyze. Do not modify this file or write any code
 * here; your task is just to read it and work out the runtimes.
 */
#include "BigOFunctions.h"
#include "priorityqueue.h"
#include "hashset.h"
#include "vector.h"
#include "random.h"
#include <iostream>
using namespace std;

void iterativePrinting(int n) {
    for (int row = 0; row < n; row++) {
        if (row % 1000 == 0) {
            for (int col = 0; col < n; col++) {
                cout << '*';
            }
        }
        cout << endl;
    }
}

void iterativePQ(int n) {
    /* This priority queue type is backed by a binary heap, the same way the
     * HeapPQueue from Assignment 6 was.
     */
    PriorityQueue<int> pq;

    for (int i = 0; i < n; i++) {
       for (int j = 0; j < i; j++) {
           pq.enqueue(i * n + j, i * n + j);
       }
    }

    int total = 0;
    while (!pq.isEmpty()) {
        total += pq.dequeue();
    }

    cout << total << endl;
}

void iterativeHashSet(int n) {
    /* This type is backed by a chained hash table that rehashes whenever the
     * load factor exceeds a small constant.
     */
    HashSet<int> mySet;

    for (int i = 0; i < n; i++) {
        mySet.add(i);
    }

    int total = 0;
    for (int i = 0; i < n; i++) {
        if (mySet.contains(i)) {
            mySet.remove(i);
            total += i;
        }
    }

    cout << total << endl;
}

void iterativeLooping(int n) {
    int i = n;
    int total = 0;
    while (i >= 1) {
        total += i;
        i = i / 2;
    }

    cout << total << endl;
}

void iterativeVector(int n) {
    Vector<int> values;
    for (int i = 0; i < n; i++) {
        values += randomInteger(0, n); // randomInteger runs in time O(1).
    }

    int total = 0;
    while (values.size() > 1) {
        /* Uses an algorithm with the same runtime complexity as mergesort. */
        values.sort();

        total += values[0];
        values.remove(0);
        values[0] = randomInteger(0, n);
    }

    cout << total << endl;
}
