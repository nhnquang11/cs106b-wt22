#include "Labyrinth.h"
using namespace std;

/**
 * Function: check
 * --------------------------------------------------
 * Check the current cell and update the corresponding number of items if any are found.
 */
void check(MazeCell* cell, int& numSpells, int& numPotions, int& numWands) {
    switch (cell->whatsHere) {
    case Item::SPELLBOOK:
        ++numSpells;
        break;
    case Item::POTION:
        ++numPotions;
        break;
    case Item::WAND:
        ++numWands;
        break;
    }
}

/**
 * Function: move
 * --------------------------------------------------
 * Given the direction, return a pointer to a cell
 * that is moved from the current cell in the given direction.
 */
MazeCell* move(MazeCell* cell, char direction) {
    switch (direction) {
    case 'N':
        return cell->north;
    case 'S':
        return cell->south;
    case 'E':
        return cell->east;
    case 'W':
        return cell->west;
    default:
        return nullptr;
    }
}

/**
 * Function: move
 * --------------------------------------------------
 * Given the start cell in the labyrinth and a string of instructions,
 * check whether all the moves are valid and if there are three magical
 * items to free from the walls.
 */
bool isPathToFreedom(MazeCell* start, const string& moves) {
    int numSpells = 0;
    int numPotions = 0;
    int numWands = 0;

    check(start, numSpells, numPotions, numWands);
    for (char direction: moves) {
        start = move(start, direction);
        if  (start == nullptr) return false;
        check(start, numSpells, numPotions, numWands);
    }
    return numSpells >= 1 && numPotions >= 1 && numWands >= 1;
}

/* * * * * * Test Cases Below This Point * * * * * */
#include "GUI/SimpleTest.h"
#include "Demos/MazeGenerator.h"

/* * * * * Provided Tests Below This Point * * * * */

/* Utility function to free all memory allocated for a maze. */
void deleteMaze(const Grid<MazeCell*>& maze) {
    for (auto* elem: maze) {
        delete elem;
    }
    /* Good question to ponder: why don't we write 'delete maze;'
     * rather than what's shown above?
     */
}

PROVIDED_TEST("Checks paths in the sample maze.") {
    auto maze = toMaze({"* *-W *",
                        "| |   |",
                        "*-* * *",
                        "  | | |",
                        "S *-*-*",
                        "|   | |",
                        "*-*-* P"});

    /* These paths are the ones in the handout. They all work. */
    EXPECT(isPathToFreedom(maze[2][2], "ESNWWNNEWSSESWWN"));
    EXPECT(isPathToFreedom(maze[2][2], "SWWNSEENWNNEWSSEES"));
    EXPECT(isPathToFreedom(maze[2][2], "WNNEWSSESWWNSEENES"));

    /* These paths don't work, since they don't pick up all items. */
    EXPECT(!isPathToFreedom(maze[2][2], "ESNW"));
    EXPECT(!isPathToFreedom(maze[2][2], "SWWN"));
    EXPECT(!isPathToFreedom(maze[2][2], "WNNE"));

    /* These paths don't work, since they aren't legal paths. */
    EXPECT(!isPathToFreedom(maze[2][2], "WW"));
    EXPECT(!isPathToFreedom(maze[2][2], "NN"));
    EXPECT(!isPathToFreedom(maze[2][2], "EE"));
    EXPECT(!isPathToFreedom(maze[2][2], "SS"));

    deleteMaze(maze);
}

PROVIDED_TEST("Can't walk through walls.") {
    auto maze = toMaze({"* S *",
                        "     ",
                        "W * P",
                        "     ",
                        "* * *"});

    EXPECT(!isPathToFreedom(maze[1][1], "WNEES"));
    EXPECT(!isPathToFreedom(maze[1][1], "NWSEE"));
    EXPECT(!isPathToFreedom(maze[1][1], "ENWWS"));
    EXPECT(!isPathToFreedom(maze[1][1], "SWNNEES"));

    deleteMaze(maze);
}

PROVIDED_TEST("Works when starting on an item.") {
    auto maze = toMaze({"P-S-W"});

    EXPECT(isPathToFreedom(maze[0][0], "EE"));
    EXPECT(isPathToFreedom(maze[0][1], "WEE"));
    EXPECT(isPathToFreedom(maze[0][2], "WW"));

    deleteMaze(maze);
}
