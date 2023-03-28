#include "Huffman.h"
#include "map.h"
#include "priorityqueue.h"
using namespace std;

/**
 * Deallocates all nodes in a Huffman tree. We've provided this helper function
 * to you since we also use it in our test driver and figured you might want
 * to make use of it.
 */
void deleteTree(EncodingTreeNode* tree) {
    if (tree != nullptr) {
        deleteTree(tree->zero);
        deleteTree(tree->one);
        delete tree;
    }
}

/**
 * Constructs a Huffman coding tree for the given string, using the algorithm
 * described in lecture.
 *
 * If the input string does not contain at least two different characters,
 * this function should report an error.
 *
 * When assembling larger trees out of smaller ones, make sure to set the first
 * tree dequeued from the queue to be the zero subtree of the new tree and the
 * second tree as the one subtree.
 */
EncodingTreeNode* huffmanTreeFor(const string& str) {
    /* A map storing the frequencies of different characters in the input string. */
    Map<char, int> freq;
    for (char c: str) {
        ++freq[c];
    }

    /* Report an error on invalid string input. */
    if (freq.isEmpty() || freq.size() == 1) {
        error("The input string must have at least two different characters.");
    }

    /* Enqueue all the different characters with their frequencies as priority values to the pq. */
    PriorityQueue<EncodingTreeNode*> pq;
    for (char c: freq) {
        EncodingTreeNode* node = new EncodingTreeNode;
        node->ch = c;
        node->one = node->zero = nullptr;
        pq.enqueue(node, freq[c]);
    }

    /* Building a Huffman coding tree. */
    while (pq.size() != 1) {
        /* Dequeue the first node. */
        int firstNodePriority = pq.peekPriority();
        EncodingTreeNode* firstNode = pq.dequeue();

        /* Dequeue the second node. */
        int secondNodePriority = pq.peekPriority();
        EncodingTreeNode* secondNode = pq.dequeue();

        /* Create a new node with the 0 child is the first node and the 1 child is the second node. */
        EncodingTreeNode* newNode = new EncodingTreeNode;
        newNode->zero = firstNode;
        newNode->one = secondNode;

        /* Enqueue a newly constructed node to the pq with the priority is the sum of its children's priorities. */
        int newPriority = firstNodePriority + secondNodePriority;
        pq.enqueue(newNode, newPriority);
    }

    /* Dequeue the pointer to the top of the Huffman coding tree which is the only one left from the pq. */
    EncodingTreeNode* head = pq.dequeue();
    return head;
}

/**
 * Given a Queue<Bit> containing a compressed message and a tree that was used
 * to encode those bits, decodes the bits back to the original message.
 *
 * You can assume the input tree is not null and is not a single character;
 * those are edge cases you don't need to handle.
 *
 * You can assume the input Queue<Bit> is well-formed in that every character
 * was encoded correctly, there are no stray bits in the Queue, etc.
 */
string decodeText(Queue<Bit>& bits, EncodingTreeNode* tree) {
    string text = "";
    EncodingTreeNode* ptr = tree;

    while (!bits.isEmpty()) {
        Bit bit = bits.dequeue();
        if (bit == 0) {
            ptr = ptr->zero;
        } else {
            ptr = ptr->one;
        }

        /* If the current position is a leaf node, add the character to the result string
         * and move the pointer back to the top to be ready for the next character. */
        if (ptr->zero == nullptr && ptr->one == nullptr) {
            text += ptr->ch;
            ptr = tree;
        }
    }
    return text;
}

/**
 * Given a Huffman coding tree, build a table mapping from chars to their
 * encoded bits sequence.
 */
void buildTable(EncodingTreeNode* tree, Map<char, string>& encode,
                const string& code) {
    if (tree->zero == nullptr && tree->one == nullptr) {
        encode[tree->ch] = code;
        return;
    }
    buildTable(tree->zero, encode, code + "0");
    buildTable(tree->one, encode, code + "1");
}

/**
 * Enqueue all bits from a tring to the bits queue.
 *
 * >>> bits = {}
 * >>> bitsSeq = "110011"
 * >>> enqueueBits(bitsSeq, bits)
 * >>> bits
 * {1, 1, 0, 0, 1, 1}
 */
void enqueueBits(const string& bitsSeq, Queue<Bit>& bits) {
    for (char bit: bitsSeq) {
        if (bit == '0') {
            bits.enqueue(0);
        } else if (bit == '1') {
            bits.enqueue(1);
        } else {
            error("Invalid bit!");
        }
    }
}

