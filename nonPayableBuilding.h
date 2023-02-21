#ifndef _NON_PAYABLE_BUILDING_H_
#define _NON_PAYABLE_BUILDING_H_
#include "cell.h"
#include <string>

class Player;

class NonPayableBuilding : public Cell {
    public:
        NonPayableBuilding(std::string name);
        ~NonPayableBuilding();
        void setOwner(Player* newOwner);
        Player* getOwner();
};

#endif
