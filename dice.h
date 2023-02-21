#ifndef DICE_H
#define DICE_H

#include <random>

const int DICE_SIZE = 6;

class Dice {
    unsigned seed;
    std::default_random_engine rng;
public:
    Dice(unsigned seed);
    Dice();
    ~Dice();
    int roll();
    int roll(int dice1, int dice2);
};

#endif
