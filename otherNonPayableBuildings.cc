#include "otherNonPayableBuildings.h"
#include <string>
#include <iostream>

using namespace std;

OtherNonPayableBuildings::OtherNonPayableBuildings(string name) : NonPayableBuilding{name} {
    setType(Cell::NonPayable);
}

OtherNonPayableBuildings::~OtherNonPayableBuildings() {}

string OtherNonPayableBuildings::getName() {
    return name;
}

void OtherNonPayableBuildings::event(Player * pName, std::ostream& out, std::istream& in) {
    if (name == "COLLECT OSAP") {
        out << "You collected $200 from OSAP" << endl;
        pName->addBalance(200);
    } else if (name == "COOP FEE") {
        out << "You will be charged $150 for coop fee" << endl;
        pName->charge(150);
    } else if (name == "GO TO TIMS") {
        out << "You will be sent to tims" << endl;
        pName->setPosition(10);
        pName->setAtTims(true);
    } else if (name == "Goose Nesting") {
        out << "You are attacked by a flock of nesting geese》" << endl;
    } else if (name == "DC Tims Line") {
        out << "You landed on DC Tims Line" << endl;
    }
}
