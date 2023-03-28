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

#include "myDeque.h"
#include "error.h"
using namespace std;


Deque::Deque() {
    head = new DoublyLinkedCell;
    tail = new DoublyLinkedCell;
    head->prev = nullptr;
    head->next = tail;
    tail->prev = head;
    tail->next = nullptr;
    length = 0;
}

Deque::~Deque() {
    while (head != nullptr) {
        DoublyLinkedCell* next = head->next;
        delete head;
        head = next;
    }
}

bool Deque::isEmpty() const {
    return size() == 0;
}

int Deque::size() const {
    return length;
}

void Deque::pushFront(int value) {
    DoublyLinkedCell* newCell = new DoublyLinkedCell;
    newCell->value = value;
    newCell->next = head->next;
    newCell->prev = head;
    newCell->next->prev = newCell;
    head->next = newCell;
    ++length;
}

void Deque::pushBack(int value) {
    DoublyLinkedCell* newCell = new DoublyLinkedCell;
    newCell->value = value;
    newCell->next = tail;
    newCell->prev = tail->prev;
    newCell->prev->next = newCell;
    tail->prev = newCell;
    ++length;
}

int Deque::peekFront() const {
    if (isEmpty()) {
        error("Deck is empty.");
    }
    return head->next->value;
}

int Deque::peekBack() const {
    if (isEmpty()) {
        error("Deck is empty.");
    }
    return tail->prev->value;
}

int Deque::popFront() {
    if (isEmpty()) {
        error("Deck is empty.");
    }
    DoublyLinkedCell* popCell = head->next;
    int result = popCell->value;

    head->next = popCell->next;
    popCell->next->prev = head;
    delete popCell;

    return result;
}

int Deque::popBack() {
    if (isEmpty()) {
        error("Deck is empty.");
    }
    DoublyLinkedCell* popCell = tail->prev;
    int result = popCell->value;

    tail->prev = popCell->prev;
    popCell->prev->next = tail;
    delete popCell;

    return result;
}
