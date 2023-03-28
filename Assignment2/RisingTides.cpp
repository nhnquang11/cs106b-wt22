#include "RisingTides.h"
#include "GUI/SimpleTest.h"
#include "queue.h"
using namespace std;

/* Function prototypes */
Grid<bool> floodedRegionsIn(const Grid<double>& terrain,
                            const Vector<GridLocation>& sources,
                            double height);
void floodedCell(int row, int col, const Grid<double>& terrain, double height,
                 Grid<bool>& flooded, Grid<bool>& checked, Queue<GridLocation>& frontier);

/**
 * Function: floodedRegionsIn
 * Usage: loodedRegionsIn(terrain, source, height)
 * --------------------------------------------------
 * Returns a grid marking all the flooded cells in the terrain
 * given the water source locations and their height.
 */
Grid<bool> floodedRegionsIn(const Grid<double>& terrain,
                            const Vector<GridLocation>& sources,
                            double height) {
    /* Keep track of flooded cells. */
    Grid<bool> flooded(terrain.numRows(), terrain.numCols(), false);
    /* Keep track of whether cells have been checked or not. */
    Grid<bool> checked(terrain.numRows(), terrain.numCols(), false);

    Queue<GridLocation> frontier;
    /* Loop through each water source. */
    for (const GridLocation& source: sources) {
        int row = source.row;
        int col = source.col;

        /* If the water source is at or below the water level,
         * flood that square and add that square to the queue */
        if (terrain[row][col] <= height) {
            flooded[row][col] = true;
            frontier.enqueue(source);
            checked[row][col] = true;
        }
    }

    /* Traverse through all the flooded cells to decide whether to flood their neighbors or not. */
    while (!frontier.isEmpty()) {
        const GridLocation& cell = frontier.dequeue();
        int row = cell.row;
        int col = cell.col;

        /* Check all the neighbors 1 step away in a cardinal direction. */
        floodedCell(row - 1, col    , terrain, height, flooded, checked, frontier);
        floodedCell(row + 1, col    , terrain, height, flooded, checked, frontier);
        floodedCell(row    , col - 1, terrain, height, flooded, checked, frontier);
        floodedCell(row    , col + 1, terrain, height, flooded, checked, frontier);
    }

    return flooded;
}

/**
 * Function: floodedCell
 * Usage: floodedCell(row, col, terrain, height, flooded, checked, frontier)
 * --------------------------------------------------
 * A helper function to process a single cell.
 */
void floodedCell(int row, int col, const Grid<double>& terrain, double height,
                 Grid<bool>& flooded, Grid<bool>& checked, Queue<GridLocation>& frontier) {
    /* Do nothing if the location is out of bound or has been checked before or is higher than the water source. */
    if (!terrain.inBounds(row, col) || checked[row][col] || terrain[row][col] > height) {
        return;
    }
    /* Flood the location and add the location to the frontier. */
    flooded[row][col] = true;
    frontier.enqueue(GridLocation(row, col));
    checked[row][col] = true;
}


/***** Test Cases Below This Point *****/
PROVIDED_TEST("Nothing gets wet if there are no water sources.") {
    Grid<double> world = {
        { 0, 0, 0 },
        { 0, 0, 0 },
        { 0, 0, 0 }
    };

    Vector<GridLocation> sources = {
        // empty
    };

    /* There are no water sources, so nothing should be underwater. */
    Grid<bool> water = floodedRegionsIn(world, sources, 1.0);
    Grid<bool> expected = {
        { false, false, false },
        { false, false, false },
        { false, false, false }
    };

    EXPECT_EQUAL(water, expected);
}

PROVIDED_TEST("Everything gets wet if all locations are below the water level.") {
    Grid<double> world = {
        { 0, 0, 0 },
        { 0, 0, 0 },
        { 0, 0, 0 }
    };

    Vector<GridLocation> sources = {
        { 0, 0 }
    };

    /* Everything should flood; there are no barriers to stop the water. */
    Grid<bool> water = floodedRegionsIn(world, sources, 1.0);
    Grid<bool> expected = {
        {  true,  true,  true },
        {  true,  true,  true },
        {  true,  true,  true }
    };

    EXPECT_EQUAL(water, expected);
}

PROVIDED_TEST("Water can't cross a vertical levee.") {
    Grid<double> world = {
        { 0, 2, 0 },
        { 0, 2, 0 },
        { 0, 2, 0 }
    };

    Vector<GridLocation> sources = {
        { 0, 0 }
    };

    /* Only locations to the left of the barrier should be under water. */
    Grid<bool> water = floodedRegionsIn(world, sources, 1.0);
    Grid<bool> expected = {
        {  true, false, false },
        {  true, false, false },
        {  true, false, false }
    };

    EXPECT_EQUAL(water, expected);
}

PROVIDED_TEST("Water can't cross a diagonal levee.") {
    Grid<double> world = {
        { 0, 0, 2 },
        { 0, 2, 0 },
        { 2, 0, 0 }
    };

    Vector<GridLocation> sources = {
        { 0, 0 }
    };

    /* Water only flows in the four cardinal directions, so it can't
     * pass through the barrier. Only the top should be flooded.
     */
    Grid<bool> water = floodedRegionsIn(world, sources, 1.0);
    Grid<bool> expected = {
        {  true,  true, false },
        {  true, false, false },
        { false, false, false }
    };

    EXPECT_EQUAL(water, expected);
}

