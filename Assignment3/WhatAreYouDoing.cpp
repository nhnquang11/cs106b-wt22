#include "WhatAreYouDoing.h"
#include "strlib.h"
#include "GUI/SimpleTest.h"
using namespace std;

/* Function prototypes */
Set<string> allEmphasesOf(const string& sentence);
void generate(Set<string>& allEmphases, Vector<string>& tokens, Vector<int>& wordIndices, int i);

/**
 * Function: allEmphasesOf
 * Usage: allEmphasesOf(sentence)
 * --------------------------------------------------
 * Returns all possible ways to emphasize the given sentence.
 */
Set<string> allEmphasesOf(const string& sentence) {
    Set<string> allEmphases;
    Vector<string> tokens = tokenize(sentence);
    Vector<int> wordIndices;  // stores the indices of alpha tokens
    for (int i = 0; i < tokens.size(); i++) {
        if (isalpha(tokens[i][0])) {
            wordIndices.add(i);
        }
    }
    generate(allEmphases, tokens, wordIndices, 0);
    return allEmphases;
}

/**
 * Function: generate
 * Usage: generate(allEmphases, tokens, wordIndices, i)
 * --------------------------------------------------
 * Given a vector of tokens, a vector of indices marking alpha tokens' position and
 * the index to be considered being emphasized, generates all possible ways to emphasize
 * a sentence constructed from tokens.
 */
void generate(Set<string>& allEmphases, Vector<string>& tokens, Vector<int>& wordIndices, int i) {
    /* Add a new sentence constructed from tokens to allEmphases
     * if there are no more tokens to be considered. */
    if (i == wordIndices.size()) {
       string str = "";
       for (string s: tokens) {
           str += s;
       }
       allEmphases.add(str);
    } else {
        /* Option 1: Decide not to emphasize the current token and move to the next. */
        tokens[wordIndices[i]] = toLowerCase(tokens[wordIndices[i]]);
        generate(allEmphases, tokens, wordIndices, i+1);

        /* Option 2: Emphasize the current token and move to the next. */
        tokens[wordIndices[i]] = toUpperCase(tokens[wordIndices[i]]);
        generate(allEmphases, tokens, wordIndices, i+1);
    }
}

/* * * * * * Test Cases * * * * * */
#include "GUI/SimpleTest.h"

STUDENT_TEST("Non-alpha sentences.") {
    Set<string> expected = {
        "!@#$%^&*()"
    };

    EXPECT_EQUAL(allEmphasesOf("!@#$%^&*()"), expected);
}

STUDENT_TEST("Trivial case.") {
    string randomSentence = "I became paranoid that the school of jellyfish was spying on me.";
    EXPECT_EQUAL(allEmphasesOf(randomSentence).size(), 4096);
}

/* * * * * * Test cases from the starter files below this point. * * * * * */

PROVIDED_TEST("Enumerates all options in a simple case.") {
    Set<string> expected = {
        "hello",
        "HELLO",
    };

    EXPECT_EQUAL(allEmphasesOf("Hello"), expected);
}

PROVIDED_TEST("Each option has the right length.") {
    string sentence = "Hello, world!";
    for (string option: allEmphasesOf(sentence)) {
        EXPECT_EQUAL(option.length(), sentence.length());
    }
}

PROVIDED_TEST("Enumerates all options in a more typical case.") {
    Set<string> expected = {
        "you are?",
        "you ARE?",
        "YOU are?",
        "YOU ARE?"
    };

    EXPECT_EQUAL(allEmphasesOf("You Are?"), expected);
}

PROVIDED_TEST("Stress Test: Recursion only branches on words (should take under a second)") {
    /* We're producing a string consisting of fifty copies of the * character. This tokenizes
     * into fifty separate stars. A star is the same whether or not you capitalize it - there
     * is no such thing as an "upper-case" or "lower-case" star. Therefore, your code should
     * not try to form two versions of the sentence, one with the star capitalized and one
     * without, because the two versions will end up being the same and the work to compute
     * both options will dramatically increase the runtime.
     *
     * For reference, if you do try branching the recursion and checking what happens both if
     * you capitalize the star and if you don't, you'll try exploring 2^50 different possible
     * capitalizations. That's 1,125,899,906,842,624 options, and even doing a billion of
     * these a second is going to take over two years to generate them all! And of course,
     * that's all wasted work, since there's only one way to capitalize this sentence, and
     * that's just to leave it as-is.
     *
     * If your code is hanging when this test is running, it likely means that your code is
     * trying to enumerate all of these options. See if you can edit your code so that, if
     * you're given a non-word token, you just leave it as-is and don't change anything.
     */
    string punctuation(50, '*'); // 50 copies of *

    /* The only emphasis is itself. */
    Set<string> expected = {
        punctuation
    };

    EXPECT_EQUAL(allEmphasesOf(punctuation), expected);
}

PROVIDED_TEST("Stress test: Generates each option once (should take at most a few seconds)") {
    /* This sentence has 13 words in it. There are therefore 2^13 = 8192 possible emphases for
     * the words there, which is a big number but not so large that the computer can't handle
     * it if generates each emphasis exactly once.
     *
     * On the other hand, if your code tries to generate the same emphases multiple times,
     * this test may take a very, very long time to complete, and might even appear to freeze
     * up.
     *
     * If your code gets stuck in this test, trace through your code and confirm that you can't
     * produce the same emphasis multiple times. Check to make sure you aren't, for example,
     * looping over every token in the input sentence and deciding which one to capitalize
     * next. The recursion here follows more of an include/exclude type pattern (more like
     * subsets and combinations) than it does a "which is next?" type pattern (more like
     * permutations).
     */
    string yeats = "Turing and turning in a widening gyre / the falcon cannot hear the falconer.";
    EXPECT_EQUAL(allEmphasesOf(yeats).size(), 8192);
}

PROVIDED_TEST("Sentence including alpha and non-alpha tokens.") {
    Set<string> expected = {
        "number 123",
        "NUMBER 123"
    };

    EXPECT_EQUAL(allEmphasesOf("nUmBeR 123"), expected);
}
