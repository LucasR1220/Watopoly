#include "textDisplay.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <utility>
#include "player.h"

using namespace std;

TextDisplay::TextDisplay(Board const* board, std::vector<std::shared_ptr<Player>> players) : players{std::move(players)} {
    vector<cellModel> top;
    vector<cellModel> bottom;
    int id = 0;
    for (int i = 20; i <= 30; i++) {
        top.push_back(cellModel{board->getCellAt(i), i});
    }
    displayModel.push_back(top);

    for (int i = 0; i < 9; i++) {
        vector<cellModel> row;
        row.push_back(cellModel{board->getCellAt(19 - i), 19 - i});
        for (int j = 0; j < 9; j++) {
            row.push_back(cellModel{nullptr, -1});
        }
        row.push_back(cellModel{board->getCellAt(31 + i), 31 + i});
        displayModel.push_back(row);
    }

    for (int i = 10; i >= 0; i--) {
        bottom.push_back(cellModel{board->getCellAt(i), i});
    }

    displayModel.push_back(bottom);

}

vector<Player*> TextDisplay::getPlayerAt(int position) const {
    vector<Player*> res;
    for (int i = 0; i < players.size(); i++) {
        if (players.at(i)->getPosition() == position) {
            res.push_back(players.at(i).get());
        }
    }
    return res;
}

void printCellName(string name, int level) {
    if (name.size() < 8) {
        if (level == 1) {
            cout << string(8, ' ') << '|';
            return;
        }
        cout << left << setw(8) << name.substr(0, 8) << '|';
    } else {
        cout << left << setw(8) << name.substr(8 * level, 8) << '|';
    }
}

void TextDisplay::print() const {
    int cellHeight = 5;
    int cellWidth = 8;
    cout << string((cellWidth + 1) * 11 + 1, '_') << endl;
    for (int i = 0; i < 11 * cellHeight; i++) {
        cout << '|';
        for (int j = 0; j < 11; j++) {
            int y = i % cellHeight;
            if (displayModel[i / cellHeight][j].cell == nullptr) {
                // draw bottom inner edge
                if (i == cellHeight * 10 - 1 && j != 0 && j != 11) {
                    cout << string(9, '_');
                } else if (j == 9 && i / cellHeight != 0 && i / cellHeight != 10) { // draw right inner edge
                    cout << string(8, ' ');
                    cout << '|';
                } else {
                    cout << string(9, ' ');
                }
                continue;
            }
            string name = displayModel[i / cellHeight][j].cell->getName();
            if (displayModel[i / cellHeight][j].cell->getType() == Cell::Academic) {
                switch (y) {
                    case 0: {
                        // print the details of an academic building
                        AcademicBuilding* academicBuilding =
                                dynamic_cast<AcademicBuilding*>(displayModel[i / cellHeight][j].cell.get());
                        if (academicBuilding) {
                            if (academicBuilding->getOwner()) {
                                // TODO change to I
                                cout << academicBuilding->getOwner()->getSymbol() << " lv."
                                    << academicBuilding->getLevel() << string(2, ' ') << '|';
                            } else {
                                cout << string(8, ' ') << '|';
                            }
                        }
                        break;
                    }
                    case 1: cout << string(8, '-') << '|'; break;
                    case 2: printCellName(name, 0); break;
                    case 3: {
                        vector<Player*> playersHere = getPlayerAt(displayModel[i / cellHeight][j].id);
                        int spacesLeft = 8;
                        for (auto p : playersHere) {
                            cout << p->getSymbol();
                            spacesLeft--;
                        }
                        cout << string(spacesLeft, ' ') << '|';
                        break;
                    }
                    case 4: cout << string(8, '_') << '|'; break;
                }
            } else {
                switch (y) {
                    case 0: printCellName(name, 0); break;
                    case 1: printCellName(name, 1); break;
                    case 2: {
                        if (displayModel[i / cellHeight][j].cell->getOwner()) {
                            cout << "Owner: " << displayModel[i / cellHeight][j].cell->getOwner()->getSymbol()
                                 << '|';
                        } else {
                            cout << string(8, ' ') << '|';
                        }
                        break;
                    }
                    case 3: {
                        vector<Player*> playersHere = getPlayerAt(displayModel[i / cellHeight][j].id);
                        int spacesLeft = 8;
                        for (auto p : playersHere) {
                            cout << p->getSymbol();
                            spacesLeft--;
                        }
                        cout << string(spacesLeft, ' ') << '|';
                        break;
                    }
                    case 4: cout << string(8, '_') << '|'; break;
                }
            }
        }
        cout << endl;
    }
}

TextDisplay::~TextDisplay() {}
