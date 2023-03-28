/*
 * CS106B Section Handout Test Harness: Section 7
 * ----------------------------------------------
 * These problems have been galvanized from years of
 * section handouts that have been worked on by numerous
 * instructors and TA's. Codified by Neel Kishnani for 
 * Winter 2022.
 *
 * A huge thank you to Keith Schwarz and Julie Zelenski
 * for creating an amazing testing harness!
 */
#include <iostream>
#include <string>
#include "testing/SimpleTest.h"
#include "cell.h"
#include "utility.h"
#include "error.h"
using namespace std;

void partitionList(Cell* list, Cell*& smaller, Cell*& pivot, Cell*& bigger);

/*
 * Function: quickSort
 * --------------------
 * Takes in a linked list and sorts it in-place using the 
 * quickSort algorithm.
 */
void quicksort(Cell*& list) {
    /* If the length of the list is 0 or 1, we're done. */
    if (list == nullptr || list->next == nullptr) return;

    Cell* pivot = list;
    Cell* rest = list->next;
    pivot->next = nullptr;

    Cell* smaller = nullptr;
    Cell* bigger = nullptr;
    partitionList(rest, smaller, pivot, bigger);

    quicksort(smaller);
    quicksort(bigger);

    pivot = concat(pivot, bigger);
    smaller = concat(smaller, pivot);
    list = smaller;
}

void prependCell(Cell* toPrepend, Cell*& list) {
    toPrepend->next = list;
    list = toPrepend;
}

void partitionList(Cell* list, Cell*& smaller, Cell*& pivot, Cell*& bigger) {
    while (list != nullptr) {
        Cell* next = list->next;
        if (list->value == pivot->value) {
            prependCell(list, pivot);
        } else if (list->value > pivot->value) {
            prependCell(list, bigger);
        } else {
            prependCell(list, smaller);
        }
        list = next;
    }
}


/* * * * * Provided Tests Below This Point * * * * */
PROVIDED_TEST("Small test for quickSort") {
    Cell* list = createListFromVector({7, 5, 87, 41, 137, 1, 2});
    Cell* sorted = createListFromVector({1, 2, 5, 7, 41, 87, 137});
    quicksort(list);
    EXPECT(listEqual(list, sorted));
    freeList(list);
    freeList(sorted);
}
