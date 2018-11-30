#include "cow.h"
#include "kmint/random.hpp"
#include "kmint/graph/graph.hpp"
#include "kmint/map/map.hpp"
#include "kmint/play.hpp"
using namespace kmint;

static const char *cow_image = "resources/cow.png";
cow::cow(map::map_graph const &g, map::map_node const &initial_node) : kmint::play::map_bound_actor{ g, initial_node }, drawable_{ *this,
	kmint::graphics::image{
	cow_image, 0.1f } }, reachedHare_{ false } {}

void cow::act(delta_time dt) {
    t_passed_ += dt;

    if (shortestPath_.empty())
    {
	reachedHare_ = true;
	return;
    };

    graph::basic_node<map::map_node_info>* nextNode = shortestPath_.top();

    auto& currentNode = node();

    edge_type* nextEdge = nullptr;

    for (auto& edge : *nextNode)
    {
	if (edge.from().node_id() == nextNode->node_id() || edge.to().node_id() == nextNode->node_id())
	{
	    nextEdge = &edge;
	}
    }

    if (to_seconds(t_passed_) >= nextEdge->weight() * 0.2)
    {
	this->node(*nextNode);
	shortestPath_.pop();
	t_passed_ = from_seconds(0);
    }
}

bool cow::reached_hare() const
{
    return reachedHare_;
}

void cow::set_shortest_path(std::stack<kmint::graph::basic_node<kmint::map::map_node_info>*> shortest_path)
{
    reachedHare_ = false;
    shortestPath_ = shortest_path;
    shortestPath_.pop();
}
