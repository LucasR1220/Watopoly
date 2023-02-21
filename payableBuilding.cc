#include "payableBuilding.h"
#include <string>

using namespace std;

PayableBuilding::PayableBuilding(string name, Player* owner, int purchaseCost) : Cell{name},
    purchaseCost{purchaseCost} {
    setOwner(owner);
}

PayableBuilding::~PayableBuilding() noexcept {}

int PayableBuilding::getPurchaseCost() const {
    return purchaseCost;
}

Player* PayableBuilding::getOwner() {
    return owner;
}



void PayableBuilding::setOwner(Player * newOwner) {
    owner = newOwner;
}

PayableBuildingInfo PayableBuilding::getInfo() const {
    return PayableBuildingInfo{owner};
}
