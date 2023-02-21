#ifndef _OTHER_NON_PAYABLE_BUILDINGS_H_
#define _OTHER_NON_PAYABLE_BUILDINGS_H_
#include "nonPayableBuilding.h"
#include "player.h"

class OtherNonPayableBuildings : public NonPayableBuilding {
    public:
        OtherNonPayableBuildings(std::string name);
        ~OtherNonPayableBuildings();
        virtual void event(Player* player, std::ostream& out, std::istream& in) override;
        std::string getName();
};

#endif
