#include "LeapfrogHashTable.h"
#include <sstream>
using namespace std;

ostream& operator<< (ostream& out, LeapfrogHashTable::SlotType type) {
    string name = "<unknown slot type>";
    if (type == LeapfrogHashTable::SlotType::EMPTY)     name = "SlotType::EMPTY";
    if (type == LeapfrogHashTable::SlotType::FILLED)    name = "SlotType::FILLED";

    return out << name;
}

ostream& operator<< (ostream& out, LeapfrogHashTable::Slot slot) {
    ostringstream builder;
    builder << "{ " << quotedVersionOf(slot.value) << ", " << slot.type << ", ";

    if (slot.link == LeapfrogHashTable::NOT_LINKED) {
        builder << "NOT_LINKED";
    } else if (slot.link >= 0) {
        builder << slot.link;
    } else {
        builder << "<unknown link>";
    }

    builder << " }";

    return out << builder.str();
}

bool operator== (const LeapfrogHashTable::Slot& lhs, const LeapfrogHashTable::Slot& rhs) {
    if (lhs.type != rhs.type) return false;

    /* Any empty equals only other empties, ignoring the value. */
    if (lhs.type == LeapfrogHashTable::SlotType::EMPTY) return true;

    return lhs.value == rhs.value && lhs.link == rhs.link;
}
