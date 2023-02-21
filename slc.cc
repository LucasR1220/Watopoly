#include "slc.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

SLC::SLC() : NonPayableBuilding{"SLC"} {

    for (int i = 0; i < 3; i++) deck.push_back(Card(Card::move, -3));
    for (int i = 0; i < 4; i++) deck.push_back(Card(Card::move, -2));
    for (int i = 0; i < 4; i++) deck.push_back(Card(Card::move, -1));
    for (int i = 0; i < 3; i++) deck.push_back(Card(Card::move, 1));
    for (int i = 0; i < 4; i++) deck.push_back(Card(Card::move, 2));
    for (int i = 0; i < 4; i++) deck.push_back(Card(Card::move, 3));
    deck.push_back(Card(Card::tims, 3));
    deck.push_back(Card(Card::osap, 3));

    rng = std::default_random_engine{seed};

    std::shuffle( deck.begin(), deck.end(), rng );

}

SLC::~SLC() {}


void SLC::event(Player* player, std::ostream& out, std::istream& in) {

    cout << "You landed on SLC." << endl;
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

unsigned int SLC::getSeed() {
    return seed;
}

void SLC::setSeed(unsigned int seed) {
    this->seed = seed;
}