/**
 * Given a string and a Huffman encoding tree, encodes that text using the tree
 * and outputs a Queue<Bit> corresponding to the encoded representation.
 *
 * The input tree will not be null and will not consist of a single node; these
 * are edge cases you don't have to handle. The input tree will contain all
 * characters that make up the input string.
 */
Queue<Bit> encodeText(const string& str, EncodingTreeNode* tree) {
    /* Build a table mapping each unique char to its corresponding code. */
    Map<char, string> encode;
    buildTable(tree, encode, "");

    Queue<Bit> bits;
    /* Add each bits sequence of each character in the string to the queue. */
    for (char c: str) {
        enqueueBits(encode[c], bits);
    }
    return bits;
}

/**
 * Decodes the given Queue<Bit> and Queue<char> into a Huffman coding tree.
 *
 * You can assume that the input Queues are structured properly in that they
 * represent a legal encoding of a tree, that there aren't stray characters
 * or bits in them, etc.
 */
EncodingTreeNode* decodeTree(Queue<Bit>& bits, Queue<char>& leaves) {
    Bit bit = bits.dequeue();
    EncodingTreeNode* tree = new EncodingTreeNode;
    /* Base case: If the tree is a single leaf node, we update the value. */
    if (bit == 0) {
        tree->ch = leaves.dequeue();
        tree->zero = nullptr;
        tree->one = nullptr;
    }
    /* If the tree is not a leaf node, using recursion update its left node and right node. */
    else {
        tree->zero = decodeTree(bits, leaves);
        tree->one = decodeTree(bits, leaves);
    }
    return tree;
}

/**
 * Encodes the given Huffman tree as a Queue<Bit> and Queue<char> in the manner
 * specified in the assignment handout.
 *
 * You can assume the input Queues are empty on entry to this function.
 *
 * You can assume that the Huffman tree provided to you is properly structured,
 * in that each internal node will have two children and only the characters in
 * the leaves matter, etc.
 */
void encodeTree(EncodingTreeNode* tree, Queue<Bit>& bits, Queue<char>& leaves) {
    if (tree->zero == nullptr && tree->one == nullptr) {
        bits.enqueue(0);
        leaves.enqueue(tree->ch);
    } else {
        bits.enqueue(1);
        encodeTree(tree->zero, bits, leaves);
        encodeTree(tree->one, bits, leaves);
    }
}

/**
 * Compresses the given text string using Huffman coding, producing as output
 * a HuffmanResult containing the encoded tree and message.
 *
 * Your implementation of this function should report an error if there are
 * fewer than two distinct characters in the input string.
 */
HuffmanResult compress(const string& text) {
    EncodingTreeNode* huffmanTree = huffmanTreeFor(text);

    /* Update bits and leaves information of the tree. */
    Queue<Bit> treeBits;
    Queue<char> treeLeaves;
    encodeTree(huffmanTree, treeBits, treeLeaves);

    /* Get the compressed bits sequence of the text. */
    Queue<Bit> messageBits = encodeText(text, huffmanTree);

    HuffmanResult result;
    result.treeBits = treeBits;
    result.treeLeaves = treeLeaves;
    result.messageBits = messageBits;

    deleteTree(huffmanTree);
    return result;
}

/**
 * Decompresses the given HuffmanResult and returns the string it represents.
 *
 * Your implementation may change the file parameter however it sees fit. There
 * are no requirements about what it should look like after this function
 * returns.
 *
 * You can assume the input file is well-formed and was created by a correct
 * implementation of compress.
 */
string decompress(HuffmanResult& file) {
    Queue<Bit> treeBits = file.treeBits;
    Queue<char> treeLeaves = file.treeLeaves;
    Queue<Bit> messageBits = file.messageBits;

    EncodingTreeNode* huffmanTree = decodeTree(treeBits, treeLeaves);
    string text = decodeText(messageBits, huffmanTree);
    deleteTree(huffmanTree);
    return text;
}

/* * * * * * Test Cases Below This Point * * * * * */
#include "GUI/SimpleTest.h"

/* * * * * Student test cases are at the bottom of the file. * * * * */

/* * * * * Provided Tests Below This Point * * * * */
#include <limits>

