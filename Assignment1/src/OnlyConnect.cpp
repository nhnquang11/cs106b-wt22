/* File: OnlyConnect.cpp
 * --------------------
 * This program forms a puzzle string from a given phrase where
 * all the characters except consonants have been deleted.
 * For example, the result of "CTCHMFYCN" is "Catch Me If You Can".
 */
#include "OnlyConnect.h"
#include "GUI/SimpleTest.h"
#include "strlib.h"
#include <cctype>
using namespace std;

/* Function prototypes */
string onlyConnectize(string phrase);
bool isConsonant(char c);

/**
 * Function: onlyConnectize
 * Usage: onlyConnectize(phrase)
 * --------------------------------------------------
 * Removes all characters but consonants from the given phrase
 * and returns the upper-case version of the result.
 */
string onlyConnectize(string phrase) {
    if (phrase.length() == 0) return "";

    char first = toUpperCase(phrase[0]);
    string allButFirst = phrase.substr(1);
    if (isConsonant(first)) {
        return first + onlyConnectize(allButFirst);
    } else {
        return onlyConnectize(allButFirst);
    }
}

/**
 * Function: isConsonant
 * Usage: isConsonant(char)
 * --------------------------------------------------
 * Checks whether the given char is a consonant or not.
 */
bool isConsonant(char c) {
    static string vowels = "aeiou";
    c = toLowerCase(c);
    return isalpha(c) && (vowels.find(c) == string::npos);
}

/* * * * * * Provided Test Cases * * * * * */

PROVIDED_TEST("Converts lower-case to upper-case.") {
    EXPECT_EQUAL(onlyConnectize("lowercase"), "LWRCS");
    EXPECT_EQUAL(onlyConnectize("uppercase"), "PPRCS");
}

PROVIDED_TEST("Handles non-letter characters properly.") {
    EXPECT_EQUAL(onlyConnectize("2.718281828459045"), "");
    EXPECT_EQUAL(onlyConnectize("'Hi, Mom!'"), "HMM");
}

PROVIDED_TEST("Handles single-character inputs.") {
    EXPECT_EQUAL(onlyConnectize("A"), "");
    EXPECT_EQUAL(onlyConnectize("+"), "");
    EXPECT_EQUAL(onlyConnectize("Q"), "Q");
}

/* * * * * * Student Test Cases * * * * * */

STUDENT_TEST("Handels inputs consisting of special characters.") {
    EXPECT_EQUAL(onlyConnectize("H!e@l#l$o%, w^o&r*l(d)!"), "HLLWRLD");
}