PROVIDED_TEST("Water can't flow diagonally.") {
    Grid<double> world = {
        { 0, 2, 0 },
        { 2, 0, 2 },
        { 0, 2, 0 }
    };

    Vector<GridLocation> sources = {
        { 1, 1 }
    };

    /* Water should be trapped in the center, since it can't move
     * diagonally.
     */
    Grid<bool> water = floodedRegionsIn(world, sources, 1.0);
    Grid<bool> expected = {
        { false, false, false },
        { false,  true, false },
        { false, false, false }
    };

    EXPECT_EQUAL(water, expected);
}

PROVIDED_TEST("Water can flow in all cardinal directions.") {
    Grid<double> world = {
        { 2, 0, 2 },
        { 0, 0, 0 },
        { 2, 0, 2 }
    };

    Vector<GridLocation> sources = {
        { 1, 1 }
    };

    /* The water in this case should flow up, down, left, and right. */
    Grid<bool> water = floodedRegionsIn(world, sources, 1.0);
    Grid<bool> expected = {
        { false,  true, false },
        {  true,  true,  true },
        { false,  true, false }
    };

    EXPECT_EQUAL(water, expected);
}

PROVIDED_TEST("Water can flow from multiple sources.") {
    Grid<double> world = {
        { 0, 0, 2 },
        { 0, 2, 0 },
        { 2, 0, 0 }
    };

    Vector<GridLocation> sources = {
        { 0, 0 },
        { 2, 2 }
    };

    /* Everything except the levee should be under water. */
    Grid<bool> water = floodedRegionsIn(world, sources, 1.0);
    Grid<bool> expected = {
        {  true,  true, false },
        {  true, false,  true },
        { false,  true,  true }
    };

    EXPECT_EQUAL(water, expected);
}

PROVIDED_TEST("Handles asymmetric worlds and non-square grids") {
    Grid<double> world = {
        { 3, 1, 4, 1 },
        { 5, 9, 2, 6 },
        { 5, 3, 5, 8 }
    };

    /* Initial test - water shouldn't leak out from the 2 if the height is 3.5. */
    Vector<GridLocation> sources = {
        { 1, 2 }
    };

    Grid<bool> expected = {
        { false, false, false, false },
        { false, false,  true, false },
        { false, false, false, false },
    };

    EXPECT_EQUAL(floodedRegionsIn(world, sources, 3.5), expected);

    /* Now, increase the water height to 4.5. */
    expected = {
        {  true,  true,  true,  true },
        { false, false,  true, false },
        { false, false, false, false },
    };

    EXPECT_EQUAL(floodedRegionsIn(world, sources, 4.5), expected);

    /* Now, increase the water height to 5.5. */
    expected = {
        {  true,  true,  true,  true },
        {  true, false,  true, false },
        {  true,  true,  true, false },
    };

    EXPECT_EQUAL(floodedRegionsIn(world, sources, 5.5), expected);

    /* Now, increase the water height to 6.5. */
    expected = {
        {  true,  true,  true,  true },
        {  true, false,  true,  true },
        {  true,  true,  true, false },
    };

    EXPECT_EQUAL(floodedRegionsIn(world, sources, 6.5), expected);

    /* Now, increase the water height to 9.5. */
    expected = {
        {  true,  true,  true,  true },
        {  true,  true,  true,  true },
        {  true,  true,  true,  true },
    };

    EXPECT_EQUAL(floodedRegionsIn(world, sources, 9.5), expected);
}

PROVIDED_TEST("Stress test: Handles a large, empty world quickly.") {
    Grid<double> world(100, 100); // Large world, everything defaults to 0 height.
    Vector<GridLocation> sources = {
        { 0, 0 }
    };

    /* This may take a long time to complete if the solution is inefficient. Look
     * for things like
     *
     * 1. passing around large objects by *value* rather than by *reference*,
     * 2. revisiting the same squares multiple times (e.g. flooding the same
     *    cell many times due to not checking if something is flooded),
     *
     * etc.
     */
    Grid<bool> water = floodedRegionsIn(world, sources, 1.0);
    EXPECT_EQUAL(water.numRows(), world.numRows());
    EXPECT_EQUAL(water.numCols(), world.numCols());

    /* Everything should be flooded. */
    for (int row = 0; row < world.numRows(); row++) {
        for (int col = 0; col < world.numCols(); col++) {
            EXPECT_EQUAL(water[row][col], true);
        }
    }
}

/***** Student test case *****/
STUDENT_TEST("Water flows from multiple sources.") {
    Grid<double> world = {
        { 3, 5, 5, 1 },
        { 5, 3, 2, 5 },
        { 5, 3, 4, 5 },
        { 5, 3, 5, 4 }
    };

    Vector<GridLocation> sources = {
        { 1, 2 }, { 3, 3 }
    };

    Grid<bool> expected = {
        { false, false, false, false },
        { false, true, true, false },
        { false, true, true, false },
        { false, true, false, true }
    };

    EXPECT_EQUAL(floodedRegionsIn(world, sources, 4), expected);
}