/* Utility function to test if a purported Huffman tree is indeed a Huffman tree.
 * Specifically, this checks that each internal node has either zero or two
 * children. There are other ways you could produce an invalid Huffman tree - for
 * example, by having uninitialized pointers or by linking in a cycle - but we
 * don't test for that here.
 */
bool isEncodingTree(EncodingTreeNode* tree) {
    /* The empty tree is not a Huffman tree. */
    if (tree == nullptr) return false;

    /* If we have one missing child, we should have two missing children. */
    if ((tree->zero == nullptr) != (tree->one == nullptr)) return false;

    /* If we have children at all, they need to be Huffman trees. */
    return tree->zero == nullptr || (isEncodingTree(tree->zero) && isEncodingTree(tree->one));
}

/* Utility function to test if two trees are equal. This is adapted from Section
 * Handout 8 and particularized to Huffman trees.
 */
bool areEqual(EncodingTreeNode* lhs, EncodingTreeNode* rhs) {
    /* Base case: If either is a leaf, both should be. */
    bool lhsLeaf = lhs->zero == nullptr && lhs->one == nullptr;
    bool rhsLeaf = rhs->zero == nullptr && rhs->one == nullptr;
    if (lhsLeaf || rhsLeaf) {
        return lhs->ch == rhs->ch && lhsLeaf == rhsLeaf;
    }

    /* Otherwise, they're both internal nodes. Check that they match. */
    return areEqual(lhs->zero, rhs->zero) && areEqual(lhs->one, rhs->one);
}

/* Utility function to return a string of all possible characters. */
string pangrammaticString() {
    string result;

    char ch = numeric_limits<char>::min();
    result += ch;
    do {
        ch++;
        result += ch;
    } while (ch != numeric_limits<char>::max());

    return result;
}

/* Utility function that makes an inefficient but still valid encoding tree
 * for the given characters.
 */
EncodingTreeNode* strandTreeFor(const string& text, size_t index = 0) {
    if (index == text.size()) error("No characters provided to strandTreeFor.");

    /* We always get a leaf node. */
    EncodingTreeNode* leaf = new EncodingTreeNode {
        text[index], nullptr, nullptr
    };

    /* Last character? If so, that's all. */
    if (index + 1 == text.size()) return leaf;

    /* Otherwise, build a larger tree. */
    else return new EncodingTreeNode {
        ' ', leaf, strandTreeFor(text, index + 1)
    };
}

PROVIDED_TEST("huffmanTreeFor reports errors on invalid inputs.") {
    EXPECT_ERROR(huffmanTreeFor(""));    // No characters
    EXPECT_ERROR(huffmanTreeFor("a"));   // One character
    EXPECT_ERROR(huffmanTreeFor("aaa")); // One character
}

PROVIDED_TEST("huffmanTreeFor builds tree for two characters.") {
    EncodingTreeNode* reference = new EncodingTreeNode {
        ' ', new EncodingTreeNode {'a', nullptr, nullptr}, new EncodingTreeNode {'b', nullptr, nullptr}
    };

    EncodingTreeNode* tree = huffmanTreeFor("aaabbbb");
    EXPECT(isEncodingTree(tree));
    EXPECT(areEqual(tree, reference));

    deleteTree(reference);
    deleteTree(tree);
}

PROVIDED_TEST("huffmanTreeFor works on the full range of characters.") {
    /* Get a string of all possible characters, then pair them off and see what we find. */
    string allChars = pangrammaticString();
    for (size_t i = 0; i < allChars.size(); i += 2) {
        string toEncode;
        toEncode += allChars[i];
        toEncode += allChars[i + 1];
        toEncode += allChars[i + 1];

        EncodingTreeNode* reference = new EncodingTreeNode {
            ' ',
            new EncodingTreeNode {allChars[i], nullptr, nullptr},
            new EncodingTreeNode {allChars[i + 1], nullptr, nullptr}
        };

        EncodingTreeNode* tree = huffmanTreeFor(toEncode);
        EXPECT(isEncodingTree(tree));
        EXPECT(areEqual(tree, reference));

        deleteTree(reference);
        deleteTree(tree);
    }
}

