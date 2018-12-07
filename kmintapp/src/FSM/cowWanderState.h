#ifndef KMINTAPP_COWWANDERSTATE_H
#define KMINTAPP_COWWANDERSTATE_H
#include "State.h"
#include "../cow.h"

class cowWanderState : public state {
public:
	cowWanderState(cow* cow);
	void update(kmint::delta_time deltaTime) override;

private:
	cow* cow_;
    kmint::delta_time wanderingTime_;
};
#endif
