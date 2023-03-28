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
#include "set.h"
#include "map.h"
using namespace std;

bool canVisitAllSites(const Vector<GPoint>& sites, double travelTimeAvailable);
bool canVisitAllSitesRec(const Vector<GPoint>& sites, double travelTimeAvailable,
                         Vector<bool>& visited, int numSites, double travelTimeSoFar,
                         const GPoint& lastSite);
double distance(const GPoint& p1, const GPoint& p2);

/*
 * Function: canVisitAllSites
 * ---------------------------
 * Write a function that takes as input a list of all the
 * sites you’d like to visit and an amount of free time
 * available to you and returns whether it’s possible to
 * visit all those sites in the allotted time (assume you’ve
 * already factored inthe cost of speaking at each site
 * and that you’re just concerned about the travel time.)
 */
bool canVisitAllSites(const Vector<GPoint>& sites, double travelTimeAvailable) {
    Vector<bool> visited;
    for (int i = 0; i < sites.size(); i++) {
        visited.add(false);
    }
    visited[0] = true;
    return canVisitAllSitesRec(sites, travelTimeAvailable, visited, 1, 0, sites[0]);
}

/*
 * Function: canVisitAllSitesRec
 * ---------------------------
 * A helper function using recursion and backtracking.
 *
 * @param  sites:                list of all sites you would like to visit
 * @param  travelTimeAvailable:  an amount of free time available to you
 * @param  visited:              a boolean vector to indicate whether a specific site has been visited or not
 * @param  numSites:             number of sites visited so far
 * @param  travelTimeSoFar:      an amount of time so far to travel through numSites
 * @param  lastSite:             the last site you have visited
 * @return boolean:              whether it's possible to visit all those sites in the allotted time
 */
bool canVisitAllSitesRec(const Vector<GPoint>& sites, double travelTimeAvailable,
                         Vector<bool>& visited, int numSites, double travelTimeSoFar,
                         const GPoint& lastSite) {
    if (numSites == sites.size()) {
        return travelTimeSoFar <= travelTimeAvailable;
    }

    if (travelTimeSoFar > travelTimeAvailable) {
        return false;
    }

    for (int i = 0; i < sites.size(); i++) {
        if (!visited[i]) {
            double travelTime = distance(sites[i], lastSite);
            visited[i] = true;
            if (canVisitAllSitesRec(sites, travelTimeAvailable, visited, numSites + 1,
                                    travelTimeSoFar + travelTime, sites[i])) {
                return true;
            }
            visited[i] = false;
        }
    }

    return false;
}

/*
 * Function: distance
 * ---------------------------
 * Calculates the Euclidean distance between p1 and p2.
 */
double distance(const GPoint& p1, const GPoint& p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("Provided Test: Simple examples!") {
    Vector<GPoint> sites1 = {{0, 0}, {5, 5}};
    Vector<GPoint> sites2 = {{0, 0}, {2, 2}, {5, 5}};
    EXPECT_EQUAL(canVisitAllSites(sites1, 8), true);
    EXPECT_EQUAL(canVisitAllSites(sites2, 8), true);
}
