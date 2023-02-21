#include "cell.h"

Cell::Cell(std::string name) : name{name} {}

Cell::~Cell() {}

std::string Cell::getName() {
    return name;
}

void Cell::setType(Type type) {
    this->type = type;
}

Cell::Type Cell::getType() const {
    return type;
}
