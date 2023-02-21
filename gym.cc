#include "gym.h"
#include "dice.h"
#include <iostream>

Gym::Gym(std::string name, Player* owner, int purchaseCost) : PayableBuilding{name, owner, purchaseCost} {
    setType(Cell::NonAcademic);
}

Gym::~Gym() noexcept {}

int Gym::getTuition() const {
    Dice dice;
    if (gymsOwnedByOwner == 1) {
        return (dice.roll() + dice.roll()) * 4;
    } else if (gymsOwnedByOwner == 2) {
        return (dice.roll() + dice.roll()) * 10;
    }
    return 0;
}

void Gym::notify(Subject<PayableBuildingInfo, PayableBuildingState> &whoFrom) {
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
        gymsOwnedByOwner = count + 1;
    }
}

void Gym::setOwner(Player *newOwner) {
    owner = newOwner;
    if (!newOwner) {
        return;
    }
    setState(PayableBuildingState{PayableBuildingState::NEW_OWNER});
    notifyObservers();
    notify(*this);
}

void Gym::event(Player *player, std::ostream& out, std::istream& in) {
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
        out << "This building is owned by " << owner->getName() << " and they own " << gymsOwnedByOwner
            << " gyms in total." << std::endl;
        out << "You are going to be charged by $" << getTuition() << ".";
        player->charge(getTuition());
    } else {
        out << "This building is owned by you." << std::endl;
    }
}
