/* File: PlayingFair.cpp
 * --------------------
 * This program uses mutually recursion to generate an order sequence
 * in which A and B should perform the activity so as to make it as close
 * to fair as possible.
 */
#include "PlayingFair.h"
#include "GUI/SimpleTest.h"
#include "error.h"
using namespace std;

/* Function prototypes */
string aSequenceOfOrder(int n);
string bSequenceOfOrder(int n);

/**
 * Function: aSequenceOfOrder
 * Usage: aSequenceOfOrder(n)
 * --------------------------------------------------
 * Returns the sequence of order n in which A and B should
 * perform the activity starting with A.
 * Example:
 *  n = 0: A
 *  n = 1: AB
 *  n = 2: ABBA
 */
string aSequenceOfOrder(int n) {
    if (n < 0) {
        error("The order must be a whole number.");
    }

    if (n == 0) {
        return "A";
    }
    return aSequenceOfOrder(n - 1) + bSequenceOfOrder(n - 1);
}

/**
 * Function: bSequenceOfOrder
 * Usage: bSequenceOfOrder(n)
 * --------------------------------------------------
 * Returns the sequence of order n in which A and B should
 * perform the activity starting with B.
 * Example:
 *  n = 0: B
 *  n = 1: BA
 *  n = 2: BAAB
 */
string bSequenceOfOrder(int n) {
    if (n < 0) {
        error("The order must be a whole number.");
    }

    if (n == 0) {
        return "B";
    }
    return bSequenceOfOrder(n - 1) + aSequenceOfOrder(n - 1);
}

/* * * * * * Provided Test Cases * * * * * */

PROVIDED_TEST("Sequences of order 3 are correct.") {
    /* Some very basic checks. */
    EXPECT_EQUAL(aSequenceOfOrder(3), "ABBABAAB");
    EXPECT_EQUAL(bSequenceOfOrder(3), "BAABABBA");
}

PROVIDED_TEST("Only characters should be As and Bs.") {
    for (int i = 0; i < 10; i++) {
        for (char ch: aSequenceOfOrder(i)) {
            EXPECT(ch == 'A' || ch == 'B');
        }
    }
}

PROVIDED_TEST("A-sequence of positive order should have equal As and Bs.") {
    /* Exclude the sequence of order 0, which is just a single character. */
    for (int i = 1; i < 10; i++) {
        int as = 0;
        int bs = 0;
        for (char ch: aSequenceOfOrder(i)) {
            if (ch == 'A') as++;
            else bs++;
        }

        EXPECT_EQUAL(as, bs);
    }
}

PROVIDED_TEST("Triggers error on negative inputs.") {
    /* The EXPECT_ERROR macro expects the given expression to call error(). Remember that
     * you need to guard against invalid inputs.
     */
    EXPECT_ERROR(aSequenceOfOrder(-137));
    EXPECT_ERROR(bSequenceOfOrder(-137));
}

/* * * * * * Student Test Cases * * * * * */

STUDENT_TEST("Floating-point numbers as the input order.") {
    EXPECT_EQUAL(aSequenceOfOrder(2.0), "ABBA");
    EXPECT_EQUAL(bSequenceOfOrder(2.0), "BAAB");
    EXPECT_ERROR(aSequenceOfOrder(-10.0));
    EXPECT_ERROR(bSequenceOfOrder(-10.0));
}
