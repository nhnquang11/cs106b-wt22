#include "tokenscanner.h"
#include "string.h"

using namespace std;

int syllablesIn(string word);
bool isHaiku(string line1, string line2, string line3);
int syllablesInLine(string line);

bool isHaiku(string line1, string line2, string line3) {
    return syllablesInLine(line1) == 5 &&
           syllablesInLine(line2) == 7 &&
           syllablesInLine(line3) == 5;
}

int syllablesInLine(string line) {
    TokenScanner scanner(line);
    scanner.ignoreWhitespace();

    int numSyllables = 0;
    while(scanner.hasMoreTokens()) {
        string token = scanner.nextToken();
        if (scanner.getTokenType(token) == TokenScanner::TokenType::WORD) {
            numSyllables += syllablesIn(token);
        }
    }
    return numSyllables;
}
