#ifndef GYM_H
#define GYM_H

#include "payableBuilding.h"

const int GYM_COUNT = 2;

class Gym: public PayableBuilding {
    int tuitions[GYM_COUNT];
    int gymsOwnedByOwner = 0;
public:
    Gym(std::string name, Player* owner, int purchaseCost);
    ~Gym();
    int getTuition() const override;
    void notify(Subject<PayableBuildingInfo, PayableBuildingState> &whoFrom) override;
    void setOwner(Player* newOwner) override;
    void event(Player* player, std::ostream& out, std::istream& in) override;
};

#endif
