#include "State.h"

state::state() : elapsedTime_{ 0 }
{
}

void state::update(kmint::delta_time deltaTime)
{
    elapsedTime_ += deltaTime;
}
