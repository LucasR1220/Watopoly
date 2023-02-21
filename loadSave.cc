#include <iostream>
#include "loadSave.h"
//#include "watopoly.h"
#include <vector>
#include "player.h"
#include "loadSave.h"
#include <memory>
#include <cstring>
#include <sstream>
using namespace std;

LoadSave::LoadSave(): filename{""}, board{Board()}, player{nullptr}{}
LoadSave::LoadSave(string filename, Board board, vector<shared_ptr<Player>> player): filename{filename}, board{board}, player{player}{}

bool isMortgaged(vector<PayableBuilding*> mortgaged, string name){
    for (int i = 0; i < mortgaged.size(); ++i){
        if (mortgaged[i]->getName() == name){
            return true;
        }
    }
    return false;
}
void LoadSave::save(string filename){
    ofstream myfile(filename);
    myfile << player.size() << endl;

    for (int i = 0; i < player.size(); ++i){
        myfile << "player" << i+1 << " ";
        myfile << player[i].get()->getSymbol() << " ";      //get the char that represents the player
        
        myfile << player[i].get()->getTimsCup() << " "; 
        
        myfile << player[i].get()->getBalance() << " ";
        myfile << player[i].get()->getPosition() << " "; // get the position of the player;

        if (player[i].get()->getPosition() == 10){
            //cout << player[i].get()->getAtTims() << endl;
            if (!(player[i].get()->getAtTims())){
                myfile << "0";
            } //pass by tims
            else {
                myfile << "1 ";
                myfile << player[i].get()->getRoundsAtTims();//number of round needed until move
            }
        }
        myfile << endl;
    }

    for (int i = 0; i < 40; ++i){
        string str = board.getCellAt(i).get()->getName();
        while (true){
            size_t found = str.find(' ');
            if (found == string::npos){
                break;
            } else {
                str[found] = '_';
            }
        }
        myfile << str << " ";
        //cout << "Building: " << board.getCellAt(i).get()->getName() << " " << endl;
        if (board.getCellAt(i).get()->getOwner() == nullptr){
            //cout << "Owner: " << "Bank " << endl;
            myfile << "BANK"<< " ";
            myfile << "0" << endl;
        } else {
            //cout << "Owner: " << board.getCellAt(i).get()->getOwner()->getSymbol() << endl;
            myfile << board.getCellAt(i).get()->getOwner()->getSymbol() << " ";
            if (isMortgaged(board.getCellAt(i).get()->getOwner()->getMortgaged(), board.getCellAt(i).get()->getName())){
                myfile  << "-1" << endl;
            } else {
                if (auto ab = dynamic_cast<AcademicBuilding*>(board.getCellAt(i).get())){
                    //cout <<12412 << endl; 
                    myfile << ab->getLevel() << endl;
                } else {
                    myfile << "0 " << endl;
                }
            }
        }

        //cout << endl;
    }

    myfile.close();
}

Player& getPlayer(char symbol, vector<shared_ptr<Player>> player){
    for (int i = 0; i < player.size(); ++i){
        if (player[i].get()->getSymbol() == symbol){
            return *(player[i].get());
        }
    }

    return *(player[0].get());
}

