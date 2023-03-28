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
#include <string>
#include "testing/SimpleTest.h"
#include "testing/TextUtils.h"
#include "set.h"
#include <cctype>
using namespace std;

/*
 * Function: isElementSpellable
 * -----------------------------
 * Write a function that accepts as input a string and a Set<string>
 * containing all element symbols (stored with the proper
 * capitalization), then returns whether that string can be
 * written using only element symbols.
 */
bool isElementSpellableRec(const string& text, int index, const Set<string>& symbols, string& result) {
    if (index == text.size()) return true;

    /* A symbol that contains only one charater. */
    char firstChar = toupper(text[index]);
    string oneChar(1, firstChar);
    bool oneCharRes = false;
    if (symbols.contains(oneChar)) {
        oneCharRes = isElementSpellableRec(text, index + 1, symbols, result);
    }
    if (oneCharRes) result = oneChar + result;

    /* A symbol that contains two characters. */
    bool twoCharsRes = false;
    if (index < text.size() - 1) {
        char secondChar = tolower(text[index+1]);
        string twoChars = oneChar;
        twoChars += secondChar;
        if (symbols.contains(twoChars)) {
            twoCharsRes = isElementSpellableRec(text, index + 2, symbols, result);
        }
        /* If the result of the one-character symbol is false and
         * the result of the two-character symbol is true,
         * then use the word constructed from two-character recursion. */
        if (twoCharsRes && !oneCharRes) result = twoChars + result;
    }
    return oneCharRes || twoCharsRes;
}

bool isElementSpellable(const string& text, const Set<string>& symbols) {
    string res;
    return isElementSpellableRec(text, 0, symbols, res);
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("Provided Test: Verify that the solution works on examples from the handout.") {
  Set<string> symbols = {
    "H", "He", "Li", "Be", "B",
    "C", "N", "O", "F", "Ne", "Na",
    "Mg", "Al", "Si", "P", "S", "Cl",
    "Ar", "K", "Ca", "Sc", "Ti", "V",
    "Cr", "Mn", "Fe", "Co", "Ni",
    "Cu", "Zn", "Ga", "Ge", "As",
    "Se", "Br", "Kr", "Rb", "Sr",
    "Y", "Zr", "Nb", "Mo", "Tc",
    "Ru", "Rh", "Pd", "Ag", "Cd",
    "In", "Sn", "Sb", "Te", "I",
    "Xe", "Cs", "Ba", "La", "Ce",
    "Pr", "Nd", "Pm", "Sm", "Eu",
    "Gd", "Tb", "Dy", "Ho", "Er",
    "Tm", "Yb", "Lu", "Hf", "Ta",
    "W", "Re", "Os", "Ir", "Pt",
    "Au", "Hg", "Tl", "Pb",
    "Bi", "Po", "At", "Rn", "Fr",
    "Ra", "Ac", "Th", "Pa", "U",
    "Np", "Pu", "Am", "Cm", "Bk",
    "Cf", "Es", "Fm", "Md", "No",
    "Lr", "Rf", "Db", "Sg", "Bh",
    "Hs", "Mt", "Ds", "Rg", "Cn",
    "Nh", "Fl", "Mc", "Lv", "Ts",
    "Og"
  };
  EXPECT_EQUAL(isElementSpellable("feline", symbols), true);
  EXPECT_EQUAL(isElementSpellable("interesting", symbols), false);
  EXPECT_EQUAL(isElementSpellable("CHeMoWIZrDy", symbols), true);
}
