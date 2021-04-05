#include "State.h"

State::~State()
{
    
}

bool State::requireExit()
{
    return !active;
}
