/*
 * CS106B Section Handout Test Harness: Section 3
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
#include <string>
#include "testing/SimpleTest.h"
#include "testing/TextUtils.h"
#include "set.h"
using namespace std;

int fewestCoinsForMemoized(int cents, const Set<int>& coins, Map<int, int>& memoizedResults);

/*
 * Function: fewestCoinsFor
 * -------------------------
 * Write a function that takes as input a number of cents and a Set<int>
 * indicating the different denominations of coins used in a country,
 * then returns the minimum number of coins required to make change
 * for that total. In the case of US coins, this should always return
 * the same number as the greedy approach, but in general it might return
 * a lot fewer!
 */
int fewestCoinsFor(int cents, const Set<int>& coins) {
    Map<int, int> memoizedResults;
    return fewestCoinsForMemoized(cents, coins, memoizedResults);
}

/*
 * Function: fewestCoinsForMemoized
 * -------------------------
 * A helper function using memoization to speed up the calculating process.
 */
int fewestCoinsForMemoized(int cents, const Set<int>& coins, Map<int, int>& memoizedResults) {
    if (cents < 0) {
        return 1e9;
    } else if (cents == 0) {
        return 0;
    }
    if (coins.isEmpty()) {
        return 1e9;
    }

    if (memoizedResults.containsKey(cents)) {
        return memoizedResults[cents];
    } else {
        int takenOut = coins.first();
        int numUsingTakenOut = 1 + fewestCoinsForMemoized(cents - takenOut, coins, memoizedResults);
        int numWithoutTakenOut = fewestCoinsForMemoized(cents, coins - takenOut, memoizedResults);
        int minNumCoins = min(numUsingTakenOut, numWithoutTakenOut);
        memoizedResults[cents] = minNumCoins;
        return minNumCoins;
    }
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("Provided Test: Verify that the solution works on a simple example and the provided example") {
    Set<int> coins = {50, 25, 10, 5, 1};
    EXPECT_EQUAL(fewestCoinsFor(12, coins), 3);
    // You might want to uncomment this after implementing memoization.
    EXPECT_EQUAL(fewestCoinsFor(97, coins), 6);
}