PROVIDED_TEST("huffmanTreeFor uses cumulative weights (v1).") {
    /* This tree:
     *                 *
     *                / \
     *               *   D
     *              / \
     *             C   *
     *                / \
     *               A   B
     */
    EncodingTreeNode* reference = new EncodingTreeNode {
        '*',
            new EncodingTreeNode { '!',
                new EncodingTreeNode { 'C', nullptr, nullptr },
                new EncodingTreeNode { '?',
                    new EncodingTreeNode { 'A', nullptr, nullptr },
                    new EncodingTreeNode { 'B', nullptr, nullptr }
                }
            },
            new EncodingTreeNode { 'D', nullptr, nullptr }
    };

    /* Ax2, Bx3, Cx4, Dx10 */
    EncodingTreeNode* tree = huffmanTreeFor("AABBBCCCCDDDDDDDDDD");
    EXPECT(isEncodingTree(tree));
    EXPECT(areEqual(tree, reference));

    deleteTree(reference);
    deleteTree(tree);
}

PROVIDED_TEST("huffmanTreeFor uses cumulative weights (v2).") {
    /*
     *          *
     *       /     \
     *      *       *
     *     / \     / \
     *    D   E   F   *
     *               / \
     *              C   *
     *                 / \
     *                A   B
     */
    EncodingTreeNode* reference =new EncodingTreeNode {
        ' ',
        new EncodingTreeNode {
            ' ',
            new EncodingTreeNode{ 'D', nullptr, nullptr },
            new EncodingTreeNode{ 'E', nullptr, nullptr }
        },
        new EncodingTreeNode {
            ' ',
            new EncodingTreeNode{ 'F', nullptr, nullptr },
            new EncodingTreeNode {
                ' ',
                new EncodingTreeNode{ 'C', nullptr, nullptr },
                new EncodingTreeNode{
                    ' ',
                    new EncodingTreeNode{ 'A', nullptr, nullptr },
                    new EncodingTreeNode{ 'B', nullptr, nullptr },
                }
            }
        }
    };

    /* Ax2, Bx3, Cx4, Dx6, Ex7, Fx8 */
    EncodingTreeNode* tree = huffmanTreeFor("AABBBCCCCDDDDDDEEEEEEEFFFFFFFF");
    EXPECT(isEncodingTree(tree));

    EXPECT(areEqual(tree, reference));

    deleteTree(reference);
    deleteTree(tree);
}

PROVIDED_TEST("decodeText works on small sample.") {
    /* This tree:
     *                 *
     *                / \
     *               O   *
     *                  / \
     *                 *   N
     *                / \
     *               M   S
     */
    EncodingTreeNode* tree = new EncodingTreeNode {
        '*',
            new EncodingTreeNode { 'O', nullptr, nullptr },
            new EncodingTreeNode { '*',
                new EncodingTreeNode{ '*',
                    new EncodingTreeNode { 'M', nullptr, nullptr },
                    new EncodingTreeNode { 'S', nullptr, nullptr }
                },
                new EncodingTreeNode{ 'N', nullptr, nullptr }
            }
    };

    /* What you get if you encode MONSOON with this tree. */
    Queue<Bit> bits = { 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1 };

    EXPECT_EQUAL(decodeText(bits, tree), "MONSOON");

    deleteTree(tree);
}

PROVIDED_TEST("Can decode all char values.") {
    /* All possible characters. */
    string allChars = pangrammaticString();

    /* Try decoding all pairs of adjacent characters. */
    for (size_t i = 0; i < allChars.size(); i += 2) {
        string expected;
        expected += allChars[i];
        expected += allChars[i + 1];
        expected += allChars[i + 1];

        EncodingTreeNode* tree = new EncodingTreeNode {
            ' ',
            new EncodingTreeNode {allChars[i], nullptr, nullptr},
            new EncodingTreeNode {allChars[i + 1], nullptr, nullptr}
        };

        /* Decode the bitstream 011, which should map back to the expected
         * string.
         */
        Queue<Bit> bits = { 0, 1, 1 };
        EXPECT_EQUAL(decodeText(bits, tree), expected);

        deleteTree(tree);
    }
}

PROVIDED_TEST("encodeText works on small sample.") {
    /* This tree:
     *                 *
     *                / \
     *               O   *
     *                  / \
     *                 *   N
     *                / \
     *               M   S
     */
    EncodingTreeNode* tree = new EncodingTreeNode {
           '*',
           new EncodingTreeNode { 'O', nullptr, nullptr },
               new EncodingTreeNode { '*',
               new EncodingTreeNode{ '*',
               new EncodingTreeNode { 'M', nullptr, nullptr },
               new EncodingTreeNode { 'S', nullptr, nullptr }
            },
            new EncodingTreeNode{ 'N', nullptr, nullptr }
        }
    };

    /* What you get if you encode MONSOON with this tree. */
    Queue<Bit> expected = { 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1 };

    EXPECT_EQUAL(encodeText("MONSOON", tree), expected);

    deleteTree(tree);
}

