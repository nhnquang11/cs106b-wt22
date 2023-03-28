/*
 * CS106B Section Handout Test Harness: Section 2
 * ----------------------------------------------
 * These problems have been galvanized from years of
 * section handouts that have been worked on by numerous
 * instructors and TA's. Codified for Qt and Ed by Neel
 * Kishnani for Winter 2022.
 *
 * A huge thank you to Keith Schwarz and Julie Zelenski
 * for creating an amazing testing harness!
 */

#include "testing/SimpleTest.h"
#include "map.h"
#include <string>
using namespace std;

/**
 * Function: headOf
 * Usage: headOf(p, bosses)
 * --------------------------------------------------
 * Returns the CEO of the company where the person p works at.
 */
string headOf(const string& p, const Map<string, string>& bosses) {
    if (!bosses.containsKey(p)) {
        return p;
    }

    const string& superior = bosses[p];
    return headOf(superior, bosses);
}

/**
 * Function: areAtSameCompany
 * Usage: areAtSameCompany(p1, p2, bosses)
 * --------------------------------------------------
 * Checks whether person p1 and p2 work at the same company or not.
 */
bool areAtSameCompany(const string& p1,
                      const string& p2,
                      const Map<string, string>& bosses) {
    if (p1 == p2) {
        return true;
    }
    return headOf(p1, bosses) == headOf(p2, bosses);
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("Basic tests for areAtSameCompany") {
    Map<string, string> company1 = {
        { "Neel", "Keith" },
        { "Keith", "Mehran" },
        { "Mehran", "John" }, 
        { "John", "Jennifer" }, 
        { "Jennifer", "Persis" }
    };

    Map<string, string> company2 = {
        { "CJ", "Pierre" },
        { "Jack", "Stanley" },
        { "Stanley", "Lisa" },
        { "Lisa", "Janet" },
        { "Janet", "Poonam" }

    };

    EXPECT(areAtSameCompany("Neel", "Jennifer", company1));
    EXPECT(!areAtSameCompany("Neel", "Lisa", company1));
    EXPECT(!areAtSameCompany("CJ", "Keith", company2));
    EXPECT(!areAtSameCompany("Poonam", "Lisa", company1));

}
