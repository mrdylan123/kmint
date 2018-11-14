#include "distance.h"

distance::distance() : shortest_distance_(std::numeric_limits<int>::max()), from_node_{ nullptr }
{
}

int distance::shortest_distance() const
{
	return shortest_distance_;
}

void distance::set_shortest_distance(int shortest_distance)
{
	shortest_distance_ = shortest_distance;
}

kmint::map::map_node* distance::from_node() const
{
	return from_node_;
}

void distance::set_from_node(kmint::map::map_node* basic_edges)
{
	from_node_ = basic_edges;
}
