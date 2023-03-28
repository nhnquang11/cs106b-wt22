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
#include "lexicon.h"
#include <string>
using namespace std;

string mostXzibitWord(const Lexicon& words) {
    /* Track the best string we've seen so far and how many subwords it has. */
    string result;
    int numSubwords = 0;

    for (string word: words) {
        /* Store all the subwords we find. To avoid double-counting words, we'll hold this in a lexicon.*/
        Lexicon subwords;

        /* Loop through all possible subwords of word. */
        for (int start = 0; start < word.length(); start++) {
            for (int stop = start; stop <= word.length(); stop++) {
                string candidate = word.substr(start, stop - start);

                /* If this is a word, then record it as a subword. */
                if (words.contains(candidate)) {
                    subwords.add(candidate);
                }
            }
        }

        /* Having found all subwords, see if this is better than our best guess so far. */
        if (numSubwords < subwords.size()) {
            numSubwords = subwords.size();
            result = word;
        }
    }
    return result;
}

/* * * * * Provided Tests Below This Point * * * * */

static Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

PROVIDED_TEST("Example from handout") {
    Lexicon& lex = sharedLexicon();
    EXPECT_EQUAL(mostXzibitWord(lex), "foreshadowers");
}
