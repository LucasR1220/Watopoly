#include "nonPayableBuilding.h"
#include <string>
using namespace std;

NonPayableBuilding::NonPayableBuilding(string name) : Cell(name) {
    setType(Cell::NonPayable);
}

NonPayableBuilding::~NonPayableBuilding() {}

Player *NonPayableBuilding::getOwner() {
    return nullptr;
}

void NonPayableBuilding::setOwner(Player *newOwner) {}
