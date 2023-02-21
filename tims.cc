#include "tims.h"
#include "player.h"
#include <string>
#include <iostream>
using namespace std;

Tims::Tims(string name) : OtherNonPayableBuildings{name} {}

Tims::~Tims() {}

void Tims::event(Player * pName, std::ostream& out, std::istream& in) {
    string cmd;
    cout << "Pay 50$, use Roll Up the Rim Cup, or skip turn ('pay'/'cup'/'skip'): ";
    while (cin >> cmd) {
        if (cmd == "pay") {
            if (pName->getBalance() >= 50) {
                pName->minusBalance(50);
            } else {
                cout << "Not enough balance" << endl;
                cout << "Choose to use a 'cup': " << endl;
            }
        } else if (cmd == "cup") {
            if (pName->getTimsCup()) {
                pName->remTimsCup();
                break;
            } else {
                cout << "You don't have a Rim Cup" << endl;
                cout << "Choose to 'pay': ";
            }
        } else if (cmd == "skip") {
            pName->setBankrupt(true);
        } else {
            cout << "enter one of the three options 'pay'/'cup'/'skip'" << endl;
        }
    }
}
