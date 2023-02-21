#include "watopoly.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include "board.h"
#include "player.h"
#include "dice.h"
#include "textDisplay.h"
#include "tims.h"
#include "notEnoughMoneyException.h"
#include "loadSave.h"

using namespace std;

// helper function for command input
void allCommands(Player* pName, Cell* building, std::vector<std::shared_ptr<Player>> players, string order[]);
void tradeFunc(Player* pName, std::vector<std::shared_ptr<Player>> players, string order[], Board& gameBoard);
int timsRoll(bool * doubles);
bool isDigit(string str);
void declareBankrupt(Player* pName, std::vector<std::shared_ptr<Player>> players, string order[], Board& gameBoard);
void auction(Cell * building, std::vector<std::shared_ptr<Player>> players, Player * current);
void auction(Player * current, std::vector<std::shared_ptr<Player>> players);
void auctionBuilding(PayableBuilding * building, std::vector<std::shared_ptr<Player>> players, Player * current);

Watopoly::Watopoly(bool testing, std::string fileName) : testing{testing}, fileName{fileName}, ls{} {}

Watopoly::~Watopoly() {}

void Watopoly::play() {
    string cmd; // command entry
    int intCmd; // integer command entry
    char charCmd; // char entry
    bool loadedGame = false; // whether or not they load a game
    int playerCount; // the player count
    string tempNames[8]; // holds names of players
    string order[8]; // holds dice roll numbers
    std::vector<std::shared_ptr<Player>> players;
    int orderRolls[8]; // order deciding rolls result
    int playerTurn = 0; // player turn counter
    int roll1 = 0; // first die
    int roll2 = 0; // second die
    bool doubles = false; // rolled doubles?
    int rollResult = 0; // total amount from rolls
    bool sameName = false; // check for same name flag
    int chosen[3] = {0,0,0}; // house rule entries
    int bankruptCount = 0; // number of players eliminated
    char pieces[8] = {'G', 'B', 'D', 'P', 'S', '$', 'L', 'T'}; // all pieces
    int piecesTaken[8] = {0}; // pieces chosen
    int remainingPlayers = 0; // remaining players
    Dice myDice{};
    bool pieceChosen = false; // if player chose a piece succesfully
    bool pieceChooseAnother = false; // if player needs to pick a different piece
    bool buildingExist;
    if (testing)  cout << "Testing mode enabled" << endl;
    cout << "Welcome to Watopoly!" << endl;
    if (fileName == "") {
        cout << "Choose the number of players (enter a number 2-8): ";
        while (cin >> cmd) { 
            char temp = cmd[0];
            if (temp < '2' || temp > '8') { // if player count out of bounds
                cout << "Please choose a number between 2 and 8 (inclusive): ";
            } else { // player count valid
                playerCount = temp - '0';
                remainingPlayers = playerCount;
                break;
            }
        }
        if (cin.eof()) {
            return;
        }
        // initialize player objects
        for (int i = 0; i < playerCount; i++) { // selecting names
            cout << "Player " << (i+1) << " enter a name between 1 and 20 characters: ";
            while (cin >> cmd) {
                if (cmd.length() > 20 || cmd.length() < 1) { // check for invalid name length
                    cout << "Please enter a name between 1 and 20 characters: ";
                } else {
                    for (int j = 0; j < i; j++) { // check for repeated names
                        if (cmd == tempNames[j]) {
                            sameName = true;
                        }
                    }
                    if (sameName) {
                        cout << "Name taken, please choose a different name: ";
                        sameName = false;
                    } else {
                        tempNames[i] = cmd;
                        break;
                    }
                }
            }
            if (cin.eof()) {
                return;
            }
        }
        // rolling for order
        for (int j = 0; j < playerCount; j++) {
            cout << tempNames[j] << " please roll (type 'roll'): ";
            if (testing) {
                while (getline(cin, cmd, '\n')) {
                    if (cmd == "roll") {
                        orderRolls[j] = myDice.roll() + myDice.roll();
                        cout << orderRolls[j] << endl;
                        break;
                    } else {
                        istringstream iss {cmd};
                        int tRoll1, tRoll2;
                        iss >> cmd >> tRoll1 >> tRoll2;
                        if (tRoll1 > 6 || tRoll2 > 6 || tRoll1 < 1 || tRoll2 < 1) continue; 
                        orderRolls[j] = myDice.roll(tRoll1, tRoll2);
                        cout << orderRolls[j] << endl;
                        break;
                    } 
                }
            } else {
                while (cin >> cmd) {
                    if (cmd != "roll") { // if typo
                        cout << "Please type 'roll': ";
                        cin.clear();
                        cin.ignore();
                    } else {
                        orderRolls[j] = myDice.roll() + myDice.roll();
                        cout << orderRolls[j] << endl;
                        break;
                    }
                }
            }
            if (cin.eof()) {
                return;
            }
        }
        // placing names in order based on roll
        for (int m = 0; m < playerCount; m++) {
            int pos = 0;
            int max = 0;
            for (int n = 0; n < playerCount; n++) {
                if (orderRolls[n] > max) { // find the highest number
                    max = orderRolls[n];
                    pos = n;
                }
            }
            orderRolls[pos] = -1;
            order[m] = tempNames[pos];
        }
        // print names in order
        cout << "The player order is as following: ";
        for (int i = 0; i < playerCount; i++) {
            cout << order[i] << " ";
        }
        cout << endl;
        // choosing pieces
        cout << "Please choose a player piece" << endl;
        cout << "Goose - G" << endl;
        cout << "GRT Bus - B" << endl;
        cout << "Tim Hortons Doughnut - D" << endl;
        cout << "Professor - P" << endl;
        cout << "Student - S" << endl;
        cout << "Money - $" << endl;
        cout << "Laptop - L" << endl;
        cout << "Pink Tie - T" << endl;
        for (int i = 0; i < playerCount; i++) {
            cout << "Please choose a piece " << order[i] << ", enter the corresponding letter: "<< endl;
            while (cin >> charCmd) {
                for (int j = 0; j < 8; j++) {
                    if (charCmd == pieces[j] && piecesTaken[j] == 0) {
                        std::shared_ptr<Player> player = std::make_shared<Player>(pieces[j]);
                        players.emplace_back(player);
                        piecesTaken[j] = 1;
                        if (charCmd == 'G') {
                            player.get()->setName("Goose");
                        } else if (charCmd == 'B') {
                            player.get()->setName("GRT Bus");
                        } else if (charCmd == 'D') {
                            player.get()->setName("Tim Hortons Doughnut");
                        } else if (charCmd == 'P') {
                            player.get()->setName("Professor");
                        } else if (charCmd == 'S') {
                            player.get()->setName("Student");
                        } else if (charCmd == '$') {
                            player.get()->setName("Money");
                        } else if (charCmd == 'L') {
                            player.get()->setName("Laptop");
                        } else if (charCmd == 'T') {
                            player.get()->setName("Pink tie");
                        } 
                        pieceChosen = true;
                        break;
                    } else if (charCmd == pieces[j]) {
                        cout << "Piece already taken, choose another from" << endl;
                        for (int m = 0; m < 8; m++) {
                            if (piecesTaken[m] == 0) {
                                cout << pieces[m] << " ";
                            }
                        }
                        cout << endl;
                        pieceChooseAnother = true;
                        break;
                    }
                }
                if (pieceChosen) {
                    pieceChosen = false;
                    break;
                } else if (pieceChooseAnother) {
                    pieceChooseAnother = false;
                    continue;
                }
                cout << "Invalid piece, enter again: " << endl;
                cin.clear();
                cin.ignore();
            }
            if (cin.eof()) {
                return;
            }
            cin.clear();
            cin.ignore();
        }
    } else {
        LoadSave ls = load("save");
        gameBoard = ls.getBoard();
        players = ls.getPlayer();
    }
    TextDisplay displayBoard{&gameBoard, players};
    // players rolling now
    while (1) {
        if (players[playerTurn]->getBankrupt()) {
            playerTurn += 1;
            if (playerTurn == playerCount) {
                playerTurn = 0;
            }
            continue;
        }
        if (players[playerTurn]->getAtTims()) {
            if (players[playerTurn]->getRoundsAtTims() == 3) {
                rollResult = timsRoll(&doubles);
                if (cin.eof()) {
                    return;
                }
                if (doubles) {
                    doubles = false;
                } else {
                    cout << "You must pay 50$ or use a Roll Up the Rim Cup to get out" << endl;
                    cout << "You have $" << players[playerTurn]->getBalance() << endl;
                    cout << "You have " << players[playerTurn]->getTimsCup() << " Rim Cups" << endl;
                    if (players[playerTurn]->getBalance() < 50 && players[playerTurn]->getTimsCup() == 0) {
                        declareBankrupt(players[playerTurn].get(), players, order, gameBoard);
                        cout << "You can 'trade', 'mortgage', 'sell', declare 'bankrupt', or 'pay': ";
                        while (cin >> cmd) {
                            if (cmd == "trade") {
                                tradeFunc(players[playerTurn].get(), players, order, gameBoard);
                            } else if (cmd == "mortgage") {
                                cin >> cmd;
                                PayableBuilding * building = dynamic_cast<PayableBuilding*>(gameBoard.getCell(cmd).get());
                                if (building) players[playerTurn]->mortgage(building);
                            } else if (cmd == "bankrupt") {
                                if (players[playerTurn]->ownAnyBuildings()) {
                                    cout << "You still have owned properties" << endl;
                                    cout << "Enter an option: ";
                                } else {
                                    players[playerTurn]->setBankrupt(true);
                                }
                            } else {
                                cout << "enter a valid option listed above" << endl;
                            }
                        }
                    } else {
                        cout << "'pay' or use Rims 'cup': ";
                        gameBoard.getCellAt(10).get()->event(players[playerTurn].get(), cout, cin);
                    }
                }
                players[playerTurn]->setAtTims(false);
                players[playerTurn]->setRoundsAtTims(0);
                if (players[playerTurn]->getBankrupt()) {
                    bankruptCount += 1;
                    players[playerTurn]->setBankrupt(true);
                } else {
                    players[playerTurn]->move(rollResult);
                    cout << players[playerTurn]->getPosition() << endl;
                }
            } else {
                players[playerTurn]->setRoundsAtTims((players[playerTurn].get()->getRoundsAtTims()) + 1);
                rollResult = timsRoll(&doubles);
//                if (cin.eof()) {
//                    return;
//                }
                if (doubles) {
                    doubles = false;
                } else {
                    cout << "You must pay 50$ or use a Roll Up the Rim Cup to get out" << endl;
                    cout << "You have $" << players[playerTurn]->getBalance() << endl;
                    cout << "You have " << players[playerTurn]->getTimsCup() << " Rim Cups" << endl;
                    string cmd;
                    cout << "Pay 50$, use Roll Up the Rim Cup, or skip turn ('pay'/'cup'/'skip'): ";
                    while (cin >> cmd) {
                        if (cmd == "pay") {
                            if (players[playerTurn]->getBalance() >= 50) {
                                players[playerTurn]->minusBalance(50);
                            } else {
                                cout << "Not enough balance" << endl;
                                cout << "Choose to use a 'cup': " << endl;
                            }
                        } else if (cmd == "cup") {
                            if (players[playerTurn]->getTimsCup()) {
                                players[playerTurn]->remTimsCup();
                                break;
                            } else {
                                cout << "You don't have a Rim Cup" << endl;
                                cout << "Choose to 'pay': ";
                            }
                        } else if (cmd == "skip") {
                            players[playerTurn]->setBankrupt(true);
                        } else {
                            cout << "enter one of the three options 'pay'/'cup'/'skip'" << endl;
                        }
                    }
                }
                if (!(players[playerTurn]->getBankrupt())) {
                    players[playerTurn]->setAtTims(false);
                    players[playerTurn]->setRoundsAtTims(0);
                    players[playerTurn]->move(rollResult);
                } else {
                    players[playerTurn]->setBankrupt(false);
                }
            }
        } else {
            cout << players[playerTurn]->getSymbol() << " please roll (use 'roll'): ";
            while (getline(cin, cmd, '\n')) {
                istringstream iss {cmd};
                string firstWord;
                iss >> firstWord;
                if (firstWord != "roll") { // if typo
                    cout << "Please type 'roll': ";
                } else {
                    if (testing && cmd != "roll") {
                        int tRoll1, tRoll2;
                        iss >> tRoll1 >> tRoll2;
                        if (tRoll1 > 6 || tRoll2 > 6 || tRoll1 < 1 || tRoll2 < 1) {
                            cout << "roll out of bounds, enter again" << endl;
                            continue; 
                        }
                        if (tRoll1 == tRoll2) {
                            cout << "Doubles! You get to roll again after typing next" << endl;
                            doubles = true;
                        }
                        cout << "You rolled " << tRoll1 + tRoll2 << endl;
                        players[playerTurn]->move(myDice.roll(tRoll1, tRoll2));
                        break;
                    } else {
                        roll1 = myDice.roll();
                        roll2 = myDice.roll();
                        rollResult = roll1 + roll2;
                        if (roll1 == roll2) {
                            cout << "Doubles! You get to roll again after typing next" << endl;
                            doubles = true;
                        } 
                        cout << "You rolled " << rollResult << endl;
                        players[playerTurn]->move(rollResult);
                        break;
                    }
                }
            }
            if (cin.eof()) {
                break;
            }
        }
        int pPos = players[playerTurn]->getPosition();
        displayBoard.print();
        while (true) {
            try {
                int positionBeforeEvent = players[playerTurn]->getPosition();
                gameBoard.getCellAt(pPos)->event(players[playerTurn].get(), cout, cin);
                displayBoard.print();
                if (positionBeforeEvent != players[playerTurn]->getPosition()) {
                    gameBoard.getCellAt(players[playerTurn]->getPosition())
                        ->event(players[playerTurn].get(), cout, cin);
                }
            } catch (NotEnoughMoneyException & e) {
                declareBankrupt(players[playerTurn].get(), players, order, gameBoard);
                if (players[playerTurn]->getBankrupt()) {
                    int tempPos = players[playerTurn]->getPosition();
                    if (gameBoard.getCellAt(tempPos).get()->getType() == Cell::Academic || 
                        gameBoard.getCellAt(tempPos).get()->getType() == Cell::NonAcademic) {
                        Player * causePlayer = gameBoard.getCellAt(tempPos).get()->getOwner();
                        players[playerTurn].get()->transferAssets(causePlayer);
                    } else {
                        players[playerTurn].get()->transferToBank();
                    }
                    break;
                }
                continue;
            }
            break;
        }
        pPos = players[playerTurn]->getPosition();
        if (gameBoard.getCellAt(pPos)->getOwner() == nullptr && 
            gameBoard.getCellAt(pPos)->getType() != Cell::NonPayable) {
            auction(gameBoard.getCellAt(pPos).get(), players, players[playerTurn].get());
        }
        if (!(players[playerTurn]->getBankrupt())) {
            allCommands(players[playerTurn].get(), gameBoard.getCellAt(pPos).get(), players, order);
        } else {
            playerCount -= 1;
            auction(players[playerTurn].get(), players);
        }
        if (remainingPlayers == 1) {
            for (int i = 0; i < playerCount; i++) {
                if (!(players[i]->getBankrupt())) {
                    cout << players[i] << " is the winner!";
                    return;
                }
            }
        }
        if (doubles == true && !(players[playerTurn]->getBankrupt())) {
            doubles = false;
        } else {
            playerTurn += 1;
            if (playerTurn == playerCount) {
                playerTurn = 0;
            }
        }
    }
    return;
}

