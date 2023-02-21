#ifndef CELL_H
#define CELL_H

#include <string>
#include <vector>

class Player;

class Cell {
public:
    enum Type {Academic, NonAcademic, NonPayable};
    Cell(std::string name);
    virtual ~Cell() = 0;
    std::string getName();
    void setType(Type type);
    Type getType() const;
    virtual void event(Player* player, std::ostream& out, std::istream& in) = 0;
    virtual void setOwner(Player* newOwner) = 0;
    virtual Player* getOwner() = 0;
protected:
    std::string name;
    Type type = NonAcademic;
    std::vector<Player*> players;
    Player* owner;
};

#endif