PROVIDED_TEST("Can encode all char values.") {
    /* All possible characters. */
    string allChars = pangrammaticString();

    /* Try encoding all pairs of adjacent characters. */
    for (size_t i = 0; i < allChars.size(); i += 2) {
        string toEncode;
        toEncode += allChars[i];
        toEncode += allChars[i + 1];
        toEncode += allChars[i + 1];

        EncodingTreeNode* tree = new EncodingTreeNode {
                ' ',
                new EncodingTreeNode {allChars[i], nullptr, nullptr},
                new EncodingTreeNode {allChars[i + 1], nullptr, nullptr}
        };

        /* See what bits we get back. We should get 011, since the first
         * character has code 0 and the second has code 1.
         */
        Queue<Bit> bits = encodeText(toEncode, tree);
        Queue<Bit> expected = { 0, 1, 1 };

        EXPECT_EQUAL(bits, expected);

        deleteTree(tree);
    }
}

PROVIDED_TEST("decodeText undoes encodeText on range of sample strings.") {
    Vector<string> testCases = {
        "THAT THAT IS IS THAT THAT IS NOT IS NOT IS THAT IT IT IS",
        "AABAAABBABAAABAAAA",
        ":-) :-D XD <(^_^)>",
        pangrammaticString(),
    };

    for (string test: testCases) {
        /* Use a silly encoding scheme, but one that works regardless of what
         * characters are provided.
         */
        EncodingTreeNode* tree = strandTreeFor(test);
        EXPECT(isEncodingTree(tree));

        Queue<Bit> bits = encodeText(test, tree);
        string result = decodeText(bits, tree);

        EXPECT_EQUAL(test.size(), result.size());
        EXPECT_EQUAL(test, result);

        deleteTree(tree);
    }
}

PROVIDED_TEST("Can decode an example tree.") {
    /* This encodes this tree:
     *
     *                 *
     *                / \
     *               *   C
     *              / \
     *             A   B
     */
    Queue<Bit>  bits   = { 1, 1, 0, 0, 0 };
    Queue<char> leaves = { 'A', 'B', 'C' };

    EncodingTreeNode* tree = decodeTree(bits, leaves);
    EXPECT(isEncodingTree(tree));

    /* Confirm this is the right tree. */
    EncodingTreeNode* expected = new EncodingTreeNode {
        '*',
            new EncodingTreeNode {
                '*',
                new EncodingTreeNode { 'A', nullptr, nullptr },
                new EncodingTreeNode { 'B', nullptr, nullptr },
            },
            new EncodingTreeNode { 'C', nullptr, nullptr }
    };

    EXPECT(areEqual(tree, expected));

    deleteTree(tree);
    deleteTree(expected);
}

PROVIDED_TEST("Can decode trees using all possible char values.") {
    /* All possible characters. */
    string allChars = pangrammaticString();

    /* Try encoding all pairs of adjacent characters. */
    for (size_t i = 0; i < allChars.size(); i += 2) {
        EncodingTreeNode* expected = new EncodingTreeNode {
            ' ',
            new EncodingTreeNode {allChars[i], nullptr, nullptr},
            new EncodingTreeNode {allChars[i + 1], nullptr, nullptr}
        };
        Queue<Bit>  treeBits   = { 1, 0, 0 };
        Queue<char> treeLeaves = { allChars[i], allChars[i + 1] };

        EncodingTreeNode* tree = decodeTree(treeBits, treeLeaves);
        EXPECT(isEncodingTree(tree));
        EXPECT(areEqual(tree, expected));

        deleteTree(tree);
        deleteTree(expected);
    }
}

PROVIDED_TEST("Can encode an example tree.") {
    /* Build an encoding tree for "ABBCCCC." It should look like this:
     *
     *                 *
     *                / \
     *               *   C
     *              / \
     *             A   B
     *
     * This will compress down to
     *
     *        11000
     *        ABC
     */
    EncodingTreeNode* tree = huffmanTreeFor("ABBCCCC");

    Queue<Bit>  bits;
    Queue<char> leaves;

    encodeTree(tree, bits, leaves);

    Queue<Bit>  expectedBits   = { 1, 1, 0, 0, 0 };
    Queue<char> expectedLeaves = { 'A', 'B', 'C' };

    EXPECT_EQUAL(bits,   expectedBits);
    EXPECT_EQUAL(leaves, expectedLeaves);

    deleteTree(tree);
}

