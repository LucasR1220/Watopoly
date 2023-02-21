#ifndef CARD_H
#define CARD_H

#include "player.h"

class Card {
public:
    enum Type {move, money, tims, osap};
    Card(Card::Type type, int step = 0, int amount = 0);
    void apply(Player& player);
    Type actionType;
    int step;
    int amount;
    void what();
};

#endif
