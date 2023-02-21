#ifndef _TIMS_H_
#define _TIMS_H_
#include "otherNonPayableBuildings.h"

class Player;

class Tims : public OtherNonPayableBuildings {
    public:
        Tims(std::string name);
        ~Tims();
        void event(Player * pName, std::ostream&, std::istream& in) override;
};

#endif
