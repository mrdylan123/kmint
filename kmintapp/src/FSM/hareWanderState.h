#ifndef KMINTAPP_HAREWANDERSTATE_H
#define KMINTAPP_HAREWANDERSTATE_H
#include "State.h"

class hare;

class hareWanderState : public state {
public:
    hareWanderState(hare* hare);
    void update(kmint::delta_time deltaTime) override;

private:
    hare* hare_;
};
#endif
