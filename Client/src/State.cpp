#include "State.h"

State::~State()
{
    
}

bool State::requestsExit() const
{
    return !active;
}

void State::exitState()
{
    active = false;
}