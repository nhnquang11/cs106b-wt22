#include "vector.h"
using namespace std;

/* We've provided this function to you; you don't need to implement it. */
bool isPerfectSquare(int value);

bool hasSquareSequenceRec(int n, Vector<int>& sequence,
                          int numVisited, Vector<bool>& isVisited) {
    /* Base case: All numbers have been in the sequence and validated. */
    if (numVisited == n) {
        return true;
    }

    /* Loop through all the elements from 1 to n. */
    for (int i = 1; i <= n; i++) {
        /* If the element is not in the sequence yet */
        if (!isVisited[i]) {
            /* and the sequence is empty or the sum of the element with the last element of
             * the sequence is a perfect square,
             */
            if (sequence.isEmpty() || isPerfectSquare(i + sequence[sequence.size() - 1])) {
                /* we consider adding this element to the sequence */
                sequence.add(i);
                /* and mark that element is already in the sequence. */
                isVisited[i] = true;
                /* Using recursion to check the remaining element. */
                if (hasSquareSequenceRec(n, sequence, numVisited + 1, isVisited)) {
                    return true;
                }
                /* Backtracking step: If the recursion fails, remove the element from the sequence
                 * and uncheck it from isVisited to be ready for next loop.
                 */
                sequence.remove(sequence.size() - 1);
                isVisited[i] = false;
            }
        }
    }
    return false;
}

bool hasSquareSequence(int n, Vector<int>& sequence) {
    /* Error-handling. */
    if (n <= 0) {
        error("The number of elements must be positive.");
    }

    /* A vector to check whether a number in range 1->n has been in the sequence. */
    Vector<bool> isVisited;
    for (int i = 0; i <= n; ++i) {
        isVisited.add(false);
    }
    return hasSquareSequenceRec(n, sequence, 0, isVisited);
}
