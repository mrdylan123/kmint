#include "cow.h"
#include "kmint/random.hpp"
#include "kmint/graph/graph.hpp"
#include "kmint/map/map.hpp"
#include "kmint/play.hpp"
using namespace kmint;

static const char *cow_image = "resources/cow.png";
cow::cow(map::map_graph const &g, map::map_node const &initial_node) : kmint::play::map_bound_actor{ g, initial_node }, drawable_{ *this,
	kmint::graphics::image{
	cow_image, 0.1f } } {}

void cow::act(delta_time dt) {
	t_passed_ += dt;
	if (to_seconds(t_passed_) >= 1) {
		// pick random edge
		int next_index = random_int(0, node().num_edges());
		this->node(node()[next_index].to());
		t_passed_ = from_seconds(0);
	}
}