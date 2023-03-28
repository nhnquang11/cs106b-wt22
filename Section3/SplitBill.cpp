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
#include "error.h"
using namespace std;

void listPossiblePaymentsRec(int total, const Set<string>& people, const Map<string, int>& payments);

/*
 * Function: listPossiblePayments
 * -------------------------------
 * Write a function that takes as input a total amount of money
 * to pay (in dollars) and a set of all the people who ordered
 * something, then lists off every possible way you could split
 * the bill, assuming everyone pays a whole number of dollars.
 */
void listPossiblePayments(int total, const Set<string>& people) {
    if (total < 0) {
        error("The total amount must be nonnegative.");
    }

    if (people.isEmpty()) {
        error("The list of people must not be empty.");
    }

    listPossiblePaymentsRec(total, people, {});
}

/*
 * Function: listPossiblePaymentsRec
 * -------------------------------
 * A helper function that lists off all ways that the set of people can pay a certain total amount,
 * assuming that some number of people have already commited to a given set of payments.
 */
void listPossiblePaymentsRec(int total, const Set<string>& people,
                             const Map<string, int>& payments) {
    /* Base case: If there is only one person left, they have to pay the whole bill. */
    if (people.size() == 1) {
        string takenOut = people.first();
        Map<string, int> finalPayments = payments;
        finalPayments[takenOut] = total;
        cout << endl << finalPayments;
    }
    /* Recursive case: The first person has to pay some amount between 0 and the total amount.
     * Try all of those possibilities.
     */
    else {
        string takenOut = people.first();
        for (int amount = total; amount >= 0; amount--) {
            Map<string, int> updatedPayments = payments;
            updatedPayments[takenOut] = amount;
            listPossiblePaymentsRec(total - amount, people - takenOut, updatedPayments);
        }
    }
}

/* * * * * Provided Tests Below This Point * * * * */

/*
 * Since this is a void function, we don't have a return value to verify with.
 * We will instead just run your code to make sure there is no obvious error.
 */
PROVIDED_TEST("Provided Test: Run the provided example. There should be 15 entries.") {
    Set<string> s = {"A", "B", "C"};
    listPossiblePayments(4, s);
}
