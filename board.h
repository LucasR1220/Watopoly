#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <memory>
#include <string>
#include "academicBuilding.h"
class Cell;

class Board {
    std::vector<std::shared_ptr<Cell>> cells;
    void addAcademicBuilding(int i);
    std::string names[22];
    int academicBuildingInfo[22][8];
    AcademicBuilding::Block blocks[22];

public:
    Board();
    ~Board();
    std::shared_ptr<Cell> getCellAt(int index) const;
    std::shared_ptr<Cell> getCell(std::string name) const;
};

#endif
