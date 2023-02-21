#ifndef _TUITION_H_
#define _TUITION_H_
#include "otherNonPayableBuildings.h"

class Player;

class Tuition : public OtherNonPayableBuildings {
    public:
        Tuition();
        ~Tuition();
        void event(Player* player, std::ostream& out, std::istream& in) override;
};

#endif
