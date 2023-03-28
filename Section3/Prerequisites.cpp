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
#include "testing/SimpleTest.h"
#include "testing/TextUtils.h"
#include "set.h"
#include "map.h"
using namespace std;

void listLegalOrderingsRec(const Map<string, Set<string>>& prereqs,
                           const Vector<string>& tasksPerformed,
                           const Set<string>& taskRemaining);

/*
 * Function: listLegalOrderingsOf
 * -------------------------------
 * Write a function that takes as input a Map representing the
 * prerequisite structure, then lists all possible orders in which
 * you could complete those tasks without violating the
 * prerequisites. The prereqs map is structured so that each key
 * is a task and each value is the set of that task’s immediate prerequisites.
 * See section handout for more details
 */
void listLegalOrderingsOf(const Map<string, Set<string>>& prereqs) {
    Set<string> courses;
    for (string course: prereqs) {
        courses.add(course);
    }
    listLegalOrderingsRec(prereqs, {}, courses);
}

void listLegalOrderingsRec(const Map<string, Set<string>>& prereqs,
                           const Vector<string>& coursesFinished,
                           const Set<string>& coursesRemaining) {
    if (coursesRemaining.isEmpty()) {
        cout << coursesFinished << endl;
    } else {
        Set<string> coursesFinishedSet;
        for (string course: coursesFinished) {
            coursesFinishedSet.add(course);
        }

        for (string course: coursesRemaining) {
            if (prereqs[course].isSubsetOf(coursesFinishedSet)) {
                Vector<string> newCourses = coursesFinished;
                newCourses.add(course);
                listLegalOrderingsRec(prereqs, newCourses, coursesRemaining - course);
            }
        }

    }
}

/* * * * * Provided Tests Below This Point * * * * */

/*
 * Since this is a void function, we don't have a return value to verify with.
 * We will instead just run your code to make sure there is no obvious error.
 */
PROVIDED_TEST("Provided Test: Run the provided example. There should be 16 entries.") {
    Map<string, Set<string>> prereqs = {
        {"CS103", { "CS106A" }},
        {"CS106A", { }},
        {"CS106B", { "CS106A" }},
        {"CS107", { "CS106B" }},
        {"CS109", { "CS103", "CS106B" }},
        {"CS110", { "CS107" }},
        {"CS161", { "CS109" }}
    };
    listLegalOrderingsOf(prereqs);
}
