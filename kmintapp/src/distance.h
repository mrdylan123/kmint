#pragma once
#include "kmint/map/map.hpp"

class distance
{
public:
	distance();

	int shortest_distance() const;
	void set_shortest_distance(int shortest_distance);

	kmint::map::map_node* from_node() const;
	void set_from_node(kmint::map::map_node* basic_edges);
private:
	int shortest_distance_;
	kmint::map::map_node* from_node_;
};
