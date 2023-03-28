/*
 * CS106B Section Handout Test Harness: Section 6
 * ----------------------------------------------
 * These problems have been galvanized from years of
 * section handouts that have been worked on by numerous
 * instructors and TA's. Codified for Qt and Ed by Neel
 * Kishnani for Winter 2022.
 *
 * A huge thank you to Keith Schwarz and Julie Zelenski
 * for creating an amazing testing harness!
 */


#include <iostream>
#include "testing/SimpleTest.h"
#include "testing/TextUtils.h"
#include "OurStack.h"
#include "error.h"
using namespace std;

const int kInitialSize = 4;

OurStack::OurStack() {
    elems = new int[kInitialSize];
    logicalSize = 0;
    allocatedSize = kInitialSize;
}

OurStack::~OurStack() {
    delete[] elems;
}

void OurStack::push(int value) {
    if (logicalSize == allocatedSize) {
        setCapacity(allocatedSize * 2);
    }
    elems[logicalSize] = value;
    logicalSize++;
}

int OurStack::pop() {
    int result = peek();
    logicalSize--;
    if (logicalSize < allocatedSize / 4) {
        setCapacity(allocatedSize / 2);
    }
    return result;
}
void OurStack::setCapacity(int capacity) {
    /* If we're asking to reduce our size below the initial starting size,
     * instead leave the array at its current size.
     */
    if (capacity < kInitialSize) return;

    int* newArr = new int[capacity];
    for (int i = 0; i < size(); i++) {
        newArr[i] = elems[i];
    }
    delete[] elems;
    elems = newArr;
    allocatedSize = capacity;
}

int OurStack::peek() const {
    if (isEmpty()) {
        error("Empty stack!");
    }
    return elems[logicalSize - 1];
}

int OurStack::size() const {
    return logicalSize;
}

bool OurStack::isEmpty() const {
    return logicalSize == 0;
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("Provided Test: Make sure the stack is functional.") {
    OurStack stack;
    EXPECT_EQUAL(stack.isEmpty(), true);
    for (int i = 0; i < 10000; i++) {
        stack.push(i);
    }
    EXPECT_EQUAL(stack.size(), 10000);
    for (int i = 0; i < 10000; i++) {
        int value = stack.pop();
        EXPECT_EQUAL(value, 9999-i);
    }
    EXPECT_EQUAL(stack.isEmpty(), true);
}
