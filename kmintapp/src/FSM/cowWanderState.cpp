#include "cowWanderState.h"
#include "kmint/random.hpp"

cowWanderState::cowWanderState(cow* cow) : cow_{ cow }
{
}

void cowWanderState::update(kmint::delta_time deltaTime)
{
	state::update(deltaTime);

	if (kmint::to_seconds(elapsedTime_) >= 1) {
		// pick random edge
		int next_index = kmint::random_int(0, cow_->node().num_edges());
		cow_->node(cow_->node()[next_index].to());
		elapsedTime_ = kmint::from_seconds(0);
	}
}
