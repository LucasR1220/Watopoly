#include "needlesHall.h"
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

NeedlesHall::NeedlesHall() : NonPayableBuilding{"NEEDLES HALL"} {
    deck.push_back(Card(Card::money, -200));
    deck.push_back(Card(Card::money, 200));
    for (int i = 0; i < 2; i++) deck.push_back(Card(Card::money, 0, -100));
    for (int i = 0; i < 2; i++) deck.push_back(Card(Card::money, 0, 100));
    for (int i = 0; i < 3; i++) deck.push_back(Card(Card::money, 0, -50));
    for (int i = 0; i < 3; i++) deck.push_back(Card(Card::money, 0, 50));
    for (int i = 0; i < 6; i++) deck.push_back(Card(Card::money, 0, 25));
    rng = std::default_random_engine{seed};

    std::shuffle( deck.begin(), deck.end(), rng );
}

NeedlesHall::~NeedlesHall() {}

void NeedlesHall::event(Player * player, std::ostream& out, std::istream& in) {
    cout << "You landed on Needles Hall." << endl;
    if (deck.size() == 0) {
        deck = std::move(drawed);
        std::shuffle( deck.begin(), deck.end(), rng );
    } else {
        Card card = deck[deck.size() - 1];
        card.what();
        card.apply(*player);
        drawed.push_back(card);
        deck.pop_back();
    }
}

unsigned int NeedlesHall::getSeed() {
    return seed;
}

void NeedlesHall::setSeed(unsigned int seed) {
    this->seed = seed;
}
