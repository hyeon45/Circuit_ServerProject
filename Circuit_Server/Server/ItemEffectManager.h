#pragma once
#include "WorldState.h"

class ItemEffectManager
{
public:
    ItemEffectManager(WorldState& world); 
    void ApplyItemEffect(int playerID, int itemID);

private:
    WorldState& worldState; 
};