int timsRoll(bool * doubles) {
    Dice myDice{};
    int roll1 = 0;
    int roll2 = 0;
    int rollResult = 0;
    string cmd;
    cout << "Please type 'roll': ";
    while (cin >> cmd) {
        if (cmd != "roll") { // if typo
            cout << "Please type 'roll': ";
        } else {
            roll1 = myDice.roll();
            roll2 = myDice.roll();
            cout << "You rolled " << roll1 << " and " << roll2 << endl;
            rollResult = roll1 + roll2;
            if (roll1 == roll2) {
                *doubles = true;
            }
            break;
        }
    }
    return rollResult;
}

bool checkBuildingExist(string name) {
    if (name == "AL" || name == "ML" || name == "ECH" || name == "PAS" || name == "HH" ||
        name == "RCH" || name == "DWE" || name == "CPH" || name == "LHI" || name == "BMH" ||
        name == "OPT" || name == "EV1" || name == "EV2" || name == "EV3" || name == "PHYS" ||
        name == "B1" || name == "B2" || name == "EIT" || name == "ESC" || name == "C2" ||
        name == "MC" || name == "DC") {
            return true;
    } else {
        return false;
    }
}

//helper function for command input
void Watopoly::allCommands(Player* pName, Cell* building, std::vector<std::shared_ptr<Player>> players, string order[]) {
    string cmd;
    cout << "please enter a command (enter 'commands' to see all commands): ";
    while (cin >> cmd) {
        if (cmd == "next") {
            break;
        } else if (cmd == "trade") {
            tradeFunc(pName, players, order, gameBoard);
        } else if (cmd == "improve") {
            // improve current property
            Cell *targetBuilding;
            cout << "Enter a building name: ";
            cin >> cmd;
            if (checkBuildingExist(cmd)) {
                targetBuilding = gameBoard.getCell(cmd).get();
            } else {
                targetBuilding = nullptr;
            }
            if (targetBuilding != nullptr && targetBuilding->getType() == Cell::Academic) {
                AcademicBuilding *temp = dynamic_cast<AcademicBuilding*>(targetBuilding);
                pName->improve(temp);
            } else {
                cout << "Not a valid building name" << endl;
            }
        } else if (cmd == "mortgage") {
            Cell *targetBuilding;
            cout << "Enter a building name: ";
            cin >> cmd;
            PayableBuilding * building = dynamic_cast<PayableBuilding*>(gameBoard.getCell(cmd).get());
            if (building) pName->mortgage(building);
        } else if (cmd == "unmortgage") {
            Cell *targetBuilding;
            cout << "Enter a building name: ";
            cin >> cmd;
            targetBuilding = gameBoard.getCell(cmd).get();
            if (targetBuilding != nullptr && (targetBuilding->getType() == Cell::Academic || 
                targetBuilding->getType() == Cell::NonAcademic)) {
                PayableBuilding *temp = dynamic_cast<PayableBuilding*>(targetBuilding);
                pName->unmortgage(temp);
            } else {
                cout << "Not a valid building name" << endl;
            }
        } else if (cmd == "assets") {
            pName->assets();
        } else if (cmd == "all") {
            // print all player assets
            for (auto i : players) {
                i.get()->assets();
            }
        } else if (cmd == "save") {
            LoadSave ls{"save", gameBoard, players};
            ls.save("save");
        } else if (cmd == "restart") {
            cout << "Are you sure you want to restart the game? (type 'accept' to restart): ";
            if (cmd == "accept") {
                //restart game
            }
        } else if (cmd == "commands") {
            cout << "next - begin next player's turn" << endl;
            cout << "trade - trade with another player, usage is, trade <name> <give> <receive>" << endl;
            cout << "improve - improve this property" << endl;
            cout << "mortgage - mortgage a property" << endl;
            cout << "unmortgage - unmortgage a property" << endl;
            cout << "assets - display your assets" << endl;
            cout << "all - display all player's assets" << endl;
            cout << "save - make sure you are done with your turn" << endl;
        } else {
            cout << "Command does not exist, use 'commands' to see available commands" << endl;
        }
        cout << "enter a command: ";
    }
}

