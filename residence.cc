#include "residence.h"
#include <iostream>

Residence::Residence(std::string name, Player *owner, int purchaseCost, std::vector<int>& tuitions) :
    PayableBuilding(name, owner, purchaseCost) {
    setType(Cell::NonAcademic);
    this->tuitions = tuitions;
}

Residence::~Residence() noexcept {}

int Residence::getTuition() const {
    return tuitions[residencesOwnedByOwner - 1];
}

void Residence::setOwner(Player *newOwner) {
    owner = newOwner;
    if (!newOwner) {
        return;
    }
    setState(PayableBuildingState{PayableBuildingState::NEW_OWNER});
    notifyObservers();
    notify(*this);
}

void Residence::notify(Subject<PayableBuildingInfo, PayableBuildingState> &whoFrom) {
    if (!owner) return;
    if (whoFrom.getState().type == PayableBuildingState::NEW_OWNER) {
        int count = 0;
        for (auto &ob : observers) {
            auto temp = dynamic_cast<Subject*>(ob);
            if (temp) {
                if (temp->getInfo().owner == owner) {
                    count++;
                }
            }
        }
        residencesOwnedByOwner = count + 1;
    }
}

void Residence::event(Player *player, std::ostream& out, std::istream& in) {
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
        out << "This building is owned by " << owner->getName() << " and they own " << residencesOwnedByOwner
            << " residences in total." << std::endl;
        out << "You are going to be charged by $" << getTuition() << ".";
        player->charge(getTuition());
    } else {
        out << "This building is owned by you." << std::endl;
    }
}
