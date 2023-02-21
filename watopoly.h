#ifndef WATOPOLY_H
#define WATOPOLY_H

#include <iostream>
#include "dice.h"
#include "board.h"
#include "loadSave.h"

class Watopoly {
    bool testing;
    std::string fileName;
    Dice myDice;
    Board gameBoard{};
    LoadSave ls;
public:
    Watopoly(bool testing, std::string fileName);
    ~Watopoly();
    void allCommands(Player* pName, Cell* building, std::vector<std::shared_ptr<Player>> players, std::string order[]);
    void play();
};

#endif
