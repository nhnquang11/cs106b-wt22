/* File: Sandpiles.cpp
 * --------------------
 * This program simulates sand dropping onto the table.
 */
#include "Sandpiles.h"
#include "GUI/SimpleTest.h"
using namespace std;

/**
 * Function: dropSandOn
 * Usage: dropSandOn(world, row, col)
 * --------------------------------------------------
 * Updates the world grid when dropping a sand on the position (row, col)
 * Example: row = 1, col = 1,
 *          world:
 *              0 0 0
 *              0 4 0
 *              0 0 0
 *          dropSandOn(world, row, col)
 *          world:
 *              0 1 0
 *              1 0 1
 *              0 1 0
 */
void dropSandOn(Grid<int>& world, int row, int col) {
    if (!world.inBounds(row, col)) {
        return;
    }

    if (world[row][col] < 3) {
        world[row][col]++;
    } else {
        world[row][col] = 0;
        dropSandOn(world, row - 1, col    );
        dropSandOn(world, row + 1, col    );
        dropSandOn(world, row    , col - 1);
        dropSandOn(world, row    , col + 1);
    }
}

/* * * * * * Provided Test Cases * * * * * */

PROVIDED_TEST("Dropping into an empty cell only changes that cell.") {
    /* Create a simple source grid. */
    Grid<int> before = {
        { 3, 3, 3 },
        { 3, 0, 3 },
        { 3, 3, 3 }
    };
    Grid<int> after = {
        { 3, 3, 3 },
        { 3, 1, 3 },
        { 3, 3, 3 }
    };

    dropSandOn(before, 1, 1);
    EXPECT_EQUAL(before, after); // The above call changes 'before.'
}

PROVIDED_TEST("Non-chaining topples work.") {
    /* Create a simple source grid. */
    Grid<int> before = {
        { 0, 0, 0 },
        { 1, 3, 1 },
        { 0, 2, 0 }
    };
    Grid<int> after = {
        { 0, 1, 0 },
        { 2, 0, 2 },
        { 0, 3, 0 }
    };

    dropSandOn(before, 1, 1);
    EXPECT_EQUAL(before, after); // The above call changes 'before.'
}

PROVIDED_TEST("Two topples chain.") {
    /* Create a simple source grid. */
    Grid<int> before = {
        { 0, 0, 0, 0 },
        { 0, 3, 3, 0 },
        { 0, 0, 0, 0 }
    };
    Grid<int> after = {
        { 0, 1, 1, 0 },
        { 1, 1, 0, 1 },
        { 0, 1, 1, 0 }
    };

    dropSandOn(before, 1, 1);
    EXPECT_EQUAL(before, after); // The above call changes 'before.'
}

/* * * * * * Student Test Cases * * * * * */

STUDENT_TEST("Super chaining case!!!") {
    Grid<int> before = {
        { 3, 3, 2},
        { 2, 4, 3},
        { 0, 2, 3}
    };
    Grid<int>after = {
        { 1, 3, 0},
        { 1, 0, 3},
        { 2, 1, 1}
    };
    dropSandOn(before, 1, 1);
    EXPECT_EQUAL(before, after);
}

STUDENT_TEST("Drops sand on the edge.") {
    Grid<int> before = {
        { 3, 0, 0},
        { 0, 0, 0},
        { 0, 0, 0}
    };
    Grid<int>after = {
        { 0, 1, 0},
        { 1, 0, 0},
        { 0, 0, 0}
    };
    dropSandOn(before, 0, 0);
    EXPECT_EQUAL(before, after);
}
