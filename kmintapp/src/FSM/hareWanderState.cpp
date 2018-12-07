#include "hareWanderState.h"
#include "hare.h"

hareWanderState::hareWanderState(hare* hare) : hare_{hare}
{
}

void hareWanderState::update(kmint::delta_time deltaTime)
{
    state::update(deltaTime);

    if (kmint::to_seconds(elapsedTime_) >= 1) {
        // pick random edge
        int next_index = kmint::random_int(0, hare_->node().num_edges());
        hare_->node(hare_->node()[next_index].to());
        elapsedTime_ = kmint::from_seconds(0);
    }
}
