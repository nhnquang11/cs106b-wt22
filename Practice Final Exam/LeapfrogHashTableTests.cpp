#include "LeapfrogHashTable.h"
#include "GUI/SimpleTest.h"
#include "Demos/Timer.h"
using namespace std;

PROVIDED_TEST("Basic insertion tests.") {
    /* Ten slots; each item hashes to its last digit. */
    LeapfrogHashTable table(Hash::identity(10));

    /* Insert 18; should get
     *
     *  .  .  .  .  .  .  .  . 18 .
     *                          -
     */
    EXPECT(table.insert("18"));
    EXPECT_EQUAL(table.size(), 1);

    EXPECT_EQUAL(table.elems[0].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[1].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[2].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[3].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[4].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[5].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[6].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[7].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[8],      { "18", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[9].type, LeapfrogHashTable::SlotType::EMPTY);

    /* Insert 19; should get
     *
     *  .  .  .  .  .  .  .  . 18 19
     *                          -  -
     */
    EXPECT(table.insert("19"));
    EXPECT_EQUAL(table.size(), 2);

    EXPECT_EQUAL(table.elems[0].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[1].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[2].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[3].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[4].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[5].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[6].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[7].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[8],      { "18", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[9],      { "19", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });

    /* Insert 28; should get
     *
     * 28  .  .  .  .  .  .  . 18 19
     *  -                       0  -
     */
    EXPECT(table.insert("28"));
    EXPECT_EQUAL(table.size(), 3);

    EXPECT_EQUAL(table.elems[0],      { "28", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[1].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[2].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[3].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[4].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[5].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[6].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[7].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[8],      { "18", LeapfrogHashTable::SlotType::FILLED, 0 });
    EXPECT_EQUAL(table.elems[9],      { "19", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });

    /* Insert 29; should get
     *
     * 28 29  .  .  .  .  .  . 18 19
     *  -  -                    0  1
     */
    EXPECT(table.insert("29"));
    EXPECT_EQUAL(table.size(), 4);

    EXPECT_EQUAL(table.elems[0],      { "28", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[1],      { "29", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[2].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[3].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[4].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[5].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[6].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[7].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[8],      { "18", LeapfrogHashTable::SlotType::FILLED, 0 });
    EXPECT_EQUAL(table.elems[9],      { "19", LeapfrogHashTable::SlotType::FILLED, 1 });

    /* Insert 38; should get
     *
     * 28 29 38  .  .  .  .  . 18 19
     *  2  -  -                 0  1
     */
    EXPECT(table.insert("38"));
    EXPECT_EQUAL(table.size(), 5);

    EXPECT_EQUAL(table.elems[0],      { "28", LeapfrogHashTable::SlotType::FILLED, 2 });
    EXPECT_EQUAL(table.elems[1],      { "29", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[2],      { "38", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[3].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[4].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[5].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[6].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[7].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[8],      { "18", LeapfrogHashTable::SlotType::FILLED, 0 });
    EXPECT_EQUAL(table.elems[9],      { "19", LeapfrogHashTable::SlotType::FILLED, 1 });

    /* Insert 10; should get
     *
     * 28 29 38 10  .  .  .  . 18 19
     *  2  -  3  -              0  1
     */
    EXPECT(table.insert("10"));
    EXPECT_EQUAL(table.size(), 6);

    EXPECT_EQUAL(table.elems[0],      { "28", LeapfrogHashTable::SlotType::FILLED, 2 });
    EXPECT_EQUAL(table.elems[1],      { "29", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[2],      { "38", LeapfrogHashTable::SlotType::FILLED, 3 });
    EXPECT_EQUAL(table.elems[3],      { "10", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[4].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[5].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[6].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[7].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[8],      { "18", LeapfrogHashTable::SlotType::FILLED, 0 });
    EXPECT_EQUAL(table.elems[9],      { "19", LeapfrogHashTable::SlotType::FILLED, 1 });

    /* Insert 15; should get
     *
     * 28 29 38 10  . 15  .  . 18 19
     *  2  -  3  -     -        0  1
     */
    EXPECT(table.insert("15"));
    EXPECT_EQUAL(table.size(), 7);

    EXPECT_EQUAL(table.elems[0],      { "28", LeapfrogHashTable::SlotType::FILLED, 2 });
    EXPECT_EQUAL(table.elems[1],      { "29", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[2],      { "38", LeapfrogHashTable::SlotType::FILLED, 3 });
    EXPECT_EQUAL(table.elems[3],      { "10", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[4].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[5],      { "15", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[6].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[7].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[8],      { "18", LeapfrogHashTable::SlotType::FILLED, 0 });
    EXPECT_EQUAL(table.elems[9],      { "19", LeapfrogHashTable::SlotType::FILLED, 1 });
}

PROVIDED_TEST("Basic contains tests.") {
    /* Build the table from the previous test. */

    /* Ten slots; each item hashes to its last digit. */
    LeapfrogHashTable table(Hash::identity(10));

    EXPECT(table.insert("18"));
    EXPECT(table.insert("19"));
    EXPECT(table.insert("28"));
    EXPECT(table.insert("29"));
    EXPECT(table.insert("38"));
    EXPECT(table.insert("10"));
    EXPECT(table.insert("15"));

    /* Double-check that this worked. */
    EXPECT_EQUAL(table.size(), 7);
    EXPECT_EQUAL(table.elems[0],      { "28", LeapfrogHashTable::SlotType::FILLED, 2 });
    EXPECT_EQUAL(table.elems[1],      { "29", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[2],      { "38", LeapfrogHashTable::SlotType::FILLED, 3 });
    EXPECT_EQUAL(table.elems[3],      { "10", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[4].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[5],      { "15", LeapfrogHashTable::SlotType::FILLED, LeapfrogHashTable::NOT_LINKED });
    EXPECT_EQUAL(table.elems[6].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[7].type, LeapfrogHashTable::SlotType::EMPTY);
    EXPECT_EQUAL(table.elems[8],      { "18", LeapfrogHashTable::SlotType::FILLED, 0 });
    EXPECT_EQUAL(table.elems[9],      { "19", LeapfrogHashTable::SlotType::FILLED, 1 });

    /* Our table looks like this.
     *
     * 28 29 38 10  . 15  .  . 18 19
     *  2  -  3  -     -        0  1
     *
     * Look up each item that's here.
     */
    EXPECT_EQUAL(table.contains("18"), true);
    EXPECT_EQUAL(table.contains("19"), true);
    EXPECT_EQUAL(table.contains("28"), true);
    EXPECT_EQUAL(table.contains("29"), true);
    EXPECT_EQUAL(table.contains("38"), true);
    EXPECT_EQUAL(table.contains("10"), true);
    EXPECT_EQUAL(table.contains("15"), true);

    /* Look up many items that aren't there to try out different cases. */

    /* Empty slot. */
    EXPECT_EQUAL(table.contains("14"), false);

    /* Chain of length zero. */
    EXPECT_EQUAL(table.contains("25"), false);

    /* Long chain. */
    EXPECT_EQUAL(table.contains("48"), false);
}