LoadSave load(string filename){
    ifstream f1 {filename};

    int playerNum;
    f1 >> playerNum;
    ifstream f {filename};
    
    //cout << playerNum << " ";
    vector<shared_ptr<Player>> player;

    //cout << 12412 << endl;

    string input;
    getline(f, input);
    int count = 0;
    for (int i = 0; i < playerNum; ++i){
        //istringstream text {f};
        getline(f, input);
        //cout << 12412 << endl;
        //cout << input << endl;
        istringstream info {input};
        string playerName;
        info >> playerName; 
        //cout << playerName << " ";
        char symbol;
        info >> symbol;
        //cout << symbol << " ";
        int TimsCups;
        info >> TimsCups;
        //cout << TimsCups << " ";
        int money;
        info >> money;
        //cout << money << " ";
        bool bankrupt;
        if (money >= 0){
            bankrupt = false;
        } else {
            bankrupt = true;
        }
        
        int position;
        info >> position;
        //cout << position << " ";
        string name;
        switch (symbol){
            case 'G': name = "Goose"; break;
            case 'B': name = "GRT Bus"; break;
            case 'D': name = "Tim Hortons Doughnut"; break;
            case 'P': name = "Professor"; break;
            case 'S': name = "Student"; break;
            case '$': name = "Money"; break;
            case 'L': name = "Laptop"; break;
            case 'T': name = "Pink tie"; break;
        }
        int tims;
        //int count = 0;
        if (info >> tims){
            //cout << tims << " ";
            if (tims == 0){  //passby tims
                //cout << endl;
                vector<PayableBuilding*> owned;
                vector<PayableBuilding*> mortgaged;
                //auto sp1 = make_shared<Song>(L"The Beatles", L"Im Happy Just to Dance With You");
                shared_ptr<Player> p = make_shared<Player>(money, bankrupt, position, symbol, TimsCups, false, 0, owned, mortgaged, name);
                //cout << "player: " << p.get()->getBalance() << endl;
                //shared_ptr<Player> p(Player(money, bankrupt, position, symbol, TimsCups, false, 0, owned, mortgaged, name));
                player.emplace_back(p);
                //player.emplace_back(make_shared<Player>(money, bankrupt, position, symbol, TimsCups, false, 0, owned, mortgaged, name));
            } else {    //at tims
                count ++;
                int time;
                info >> time;
                //cout << tims << endl;
                vector<PayableBuilding*> owned;
                vector<PayableBuilding*> mortgaged;
                shared_ptr<Player> p = make_shared<Player>(money, bankrupt, position, symbol, TimsCups, true, time, owned, mortgaged, name);
                //cout << "player: " << p.get()->getBalance() << endl;
                player.emplace_back(p);
            }
            
        } else{
            //cout << endl;
            vector<PayableBuilding*> owned;
            vector<PayableBuilding*> mortgaged;
            shared_ptr<Player> p = make_shared<Player>(money, bankrupt, position, symbol, TimsCups, false, 0, owned, mortgaged, name);
            //cout << "player: " << p.get()->getBalance() << endl;
            //shared_ptr<Player> p(Player(money, bankrupt, position, symbol, TimsCups, false, 0, owned, mortgaged, name));
            player.emplace_back(p);
            //player.emplace_back(make_shared<Player>(money, bankrupt, position, symbol, TimsCups, false, 0, owned, mortgaged, name));
        }
        
        
    }
    

    //cout << 122314 << endl;
    Board b = Board();
    while (getline(f, input)){
        istringstream info {input}; 
        string name;
        string owner;
        int improvement;
        info >> name;
        info >> owner;
        info >> improvement;
        if (owner == "BANK"){
            continue;
        }
        const char* temp = owner.c_str();
        char newOwner = temp[0];
        //cout << "name: " << name << endl;
        //cout << "newOwner: " << newOwner << endl;
        //cout << "improvement: " << improvement << endl;
        //int count = 1;
        //cout << endl;
        while (true){
            size_t found = name.find('_');
                if (found == string::npos){
                    break;
                } else {
                    name[found] = ' ';
                }
            
        }
        for (int i = 0; i < 40; ++i){
            //cout << b.getCellAt(i).get()->getName() << endl;
            
            if (b.getCellAt(i).get()->getName() == name){
                
                if (improvement > 0){ 
                    //cout << 12142 << endl;          
                    getPlayer(newOwner, player).buy(dynamic_cast<PayableBuilding*>(b.getCellAt(i).get()), 0);
                    //cout << 12142 << endl;
                    dynamic_cast<AcademicBuilding*>(b.getCellAt(i).get())->setLevel(improvement);
                    break;
                }else if (improvement == 0){
                    getPlayer(newOwner, player).buy(dynamic_cast<PayableBuilding*>(b.getCellAt(i).get()), 0);
                } else {
                    getPlayer(newOwner, player).buy(dynamic_cast<PayableBuilding*>(b.getCellAt(i).get()), 0);
                    getPlayer(newOwner, player).mortgage(dynamic_cast<PayableBuilding*>(b.getCellAt(i).get()));
                    getPlayer(newOwner, player).setBalance(dynamic_cast<PayableBuilding*>(b.getCellAt(i).get())->getPurchaseCost() * -0.5);
                    break;
                }
                
            }
        }
        
        //cout << count++ << endl;
        
    }

    //cout << count << endl;
    f.close();
    return LoadSave{filename, b, player};
}
string LoadSave::getFileName(){
    return filename;
}

Board LoadSave::getBoard() {
    return board;
}

std::vector<std::shared_ptr<Player>> LoadSave::getPlayer() {
    return player;
}
