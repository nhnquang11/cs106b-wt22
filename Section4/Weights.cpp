/*
 * CS106B Section Handout Test Harness: Section 4
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
#include "vector.h"
using namespace std;

bool isMeasurableRec(int amount, const Vector<int>& weights, int index);

/*
 * Function: isMeasurable
 * -----------------------
 * Write a recursive function that determines whether it is
 * possible to measure out the desired target amount with a
 * given set of weights, which is stored in the vector weights.
 */
bool isMeasurable(int amount, const Vector<int>& weights) {
    return isMeasurableRec(amount, weights, 0);
}

/*
 * Function: isMeasurableRec
 * -----------------------
 * A helper function using recursion to determine whether it is
 * possible to measure out the desired target amount with a given
 * set of weights.
 */
bool isMeasurableRec(int amount, const Vector<int>& weights, int index) {
    if (index == weights.size()) {
        return amount == 0;
    } else {
        return isMeasurableRec(amount,                  weights, index + 1) ||
               isMeasurableRec(amount + weights[index], weights, index + 1) ||
               isMeasurableRec(amount - weights[index], weights, index + 1);
    }
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("Provided Test: Run the provided examples.") {
    EXPECT_EQUAL(isMeasurable(2, {1, 3}), true);
    EXPECT_EQUAL(isMeasurable(5, {1, 3}), false);
    EXPECT_EQUAL(isMeasurable(6, {1, 3, 7}), true);
}
