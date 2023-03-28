/*
 * CS106B Section Handout Test Harness: Section 8
 * ----------------------------------------------
 * These problems have been galvanized from years of
 * section handouts that have been worked on by numerous
 * instructors and TA's. Codified by Neel Kishnani for 
 * CS106B Winter 2022.
 *
 * A huge thank you to Keith Schwarz and Julie Zelenski
 * for creating an amazing testing harness!
 */

#include <iostream>
#include "testing/SimpleTest.h"
#include "node.h"
#include "utility.h"
#include "error.h"
#include "set.h"
using namespace std;

int removeLargestFrom(Node*& root);
void performDeletion(Node*& toRemove);

/*
 * Function: removeFrom
 * ---------------------
 * Given the root of a tree and a key to remove,
 * removes the key in-place.
 */
void removeFrom(Node*& root, int value) {
    /* If the tree is empty, there’s nothing to remove! */
    if (root == nullptr) return;
    /* If the node to delete is to the left, remove it from there. */
    else if (value < root->data) {
        removeFrom(root->left, value);
    }
    /* If the node to delete is to the right, remove from there. */
    else if (value > root->data) {
        removeFrom(root->right, value);
    }
    /* Otherwise, we’ve found the node to remove – so go remove it! */
    else {
        performDeletion(root);
    }
}
/* Actually does the deletion necessary to remove a node from the tree. */
void performDeletion(Node*& toRemove) {
    /* Case 1: The node is a leaf. Then we just delete it. */
    if (toRemove->left == nullptr && toRemove->right == nullptr) {
        delete toRemove;
        /* Inform whoever was pointing at us that we no longer exist. */
        toRemove = nullptr;
    }
    /* Case 2a: Only have a left child. */
    else if (toRemove->right == nullptr) {
        Node* replacement = toRemove->left;
        delete toRemove;
        toRemove = replacement;
    }
    /* Case 2b: Only have a right child. */
    else if (toRemove->left == nullptr) {
        Node* replacement = toRemove->right;
        delete toRemove;
        toRemove = replacement;
    }
    /* Case 3: Replace this node with the largest node in its left subtree. */
    else {
        toRemove->data = removeLargestFrom(toRemove->left);
    }
}

/* Deletes the largest value from the specified tree, returning that value. */
int removeLargestFrom(Node*& root) {
  if (root->right == nullptr) {
    int result = root->data;
    performDeletion(root);
    return result;
  }
  return removeLargestFrom(root->right);
}

PROVIDED_TEST("Simple tests for remove function") {
    Node* tree = createTreeFromVector({/* Level 1*/ 5, /* Level 2*/ 3, EMPTY});
    Node* modified = createTreeFromVector({ 5 });
    removeFrom(tree, 3);
    EXPECT(treeEqual(tree, modified));
    freeTree(tree);
    freeTree(modified);
}
