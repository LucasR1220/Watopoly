#ifndef PAYABLEBUILDINGSTATE_H
#define PAYABLEBUILDINGSTATE_H

#include "player.h"

struct PayableBuildingState {
    enum Type {
        MONOPOLY_FORMED,
        MONOPOLY_UNFORMED,
        NEW_OWNER
    };
    PayableBuildingState::Type type;
};

#endif
