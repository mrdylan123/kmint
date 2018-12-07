#include "searchPillState.h"
#include "aStar.h"
#include "cow.h"

searchPillState::searchPillState(cow* cow) : cow_{ cow }
{
    auto shortestPath = aStar::run(cow_->graph(), cow_->node(), *cow_->pill_node());

    cow_->set_shortest_path(shortestPath);
}

void searchPillState::update(kmint::delta_time deltaTime)
{
    cow_->walkShortestPath(deltaTime);
}
