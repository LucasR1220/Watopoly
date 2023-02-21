#include "dice.h"
#include <random>
#include <chrono>

Dice::Dice(unsigned seed) : seed{seed} {
    rng = std::default_random_engine{seed};
}

Dice::Dice() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    rng = std::default_random_engine{seed};
}

Dice::~Dice() {}

int Dice::roll() {
    std::uniform_int_distribution<> dist(1, DICE_SIZE);
    return dist(rng);
}

int Dice::roll(int dice1, int dice2) {
    return dice1 + dice2;
}
