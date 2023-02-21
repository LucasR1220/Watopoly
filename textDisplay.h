#ifndef TEXT_DISPLAY
#define TEXT_DISPLAY

#include <vector>
#include "cell.h"
#include "board.h"
#include <memory>
#include <map>

class TextDisplay {
    struct cellModel {
        std::shared_ptr<Cell> cell;
        int id;
    };
    std::vector<std::vector<cellModel>> displayModel;
    std::vector<std::shared_ptr<Player>> players;

    std::vector<Player*> getPlayerAt(int position) const;
public:
    TextDisplay(Board const* board, std::vector<std::shared_ptr<Player>> players);
    void print() const;
    ~TextDisplay();
};

#endif