void tradeFunc(Player* pName, std::vector<std::shared_ptr<Player>> players, string order[], Board& gameBoard) {
    string targetPlayer, read1, read2, result1, result2, cmd;
    int amount1, amount2;
    int pos = 0;
    bool num1 = false;
    bool num2 = false;
    bool match = false;
    Player * temp = nullptr;
    PayableBuilding * building1;
    PayableBuilding * building2;
    cin >> targetPlayer >> read1 >> read2;
    for (int i = 0; i < 8; i++) {
        if (order[i] == targetPlayer) {
            match = true;
            temp = players[pos].get();
            break;
        }
        pos += 1;
    }
    if (match) {
        cout << order[pos] << " do you accept this trade [yes/no]: ";
        cin >> cmd;
        if (cmd == "yes") {
            if (isDigit(read1)) {
                amount1 = stoi(read1);
                num1 = true;
            } else {
                building1 = dynamic_cast<PayableBuilding*>(gameBoard.getCell(read1).get());
            }
            if (isDigit(read2)) {
                amount2 = stoi(read2);
                num2 = true;
            } else {
                building2 = dynamic_cast<PayableBuilding*>(gameBoard.getCell(read2).get());
            }
            if (num1) {
                pName->trade(temp, amount1, building2);
            } else if (num1 && num2) {
                cout << "cannot trade currency for currency" << endl;
            } else if (num2) {
                pName->trade(temp, building1, amount2);
            } else {
                pName->trade(temp, building1, building2);
            }
        }
    } else {
        cout << "no player found, please enter valid player name" << endl;
        cin.clear();
    }
}

