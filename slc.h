#ifndef _SLC_H_
#define _SLC_H_
#include "nonPayableBuilding.h"
#include <vector>
#include <chrono>
#include <random>
#include "card.h"

class Player;

class SLC : public NonPayableBuilding {
    std::vector<Card> deck;
    std::vector<Card> drawed;
    std::default_random_engine rng;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    public:
        SLC();
        ~SLC();
        void event(Player* player, std::ostream& out, std::istream& in) override;
        void setSeed(unsigned seed);
        unsigned getSeed();
};
 
#endif