PROVIDED_TEST("Can encode trees using all possible char values.") {
    /* All possible characters. */
    string allChars = pangrammaticString();

    /* Try encoding all pairs of adjacent characters. */
    for (size_t i = 0; i < allChars.size(); i += 2) {
        EncodingTreeNode* tree = new EncodingTreeNode {
            ' ',
            new EncodingTreeNode {allChars[i], nullptr, nullptr},
            new EncodingTreeNode {allChars[i + 1], nullptr, nullptr}
        };

        /* See what we get back. It should be the bitstring 100 (root with
         * two children) and the two leaves, in order.
         */
        Queue<Bit>  treeBits;
        Queue<char> treeLeaves;

        Queue<Bit>  expectedBits = { 1, 0, 0 };
        Queue<char> expectedLeaves = { allChars[i], allChars[i + 1] };

        encodeTree(tree, treeBits, treeLeaves);
        EXPECT_EQUAL(treeBits, expectedBits);
        EXPECT_EQUAL(treeLeaves, expectedLeaves);

        deleteTree(tree);
    }
}

PROVIDED_TEST("decodeTree undoes encodeTree on sample strings.") {
    /* Make a Huffman tree for the string of all characters. */
    EncodingTreeNode* sourceTree = huffmanTreeFor(pangrammaticString());
    EXPECT(isEncodingTree(sourceTree));

    /* Encode, then decode it. */
    Queue<Bit>  bits;
    Queue<char> leaves;
    encodeTree(sourceTree, bits, leaves);

    EncodingTreeNode* resultTree = decodeTree(bits, leaves);
    EXPECT(isEncodingTree(resultTree));
    EXPECT(areEqual(sourceTree, resultTree));

    deleteTree(sourceTree);
    deleteTree(resultTree);
}

PROVIDED_TEST("Can decompress a small sample file.") {
    HuffmanResult file = {
        { 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0 },
        { 'u', 'k', 'p', 'n', 'a', 'm', 'h' },
        { 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1,
          0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0,
          0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0 }
    };

    EXPECT_EQUAL(decompress(file), "humuhumunukunukuapuaa");
}

PROVIDED_TEST("Compress reports errors on bad inputs.") {
    EXPECT_ERROR(compress(""));
    EXPECT_ERROR(compress("A"));
    EXPECT_ERROR(compress("AAAA"));
}

PROVIDED_TEST("Can compress a small sample file.") {
    HuffmanResult file = compress("ABANANAABANDANA");
    Queue<Bit>  treeBits    = { 1, 1, 1, 0, 0, 0, 0 };
    Queue<char> treeChars   = { 'D', 'B', 'N', 'A' };
    Queue<Bit>  messageBits = { 1, 0, 0, 1, 1, 0, 1, 1, 0,
                                1, 1, 1, 0, 0, 1, 1, 0, 1,
                                0, 0, 0, 1, 0, 1, 1 };

    EXPECT_EQUAL(file.treeBits, treeBits);
    EXPECT_EQUAL(file.treeLeaves, treeChars);
    EXPECT_EQUAL(file.messageBits, messageBits);
}

PROVIDED_TEST("Compress undoes decompress on a range of strings.") {
    Vector<string> testCases = {
        "THAT THAT IS IS THAT THAT IS NOT IS NOT IS THAT IT IT IS",
        "AABAAABBABAAABAAAA",
        ":-) :-D XD <(^_^)>",
        pangrammaticString(),
    };

    for (string test: testCases) {
        HuffmanResult file = compress(test);
        string result = decompress(file);

        EXPECT_EQUAL(result.size(), test.size());
        EXPECT_EQUAL(test, result);
    }
}

/* * * * * Student tests * * * * */

