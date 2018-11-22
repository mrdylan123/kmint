#pragma once

namespace kmint {
	namespace map {
		struct map_node_info;
	}
}

namespace kmint::graph
{
	template <typename NodeInfo>
	class basic_node;
}


class distance
{
public:
	distance();

	int shortest_distance() const;
	void set_shortest_distance(int shortest_distance);

	kmint::graph::basic_node<kmint::map::map_node_info> const* from_node() const;
	void set_from_node(kmint::graph::basic_node<kmint::map::map_node_info> const* basic_edges);
private:
	int shortest_distance_;
	kmint::graph::basic_node<kmint::map::map_node_info> const* from_node_;
};
