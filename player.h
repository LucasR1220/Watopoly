#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "payableBuilding.h"

class PayableBuilding;
class AcademicBuilding;
class Player {
    int balance;
    bool bankrupt;
    int position;
    char symbol;
    int timsCup;
    bool atTims = false;
    int roundsAtTims = 0;
    std::vector<PayableBuilding*> owned;
    std::vector<PayableBuilding*> mortgaged;
    std::string name;
public:
    Player(char symbol);
    Player(int balance, bool bankrupt, int position, char symbol, int timsCup, bool atTims,
    int roundsAtTims, std::vector<PayableBuilding*> owned, std::vector<PayableBuilding*> mortgaged, std::string name);
    ~Player();
    void setBalance(int update);
    int getBalance();
    int getPosition();
    bool getAtTims();
    void setAtTims(bool update);
    int getRoundsAtTims();
    void setRoundsAtTims(int update);
    void addTimsCup();
    int getTimsCup();
    void remTimsCup();
    bool ownsBuilding(PayableBuilding* building);
    void setPosition(int newPos);
    void setSymbol(char symbol);
    char getSymbol();
    void setBankrupt(bool state);
    bool getBankrupt();
    void addBalance(int update);
    void minusBalance(int update);
    void trade(Player * otherPlayer, int give, PayableBuilding* receive);
    void trade(Player * otherPlayer, PayableBuilding* give, PayableBuilding* receive);
    void trade(Player * otherPlayer, PayableBuilding* give, int receive);
    void mortgage(PayableBuilding* building);
    void move(int steps);
    void moveTo(int position);
    bool buy(PayableBuilding* building, int price);
    void unmortgage(PayableBuilding* building);
    void improve(AcademicBuilding* building);
    void removeOwned(PayableBuilding* building);
    void addOwned(PayableBuilding* building);
    bool ownAnyBuildings();
    void assets();
    std::string getName() const;
    void setName(std::string name);
    void addMortgaged(PayableBuilding * building);
    std::vector<PayableBuilding*> getMortgaged();
    std::vector<PayableBuilding*> getOwned();
    void charge(int amount);
    void transferAssets(Player * win);
    void transferToBank();
    int worth();
};

#endif
