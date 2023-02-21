#ifndef LOAD_SAVE_H
#define LOAD_SAVE_H

#include <vector>
#include "player.h"
#include "board.h"
#include <fstream>
#include <iostream>
#include <string>

class LoadSave {
    std::string filename;
    Board board;
    std::vector<std::shared_ptr<Player>> player;

    public:
        LoadSave();
        LoadSave(std::string filename, Board board, std::vector<std::shared_ptr<Player>> player);
        void save(std::string filename);
        std::string getFileName();
        Board getBoard();
        std::vector<std::shared_ptr<Player>> getPlayer();
};

LoadSave load(std::string filename);

#endif
