#ifndef KMINTAPP_SEARCHPILLSTATE_H
#define KMINTAPP_SEARCHPILLSTATE_H
#include "State.h"
#include "kmint/map/map.hpp"

class cow;

class searchPillState : public state {
public:
    searchPillState(cow* cow);
    void update(kmint::delta_time deltaTime) override;

private:
    cow* cow_;
};
#endif
