#include "card.h"
#include <iostream>

using namespace std;

Card::Card(Card::Type type, int step, int amount) : step{step}, amount{amount}, actionType{type} {}

void Card::apply(Player& player) {
    if (actionType == Card::move) {
        player.move(step);
    }
    if (actionType == Card::money) {
        if (amount > 0) {
            player.addBalance(amount);
        } else {
            player.charge(amount);
        }
    }
    if (actionType == Card::osap) {
        player.addBalance(200);
    }
    if (actionType == Card::tims) {
        player.moveTo(10);
        player.setAtTims(true);
    }
}

void Card::what() {
    cout << "You drawed: ";
    if (actionType == Card::money) {
        cout << "Get $" << amount << endl;
    } else if (actionType == Card::move) {
        cout << "Move ";
        if (step > 0) cout << "forward " << step << " steps" << endl;
        else cout << "back " << -step << " steps" << endl;
    } else if (actionType == Card::tims) {
        cout << "Go to Tims" << endl;
    } else {
        cout << "Collect osap" << endl;
    }
}
