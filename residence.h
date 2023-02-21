#ifndef RESIDENCE_H
#define RESIDENCE_H
#include <string>
#include "payableBuilding.h"

class Player;

const int RESIDENCE_COUNT = 4;
class Residence: public PayableBuilding {
    std::vector<int> tuitions;
    int residencesOwnedByOwner = 0;
public:
    Residence(std::string name, Player* owner, int purchaseCost, std::vector<int>& tuitions);
    ~Residence();
    int getTuition() const override;
    void notify(Subject<PayableBuildingInfo, PayableBuildingState> &whoFrom) override;
    void setOwner(Player* newOwner) override;
    void event(Player* player, std::ostream& out, std::istream& in) override;
};

#endif
