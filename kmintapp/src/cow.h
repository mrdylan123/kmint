#ifndef KMINTAPP_COW_HPP
#define KMINTAPP_COW_HPP

#include "kmint/map/map.hpp"
#include "kmint/play.hpp"
#include "kmint/primitives.hpp"
#include <stack>
#include "kmint/graph/graph.hpp"
#include "FSM/State.h"

class cow : public kmint::play::map_bound_actor {
public:
	cow(kmint::map::map_graph &g, kmint::map::map_node const &initial_node);
	// wordt elke game tick aangeroepen
	void act(kmint::delta_time dt) override;
	kmint::ui::drawable const &drawable() const override { return drawable_; }
	// als incorporeal false is, doet de actor mee aan collision detection
	bool incorporeal() const override { return false; }
	// geeft de radius van deze actor mee. Belangrijk voor collision detection
	kmint::scalar radius() const override { return 16.0; }

    kmint::graph::basic_graph<kmint::map::map_node_info>::node_type const* pill_node() const { return pillNode; }

	bool reached_hare() const;
	void set_shortest_path(std::stack<kmint::graph::basic_node<kmint::map::map_node_info>*> shortest_path);
    void walkShortestPath(kmint::delta_time deltaTime);

    void pushState(std::unique_ptr<state> state);

private:
	// hoeveel tijd is verstreken sinds de laatste beweging
	kmint::delta_time t_passed_{};
	// weet hoe de koe getekend moet worden
	kmint::play::image_drawable drawable_;
	bool reachedHare_;
	std::stack<kmint::graph::basic_node<kmint::map::map_node_info>*> shortestPath_;
	kmint::map::map_node_info* next_node_{nullptr};
    kmint::graph::basic_graph<kmint::map::map_node_info>::node_type const* pillNode{ nullptr };
	edge_type const *pick_next_edge();

	std::stack<std::unique_ptr<state>> currentState_;
};

#endif /* KMINTAPP_COW_HPP */#pragma once
