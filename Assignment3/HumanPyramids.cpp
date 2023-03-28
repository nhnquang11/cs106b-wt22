#include "HumanPyramids.h"
#include "error.h"
#include "map.h"
#include "GUI/SimpleTest.h"
using namespace std;

/* Function prototypes */
double weightOnBackOf(int row, int col, int pyramidHeight);
double weightOnBackOf(int row, int col, int pyramidHeight, Map<pair<int ,int>, double>& lookUpTable);
bool inbound(int row, int col, int pyramidHeight);

/**
 * Function: weightOnBackOf
 * Usage: weightOnBackOf(row, col, pyramidHeight)
 * --------------------------------------------------
 * Calculates the weight that the person at the position (row, col)
 * of the pyramid with height pyramidHeight has to carry, assuming
 * that each person weighs exactly 160 pounds and each person supports
 * half the body weight of each of the person immediately above them,
 * plus half of the weight that each of those people are supporting.
 */
double weightOnBackOf(int row, int col, int pyramidHeight) {
    Map<pair<int, int>, double> lookUpTable;
    return weightOnBackOf(row, col, pyramidHeight, lookUpTable);
}

/**
 * Function: weightOnBackOf
 * Usage: weightOnBackOf(row, col, pyramidHeight, lookUpTable)
 * --------------------------------------------------
 * A helper function using recursion and memoization to speed up the calculating process.
 */
double weightOnBackOf(int row, int col, int pyramidHeight, Map<pair<int ,int>, double>& lookUpTable) {
    if (!inbound(row, col, pyramidHeight)) error("Invalid position");
    if (row == 0 && col == 0) {
        return 0.0;
    } else if (lookUpTable.containsKey(make_pair(row, col))) {
        return lookUpTable[make_pair(row, col)];
    } else {
        double leftWeight, rightWeight;
        /* left weight */
        if (inbound(row - 1, col - 1, pyramidHeight)) {
            leftWeight = (160.0 + weightOnBackOf(row - 1, col - 1, pyramidHeight, lookUpTable)) / 2;
        } else {
            leftWeight = 0.0;
        }
        /* right weight */
        if (inbound(row - 1, col, pyramidHeight)) {
            rightWeight = (160.0 + weightOnBackOf(row - 1, col, pyramidHeight, lookUpTable)) / 2;
        } else {
            rightWeight = 0.0;
        }
        double totalWeight = leftWeight + rightWeight;
        lookUpTable[make_pair(row, col)] = totalWeight;
        return totalWeight;
    }
}

/**
 * Function: inbound
 * Usage: inbound(row, col, pyramidHeight)
 * --------------------------------------------------
 * Checks whether the position (row, col) is inbound
 * the pyramid with height = pyramidHeight.
 */
bool inbound(int row, int col, int pyramidHeight) {
    /* row and col must be greater than or equal to 0. */
    if ((row < 0) || (col < 0)) return false;
    /* the position must be inside the pyramid. */
    if ((col > row) || (row >= pyramidHeight)) return false;
    return true;
}

/* * * * * * Test cases from the starter files below this point. * * * * * */

PROVIDED_TEST("Check Person E from the handout.") {
    /* Person E is located at row 2, column 1. */
    EXPECT_EQUAL(weightOnBackOf(2, 1, 5), 240);
}

PROVIDED_TEST("Function reports errors in invalid cases.") {
    EXPECT_ERROR(weightOnBackOf(-1, 0, 10));
    EXPECT_ERROR(weightOnBackOf(10, 10, 5));
    EXPECT_ERROR(weightOnBackOf(-1, 10, 20));
}

PROVIDED_TEST("Stress test: Memoization is implemented (should take under a second)") {
    /* This will take a LONG time to complete if memoization isn't implemented.
     * We're talking "heat death of the universe" amounts of time. :-)
     *
     * If you did implement memoization but this test case is still hanging, make
     * sure that in your recursive function (not the wrapper) that your recursive
     * calls are to your new recursive function and not back to the wrapper. If you
     * call the wrapper again, you'll get a fresh new memoization table rather than
     * preserving the one you're building up in your recursive exploration, and the
     * effect will be as if you hadn't implemented memoization at all.
     */
    EXPECT(weightOnBackOf(100, 50, 200) >= 10000);
}

STUDENT_TEST("Check person at the top of the pyramid.") {
    EXPECT_EQUAL(weightOnBackOf(0, 0, 5000), 0);
}
