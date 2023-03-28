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
#include "error.h"
#include "simpio.h"
using namespace std;

/*
 * Struct definition to store string as value instead of int version
 * in cell.h
 */
struct Cell {
    string value;
    Cell *next;
};

/*
 * Function: readList
 * -------------------
 * Prompts the user 
 */
Cell* readList() {
    Cell* head = nullptr;
    Cell* tail = nullptr;

    while (true) {
        string line = getLine("Next entry: ");
        if (line == "") break;

        Cell* cell = new Cell;
        cell->value = line;
        cell->next = nullptr;

        if (head == nullptr) {
            head = tail = cell;
        } else {
            tail->next = cell;
            tail = cell;
        }
    }
    return head;
}
