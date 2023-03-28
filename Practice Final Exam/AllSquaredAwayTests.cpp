#include "GUI/SimpleTest.h"
#include "vector.h"
#include "Demos/Timer.h"
#include <fstream>
using namespace std;

/* Checks if a number is a perfect square. Due to rounding issues and the like,
 * we've implemented this one for you.
 */
bool isPerfectSquare(int n) {
    /* Negative numbers cannot be perfect squares. */
    if (n < 0) return false;

    /* Get the square root, round it to the nearest integer, and square it. */
    int candidate = round(sqrt(double(n)));
    return candidate * candidate == n;
}

/* Prototype for the function you'll implement. */
bool hasSquareSequence(int n, Vector<int>& result);

namespace {
    /* Utility to compare a generated answer against a reference file. */
    void checkCorrectness(int n, bool exists, const Vector<int>& result) {
        ifstream input("res/AllSquaredAwayAnswers_" + to_string(n));
        if (!input) SHOW_ERROR("Internal error: Cannot open reference file.");

        int numRead = 0;
        bool found = false;
        for (Vector<int> seq; input >> seq; ) {
            numRead++;
            if (seq == result) {
                found = true;
                break;
            }
        }

        /* Case 1: File is empty. That means we should not have an answer. */
        if (numRead == 0) {
            if (exists) SHOW_ERROR("Your code found a solution, but no solution exists.");
        }
        /* Case 2: File not empty; we should have an answer. */
        else {
            if (!exists) SHOW_ERROR("Solution exists, but your code did not find it.");
            if (!found)  SHOW_ERROR("A solution exists, but your code produced a sequence that was not a solution.");
        }
    }
}

PROVIDED_TEST("Can order the sequence 1") {
    Vector<int> result;
    bool exists = hasSquareSequence(1, result);
    checkCorrectness(1, exists, result);
}

PROVIDED_TEST("Can order the sequence 1, 2, ..., 15") {
    Vector<int> result;
    bool exists = hasSquareSequence(15, result);
    checkCorrectness(15, exists, result);
}

PROVIDED_TEST("Cannot order the sequence 1, 2, ..., 19") {
    Vector<int> result;
    bool exists = hasSquareSequence(19, result);
    checkCorrectness(19, exists, result);
}

PROVIDED_TEST("Stress test: Can order the sequence 1, 2, ..., 30 (should take at most two seconds)") {
    Timing::Timer timer;

    Vector<int> result;
    timer.start();
    bool exists = hasSquareSequence(30, result);
    timer.stop();

    checkCorrectness(30, exists, result);
    EXPECT_LESS_THAN(timer.timeInSeconds(), 2); // Needs to be fast enough
}
