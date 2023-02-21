#ifndef WATOPOLY_ACADEMICBUILDING_H
#define WATOPOLY_ACADEMICBUILDING_H

#include "payableBuilding.h"
#include <string>

const int MAX_IMPROVEMENT = 6;

class AcademicBuilding: public PayableBuilding {
public:
    enum Block {
        Arts1,
        Arts2,
        Eng,
        Health,
        Env,
        Sci1,
        Sci2,
        Math
    };
    AcademicBuilding(Block block, std::string name, Player* owner, int* tuitions, int purchaseCost, int improveCost);
    ~AcademicBuilding();
    int getTuition() const override;
    int getImproveCost() const;
    int getLevel() const;
    Block getBlock() const;
    void event(Player* player, std::ostream& out, std::istream& in) override;
    void setOwner(Player* newOwner);
    void setLevel(int level);
private:
    Block block;
    int level = 0;
    int tuitions[MAX_IMPROVEMENT];
    int improveCost;
    bool checkMonopolyFormed();
    bool monopolyFormed = false;
    void notify(Subject<PayableBuildingInfo, PayableBuildingState> &whoFrom) override;
};


#endif
