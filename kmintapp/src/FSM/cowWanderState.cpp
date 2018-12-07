#include "cowWanderState.h"
#include "kmint/random.hpp"
#include "searchPillState.h"

cowWanderState::cowWanderState(cow* cow) : cow_{ cow }, wanderingTime_{ 0 }
{
}

void cowWanderState::update(kmint::delta_time deltaTime)
{
	state::update(deltaTime);
    wanderingTime_ += deltaTime;

    if (kmint::to_seconds(wanderingTime_) >= 5)
    {
        cow_->pushState(std::make_unique<searchPillState>(cow_));
    }

	if (kmint::to_seconds(elapsedTime_) >= 1) {
		// pick random edge
		int next_index = kmint::random_int(0, cow_->node().num_edges());
		cow_->node(cow_->node()[next_index].to());
		elapsedTime_ = kmint::from_seconds(0);
	}
}
