// Created by August

#include "academicBuilding.h"
#include <iostream>
#include "notEnoughMoneyException.h"

AcademicBuilding::AcademicBuilding(Block block, std::string name, Player* owner, int* tuitions,
                                   int purchaseCost, int improveCost) :
    PayableBuilding{name, owner, purchaseCost}, block{block}, improveCost{improveCost} {
    type = Cell::Academic;
    for (int i = 0; i < MAX_IMPROVEMENT; i++) {
        this->tuitions[i] = tuitions[i];
    }
}

AcademicBuilding::~AcademicBuilding() noexcept {}

int AcademicBuilding::getTuition() const {
    return tuitions[level];
}

int AcademicBuilding::getImproveCost() const {
    return improveCost;
}

int AcademicBuilding::getLevel() const {
    return level;
}

AcademicBuilding::Block AcademicBuilding::getBlock() const {
    return block;
}

void AcademicBuilding::event(Player* player, std::ostream& out, std::istream& in) {
    out << "You landed on " << name << std::endl;
    std::string cmd;
    if (owner == nullptr) {
        out << "Do you want to purchase this building? [Y/N]" << std::endl;
        while (in >> cmd) {
            if (cmd == "Y") {
                if (player->buy(this, getPurchaseCost())) {
                    std::cout << "You have bought this building" << std::endl;
                };
                break;
            } else {
                break;
            }
        }
    } else if (owner != player) {
        out << "This building is owned by " << owner->getName() << " with " << level << " improvements" << std::endl;
        out << "You are going to be charged by $" << getTuition() << "." << std::endl;
        player->charge(getTuition());
    } else {
        out << "This building is owned by you." << std::endl;
    }
}

bool AcademicBuilding::checkMonopolyFormed() {
    for (auto &ob : observers) {
        Subject* s = dynamic_cast<Subject*>(ob);
        if (!s) {
            if (owner == nullptr || s->getInfo().owner == nullptr) {
                return false;
            }
            if (s->getInfo().owner != owner) {
                return false;
            }
        }
    }
    return true;
}

void AcademicBuilding::notify(Subject<PayableBuildingInfo, PayableBuildingState> &whoFrom) {
    if (whoFrom.getState().type == PayableBuildingState::MONOPOLY_FORMED) {
        monopolyFormed = true;
    } else if (whoFrom.getState().type == PayableBuildingState::MONOPOLY_UNFORMED) {
        monopolyFormed = false;
    }
}

void AcademicBuilding::setOwner(Player * newOwner) {
    owner = newOwner;
    if (checkMonopolyFormed() == true) {
        monopolyFormed = true;
        setState(PayableBuildingState{PayableBuildingState::MONOPOLY_FORMED});
        notifyObservers();
    } else {
        setState(PayableBuildingState{PayableBuildingState::MONOPOLY_UNFORMED});
        notifyObservers();
    }
}

void AcademicBuilding::setLevel(int level) {
    if (level >= MAX_IMPROVEMENT) return;
    this->level = level;
}