bool isDigit(string str) {
    for (char const &ch : str) {
        if (isdigit(ch) == 0) {
            return false;
        }
    }
    return true;
}

void declareBankrupt(Player* pName, std::vector<std::shared_ptr<Player>> players, string order[], Board& gameBoard) {
    string cmd;
    cout << "declare bankrupt or create some money (mortgage, trade, or sell)" << endl;
    cout << "enter 'bankrupt' to declare bankrupcy or 'pay' to pay the owed amount: ";
    while (cin >> cmd) {
        if (cmd == "mortgage") {
            Cell *targetBuilding = nullptr;
            cout << "Enter a building name: ";
            cin >> cmd;
            targetBuilding = gameBoard.getCell(cmd).get();
            if (targetBuilding != nullptr && (targetBuilding->getType() == Cell::Academic || 
                targetBuilding->getType() == Cell::NonAcademic)) {
                PayableBuilding *temp = dynamic_cast<PayableBuilding*>(targetBuilding);
                pName->mortgage(temp);
            } else {
                cout << "Not a valid building name" << endl;
            }
        } else if (cmd == "improve") {
            Cell *targetBuilding;
            cout << "Enter a building name: ";
            cin >> cmd;
            if (checkBuildingExist(cmd)) {
                targetBuilding = gameBoard.getCell(cmd).get();
            } else {
                targetBuilding = nullptr;
            }
            if (targetBuilding != nullptr && targetBuilding->getType() == Cell::Academic) {
                AcademicBuilding *temp = dynamic_cast<AcademicBuilding*>(targetBuilding);
                pName->improve(temp);
            } else {
                cout << "Not a valid building name" << endl;
            }
        } else if (cmd == "assets") {
            pName->assets();
        } else if (cmd == "trade") {
            tradeFunc(pName, players, order, gameBoard);
        } else if (cmd == "bankrupt") {
            pName->setBankrupt(true);
            break;
        } else if (cmd == "pay") {
            break;
        } else {
            cout << "Enter a valid command (mortgage, trade, assets, sell, bankrupt, or pay): ";
        }
    }
}

