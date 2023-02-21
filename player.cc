#include "player.h"
#include "payableBuilding.h"
#include "academicBuilding.h"
#include "dice.h"
#include "notEnoughMoneyException.h"
#include <iostream>
using namespace std;

Player::Player(char symbol) : balance{1500}, bankrupt{false}, position{0}, symbol{symbol}, 
    timsCup{0} {}

Player::Player(int balance, bool bankrupt, int position, char symbol, int timsCup, bool atTims,
    int roundsAtTims, vector<PayableBuilding*> owned, vector<PayableBuilding*> mortgaged, string name):
    balance{balance}, bankrupt{bankrupt}, position{position}, symbol{symbol}, timsCup{timsCup}, atTims{atTims},
    roundsAtTims{roundsAtTims}, owned{owned}, mortgaged{mortgaged}, name{name}{}
int Player::getBalance() {
    return balance;
}

void Player::setBalance(int update) {
    balance = update;
}

int Player::getPosition() {
    return position;
}

bool Player::getAtTims() {
    return atTims;
}
void Player::setAtTims(bool update) {
    atTims = update;
}

int Player::getRoundsAtTims() {
    return roundsAtTims;
}

void Player::setRoundsAtTims(int update) {
    roundsAtTims = update;
}

void Player::addTimsCup() {
    timsCup += 1;
}
int Player::getTimsCup() {
    return timsCup;
}

void Player::remTimsCup() {
    timsCup -= 1;
}

bool Player::ownsBuilding(PayableBuilding* building) {
    for (auto it : owned) {
        if (it->getName() == building->getName()) {
            return true;
        }
    }
    return false;
}

void Player::setPosition(int newPos) {
    position = newPos;
}

void Player::setSymbol(char symbol) {
    symbol = symbol;
}

char Player::getSymbol() {
    return symbol;
}

void Player::setBankrupt(bool state) {
    bankrupt = state;
}

bool Player::getBankrupt() {
    return bankrupt;
}

void Player::addBalance(int update) {
    balance += update;
}

void Player::minusBalance(int update) {
    balance -= update;
}

void Player::trade(Player * otherPlayer, int give, PayableBuilding* receive) {
    if (balance >= give && otherPlayer->ownsBuilding(receive)) {
        receive->setOwner(this);
        balance -= give;
        otherPlayer->addBalance(give);
        owned.emplace_back(receive);
        otherPlayer->removeOwned(receive);
    } else {
        cout << "Conditions of trade not met" << endl;
    }
}

void Player::trade(Player * otherPlayer, PayableBuilding* give, PayableBuilding* receive) {
    if (ownsBuilding(give) && otherPlayer->ownsBuilding(receive)) {
        give->setOwner(otherPlayer);
        receive->setOwner(this);
        owned.emplace_back(receive);
        int position = 0;
        for (auto it : owned) {
            if (it->getName() == give->getName()) {
                break;
            }
            position += 1;
        }
        owned.erase(owned.begin() + position);
        otherPlayer->removeOwned(receive);
        otherPlayer->addOwned(give);
    } else {
        cout << "Conditions of trade not met" << endl;
    }
}

void Player::trade(Player * otherPlayer, PayableBuilding* give, int receive) {
    if (ownsBuilding(give) && otherPlayer->getBalance() >= receive) {
        give->setOwner(otherPlayer);
        otherPlayer->minusBalance(receive);
        balance += receive;
        int position = 0;
        for (auto it : owned) {
            if (it->getName() == give->getName()) {
                break;
            }
            position += 1;
        }
        owned.erase(owned.begin() + position);
        otherPlayer->addOwned(give);
    } else {
        cout << "Conditions of trade not met" << endl;
    }
}

void Player::move(int steps) {
    int temp = position + steps;
    if (temp >= 40) {
        position = temp - 40;
        balance += 200;
    } else if (temp < 0) {
        position = temp + 40;
    } else {
        position = temp;
    }
}

void Player::moveTo(int position) {
    this->position = position;
}

bool Player::buy(PayableBuilding* building, int price) {
    if (price > balance) {
        cout << "You don't have enough money to buy this!" << endl;
        return false;
    }
    building->setOwner(this);
    owned.emplace_back(building);
    balance -= price;
    return true;
}

void Player::mortgage(PayableBuilding* building) {
    if (ownsBuilding(building)) {
        mortgaged.emplace_back(building);
        int position = 0;
        for (auto it : owned) {
            if (it->getName() == building->getName()) {
                break;
            }
            position += 1;
        }
        owned.erase(owned.begin() + position);
        balance += (building->getPurchaseCost() * 0.5);
        cout << building->getName() << " is now mortgaged" << endl;
    } else {
        cout << "You do not own this property" << endl;
    }
}