STUDENT_TEST("huffmanTreeFor") {
    /*
     *             *
     *           /   \
     *          /     *
     *         /     / \
     *        /     /   *
     *       /     /   / \
     *      /     *    *  \
     *     /     / \  / \  \
     *    f     c  d  a  b  e
     */
    EncodingTreeNode* reference =new EncodingTreeNode {
        '*',
        new EncodingTreeNode {
            'f',
            nullptr,
            nullptr
        },
        new EncodingTreeNode {
            '*',
            new EncodingTreeNode {
                '*',
                new EncodingTreeNode {
                    'c',
                    nullptr,
                    nullptr
                },
                new EncodingTreeNode {
                    'd',
                    nullptr,
                    nullptr
                }
            },
            new EncodingTreeNode {
                '*',
                new EncodingTreeNode {
                    '*',
                    new EncodingTreeNode {
                        'a',
                        nullptr,
                        nullptr
                    },
                    new EncodingTreeNode {
                        'b',
                        nullptr,
                        nullptr
                    }
                },
                new EncodingTreeNode {
                    'e',
                    nullptr,
                    nullptr
                }
            }
        }
    };

    /* ax5 bx9 cx12 dx13 ex16 fx45 */
    string str = string(5, 'a') + string(9, 'b') + string(12, 'c') + string(13, 'd') +
                 string(16, 'e') + string(45, 'f');
    EncodingTreeNode* tree = huffmanTreeFor(str);
    EXPECT(isEncodingTree(tree));

    EXPECT(areEqual(tree, reference));

    deleteTree(reference);
    deleteTree(tree);
}

STUDENT_TEST("encodeText") {
    /*
     *             *
     *           /   \
     *          /     *
     *         /     / \
     *        /     /   *
     *       /     /   / \
     *      /     *    *  \
     *     /     / \  / \  \
     *    f     c  d  a  b  e
     */
    EncodingTreeNode* tree = new EncodingTreeNode {
        '*',
        new EncodingTreeNode {
            'f',
            nullptr,
            nullptr
        },
        new EncodingTreeNode {
            '*',
            new EncodingTreeNode {
                '*',
                new EncodingTreeNode {
                    'c',
                    nullptr,
                    nullptr
                },
                new EncodingTreeNode {
                    'd',
                    nullptr,
                    nullptr
                }
            },
            new EncodingTreeNode {
                '*',
                new EncodingTreeNode {
                    '*',
                    new EncodingTreeNode {
                        'a',
                        nullptr,
                        nullptr
                    },
                    new EncodingTreeNode {
                        'b',
                        nullptr,
                        nullptr
                    }
                },
                new EncodingTreeNode {
                    'e',
                    nullptr,
                    nullptr
                }
            }
        }
    };

    /* What you get if you encode fcdabe with this tree. */
    Queue<Bit> expected = { 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1 };

    EXPECT_EQUAL(encodeText("fcdabe", tree), expected);

    deleteTree(tree);
}

PROVIDED_TEST("decodeText") {
    /*
     *             *
     *           /   \
     *          /     *
     *         /     / \
     *        /     /   *
     *       /     /   / \
     *      /     *    *  \
     *     /     / \  / \  \
     *    f     c  d  a  b  e
     */
    EncodingTreeNode* tree = new EncodingTreeNode {
        '*',
        new EncodingTreeNode {
            'f',
            nullptr,
            nullptr
        },
        new EncodingTreeNode {
            '*',
            new EncodingTreeNode {
                '*',
                new EncodingTreeNode {
                    'c',
                    nullptr,
                    nullptr
                },
                new EncodingTreeNode {
                    'd',
                    nullptr,
                    nullptr
                }
            },
            new EncodingTreeNode {
                '*',
                new EncodingTreeNode {
                    '*',
                    new EncodingTreeNode {
                        'a',
                        nullptr,
                        nullptr
                    },
                    new EncodingTreeNode {
                        'b',
                        nullptr,
                        nullptr
                    }
                },
                new EncodingTreeNode {
                    'e',
                    nullptr,
                    nullptr
                }
            }
        }
    };

    /* What you get if you encode fcdabe with this tree. */
    Queue<Bit> bits = { 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1 };

    EXPECT_EQUAL(decodeText(bits, tree), "fcdabe");

    deleteTree(tree);
}

STUDENT_TEST("Can encode an example tree.") {
    /*
     *             *
     *           /   \
     *          /     *
     *         /     / \
     *        /     /   *
     *       /     /   / \
     *      /     *    *  \
     *     /     / \  / \  \
     *    f     c  d  a  b  e
     */

    /* ax5 bx9 cx12 dx13 ex16 fx45 */
    string str = string(5, 'a') + string(9, 'b') + string(12, 'c') + string(13, 'd') +
                 string(16, 'e') + string(45, 'f');
    EncodingTreeNode* tree = huffmanTreeFor(str);

    Queue<Bit>  bits;
    Queue<char> leaves;

    encodeTree(tree, bits, leaves);

    Queue<Bit>  expectedBits   = { 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0 };
    Queue<char> expectedLeaves = { 'f', 'c', 'd', 'a', 'b', 'e' };

    EXPECT_EQUAL(bits,   expectedBits);
    EXPECT_EQUAL(leaves, expectedLeaves);

    deleteTree(tree);
}