void auction(Cell * cell, std::vector<std::shared_ptr<Player>> players, Player * current) {
    PayableBuilding *building = dynamic_cast<PayableBuilding*>(cell);
    auctionBuilding(building, players, current);
}

void auction(Player * current, std::vector<std::shared_ptr<Player>> players) {
    for (auto i : current->getOwned()) {
        i->setOwner(nullptr);
        auctionBuilding(i, players, current);
    }
    for (auto t : current->getMortgaged()) {
        t->setOwner(nullptr);
    }
    current->getOwned().clear();
    current->getMortgaged().clear();
}

void auctionBuilding(PayableBuilding * building, std::vector<std::shared_ptr<Player>> players, Player * current) {
    int startingBid = building->getPurchaseCost();
    Player * higherBidder = nullptr;
    string cmd;
    int bid;
    int turnCount = 0;
    int playerCount = players.size();
    int remainingBidders = playerCount - 1;
    int withdrawn[8] = {0};
    while (1) {
        if (players[turnCount].get() == current || withdrawn[turnCount]) {
            turnCount += 1;
            if (turnCount == playerCount) {
                turnCount = 0;
            }
            continue;
        }
        cout << "Enter an amount you would like to pay or enter withdraw" << endl;
        cout << "Starting bid is " << startingBid << endl;
        cout << players[turnCount].get()->getSymbol() << ": ";
        while (cin >> cmd) {
            if (cmd == "withdraw") {
                remainingBidders -= 1;
                withdrawn[turnCount] = 1;
                break;
            } else if ((bid = stoi(cmd))) {
                if (bid > startingBid && players[turnCount].get()->getBalance() >= bid) {
                    startingBid = bid;
                    higherBidder = players[turnCount].get();
                    break;
                } else if (players[turnCount].get()->getBalance() < bid) {
                    cout << "Not enough funds" << endl;
                } else {
                    cout << "Bid too low" << endl;
                }
            } else {
                cout << "Invalid entry, try again: ";
            }
        }
        if (remainingBidders <= 1 && higherBidder != nullptr) {
            higherBidder->addBalance(-startingBid);
            building->setOwner(higherBidder);
            higherBidder->addOwned(building);
            break;
        } else if (remainingBidders <= 1) {
            cout << "No players purchased" << endl;
            break;
        }
        turnCount += 1;
        if (turnCount == playerCount) {
            turnCount = 0;
        }
    }
}
