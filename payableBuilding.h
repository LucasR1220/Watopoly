#ifndef PAYABLE_BUILDING_H
#define PAYABLE_BUILDING_H

#include "cell.h"
#include "subject.h"
#include "observer.h"
#include "payableBuildingInfo.h"
#include "payableBuildingState.h"

class Player;

class PayableBuilding: public Cell, public Subject<PayableBuildingInfo, PayableBuildingState>,
        public Observer<PayableBuildingInfo, PayableBuildingState> {
    int purchaseCost;

public:
    PayableBuilding(std::string name, Player* owner, int purchaseCost);
    ~PayableBuilding();
    virtual Player* getOwner() override;
    void setOwner(Player* newOwner) override;
    virtual int getTuition() const = 0;
    int getPurchaseCost() const;
    virtual void notify(Subject<PayableBuildingInfo, PayableBuildingState> &whoFrom) override = 0;
    PayableBuildingInfo getInfo() const override;
};

#endif