void Player::unmortgage(PayableBuilding* building) {
    bool mort = false;
    for (auto i : mortgaged) {
        if (i->getName() == building->getName()) {
            mort = true;
        }
    }
    if (mort) {
        if (balance >= (building->getPurchaseCost() * 0.6)) {
            owned.emplace_back(building);
            int position = 0;
            for (auto it : mortgaged) {
                if (it->getName() == building->getName()) {
                    break;
                }
                position += 1;
            }
            mortgaged.erase(mortgaged.begin() + position);
            balance -= (building->getPurchaseCost() * 0.6);
            cout << building->getName() << " is now unmortgaged" << endl;
        } else {
            cout << "Not enough funds, " << (building->getPurchaseCost() * 0.6);
            cout << "$ needed" << endl;
        }
    } else {
        cout << "Building is not a mortgaged property of yours" << endl;
    }
}

void Player::improve(AcademicBuilding* building) {
    string cmd;
    cin >> cmd;
    if (building->getOwner() != this) {
        cout << "You do not own this building, cannot make improvements" << endl;
    }
    if (cmd == "buy") {
        if (balance >= building->getImproveCost()) {
            if (building->getLevel() == MAX_IMPROVEMENT - 1) {
                cout << "You can't make any more improvements" << endl;
                return;
            } else {
                building->setLevel(building->getLevel() + 1);
                minusBalance(building->getImproveCost());
                cout << building->getName() << " is now level " << building->getLevel() << endl;
            }
        } else {
            cout << "Not enough funds, $" << building->getImproveCost() << " needed, you have $" << balance << endl;
        }
    } else if (cmd == "sell") {
        if (building->getLevel() == 0) {
            cout << "You have no improvements to sell" << endl;
        } else {
            building->setLevel(building->getLevel() - 1);
            addBalance(building->getImproveCost() / 2);
            cout << "Sold one level of improvement, you received $" << building->getImproveCost() / 2 << endl;
        }
    } else {
        cout << "Invalid input." << endl;
    }
}

void Player::removeOwned(PayableBuilding* building) {
    int position = 0;
    for (auto it : owned) {
        if (it == building) {
            break;
        }
        position += 1;
    }
    owned.erase(owned.begin() + position);
}

void Player::addOwned(PayableBuilding* building) {
    owned.emplace_back(building);
}

bool Player::ownAnyBuildings() {
    if (owned.size()) {
        return false;
    } else {
        return true;
    }
}

void Player::assets() {
    cout << "Player: " << symbol << endl;
    cout << "Money: " << balance << endl;
    cout << "Properties Owned: ";
    for (auto i : owned) {
        cout << i->getName() << " ";
    }
    cout << endl;
    cout << "Properties Mortgaged: ";
    for (auto i : mortgaged) {
        cout << i->getName() << " ";
    }
    cout << endl;
}

std::string Player::getName() const {
    return name;
}

void Player::setName(std::string name) {
    this->name = name;
}

std::vector<PayableBuilding*> Player::getMortgaged() {
    return mortgaged;
}

void Player::charge(int amount) {
    if (balance < amount) {
        throw NotEnoughMoneyException{};
    } else {
        minusBalance(amount);
    }
}

void Player::addMortgaged(PayableBuilding* building) {
    mortgaged.emplace_back(building);
}

std::vector<PayableBuilding*> Player::getOwned() {
    return owned;
}

void Player::transferAssets(Player * win) {
    win->addBalance(balance);
    for (auto i : owned) {
        i->setOwner(win);
        win->addOwned(i);
    }
    owned.clear();
    for (auto i : mortgaged) {
        i->setOwner(win);
        win->addMortgaged(i);
    }
    mortgaged.clear();
}

void Player::transferToBank() {
    balance = 0;
    for (auto i : owned) {
        i->setOwner(nullptr);
    }
    owned.clear();
    for (auto i : mortgaged) {
        i->setOwner(nullptr);
    }
    mortgaged.clear();
}

int Player::worth() {
    int totalValue = balance;
    for (auto b : owned) {
        totalValue += b->getPurchaseCost();
        auto temp = dynamic_cast<AcademicBuilding*>(b);
        if (temp) {
            totalValue += temp->getImproveCost() * temp->getLevel();
        }
    }
    return totalValue;
}

Player::~Player() {}
