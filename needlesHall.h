#ifndef _NEEDLESHALL_H_
#define _NEEDLESHALL_H_
#include "nonPayableBuilding.h"
#include "card.h"
#include <vector>
#include <chrono>
#include <random>

class Player;

class NeedlesHall : public NonPayableBuilding {
    std::vector<Card> deck;
    std::vector<Card> drawed;
    std::default_random_engine rng;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();;

public:
        NeedlesHall();
        ~NeedlesHall();
        void event(Player* player, std::ostream& out, std::istream& in) override;
        void setSeed(unsigned seed);
        unsigned getSeed();
};

#endif
