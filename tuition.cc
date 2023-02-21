#include "tuition.h"
#include "player.h"
#include <string>
#include <iostream>

using namespace std;
Tuition::Tuition() : OtherNonPayableBuildings{"TUITION"} {}

Tuition::~Tuition() {}

void Tuition::event(Player* player, std::ostream& out, std::istream& in) {
    cout << "You landed on Tuition, You can either: " << endl;
    cout << "pay $300 [1]" << endl;
    cout << "10% of your assets [2]" << endl;
    int cmd;

    while (true) {
        cin >> cmd;
        if (cmd != 1 && cmd != 2) {
            cout << "Invalid input" << endl;
            continue;
        }
        if (cmd == 1) {
            player->charge(300);
            break;
        } else {
            player->charge(player->worth() / 10);
            break;
        }
    }
}
