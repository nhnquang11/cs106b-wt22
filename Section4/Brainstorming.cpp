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

bool matches(const string& text, const string& pattern);
bool matchesRec(const string& text, const string& pattern, int textIndex, int patternIndex);

/*
 * Function: matches
 * ------------------
 * Your task is to write a function that takes as input a
 * string and a pattern, then returns whether that string
 * matches the pattern.
 */
bool matches(const string& text, const string& pattern) {
    return matchesRec(text, pattern, 0, 0);
}


/*
 * Function: matchesRec
 * ------------------
 * A helper function using recursion to check whether the given string
 * matches the regular expression pattern.
 */
bool matchesRec(const string& text, const string& pattern, int textIndex, int patternIndex) {
    if (patternIndex == pattern.size()) {
        return textIndex == text.size();
    }

    if (textIndex == text.size()) {
        string rest = pattern.substr(patternIndex);
        for (char c: rest) {
            if (!(c == '*' || c == '?')) {
                return false;
            }
        }
        return true;
    }

    switch (pattern[patternIndex]) {
        case '*':
            return matchesRec(text, pattern, textIndex + 1, patternIndex) ||
                   matchesRec(text, pattern, textIndex, patternIndex + 1);
        case '.':
            return matchesRec(text, pattern, textIndex + 1, patternIndex + 1);
        case '?':
            return matchesRec(text, pattern, textIndex + 1, patternIndex + 1) ||
                   matchesRec(text, pattern, textIndex, patternIndex + 1);
        default:
            return text[textIndex] == pattern[patternIndex] &&
                   matchesRec(text, pattern, textIndex + 1, patternIndex + 1);
    }
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("Provided Test: Testing *") {
    EXPECT_EQUAL(matches("apple", "a*"), true);
    EXPECT_EQUAL(matches("apply", "a*"), true);
    EXPECT_EQUAL(matches("apoplexy", "a*"), true);
    EXPECT_EQUAL(matches("Amicus", "a*"), false);
    EXPECT_EQUAL(matches("banana", "a*"), false);
}

PROVIDED_TEST("Provided Test: Testing .") {
    EXPECT_EQUAL(matches("three", "th..."), true);
    EXPECT_EQUAL(matches("threshold", "th..."), false);
}

PROVIDED_TEST("Provided Test: Testing ?") {
    EXPECT_EQUAL(matches("color", "colo?r"), true);
    EXPECT_EQUAL(matches("colour", "colo?r"), true);
    EXPECT_EQUAL(matches("colouxr", "colo?r"), false);
}