STUDENT_TEST("Can decode an example tree.") {
    /*
     *             *
     *           /   \
     *          /     *
     *         /     / \
     *        /     /   *
     *       /     /   / \
     *      /     *    *  \
     *     /     / \  / \  \
     *    f     c  d  a  b  e
     */

    /* ax5 bx9 cx12 dx13 ex16 fx45 */
    string str = string(5, 'a') + string(9, 'b') + string(12, 'c') + string(13, 'd') +
                 string(16, 'e') + string(45, 'f');

    Queue<Bit>  bits   = { 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0 };
    Queue<char> leaves = { 'f', 'c', 'd', 'a', 'b', 'e' };

    EncodingTreeNode* tree = decodeTree(bits, leaves);
    EXPECT(isEncodingTree(tree));

    /* Confirm this is the right tree. */
    EncodingTreeNode* expected = new EncodingTreeNode {
        '*',
        new EncodingTreeNode {
            'f',
            nullptr,
            nullptr
        },
        new EncodingTreeNode {
            '*',
            new EncodingTreeNode {
                '*',
                new EncodingTreeNode {
                    'c',
                    nullptr,
                    nullptr
                },
                new EncodingTreeNode {
                    'd',
                    nullptr,
                    nullptr
                }
            },
            new EncodingTreeNode {
                '*',
                new EncodingTreeNode {
                    '*',
                    new EncodingTreeNode {
                        'a',
                        nullptr,
                        nullptr
                    },
                    new EncodingTreeNode {
                        'b',
                        nullptr,
                        nullptr
                    }
                },
                new EncodingTreeNode {
                    'e',
                    nullptr,
                    nullptr
                }
            }
        }
    };

    EXPECT(areEqual(tree, expected));

    deleteTree(tree);
    deleteTree(expected);
}

void addBits(int n, const string& code, Queue<Bit>& messageBits) {
    for (int i = 0; i < n; ++i) {
        for (char bit: code) {
            if (bit == '0') {
                messageBits.enqueue(0);
            } else {
                messageBits.enqueue(1);
            }
        }
    }
}

STUDENT_TEST("Can compress a small sample file.") {
    /* ax5 bx9 cx12 dx13 ex16 fx45 */
    string str = string(5, 'a') + string(9, 'b') + string(12, 'c') + string(13, 'd') +
                 string(16, 'e') + string(45, 'f');
    HuffmanResult file = compress(str);
    Queue<Bit>  treeBits    = { 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeChars   = { 'f', 'c', 'd', 'a', 'b', 'e' };
    Queue<Bit>  messageBits;

    addBits(5 , "1100", messageBits); // a
    addBits(9 , "1101", messageBits); // b
    addBits(12, "100" , messageBits); // c
    addBits(13, "101" , messageBits); // d
    addBits(16, "111" , messageBits); // e
    addBits(45, "0"   , messageBits); // f

    EXPECT_EQUAL(file.treeBits, treeBits);
    EXPECT_EQUAL(file.treeLeaves, treeChars);
    EXPECT_EQUAL(file.messageBits, messageBits);
}

STUDENT_TEST("Can decompress a small sample file.") {
    /* ax5 bx9 cx12 dx13 ex16 fx45 */
    string str = string(5, 'a') + string(9, 'b') + string(12, 'c') + string(13, 'd') +
                 string(16, 'e') + string(45, 'f');
    Queue<Bit>  messageBits;
    addBits(5 , "1100", messageBits); // a
    addBits(9 , "1101", messageBits); // b
    addBits(12, "100" , messageBits); // c
    addBits(13, "101" , messageBits); // d
    addBits(16, "111" , messageBits); // e
    addBits(45, "0"   , messageBits); // f

    HuffmanResult file = {
        { 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0 },
        { 'f', 'c', 'd', 'a', 'b', 'e' },
        messageBits
    };

    EXPECT_EQUAL(decompress(file), str);
}
