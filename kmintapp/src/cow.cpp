#include "cow.h"
#include "kmint/random.hpp"
#include "kmint/graph/graph.hpp"
#include "kmint/map/map.hpp"
#include "kmint/play.hpp"
#include "FSM/cowWanderState.h"
using namespace kmint;

static const char *cow_image = "resources/cow.png";

kmint::graph::basic_graph<kmint::map::map_node_info>::node_type const& random_pill_node(map::map_graph &graph) {
    int r = kmint::random_int(0, graph.num_nodes() - 1);
    for (std::size_t i = 0; i < graph.num_nodes(); ++i) {
        if (graph[i].node_info().kind == 'G') {
            if (r == 0)
            {
                graph[i].tagged(true);
                return graph[i];
            }
            else
            {
                --r;
            }
        }
    }
    throw "could not find node for hare";
}

cow::cow(map::map_graph &g, map::map_node const &initial_node) : kmint::play::map_bound_actor{ g, initial_node }, drawable_{ *this,
        kmint::graphics::image{
        cow_image, 0.1f } }, reachedHare_{ false }, pillNode{ &random_pill_node(g) }
{
    currentState_.push(std::make_unique<cowWanderState>(this));
}

void cow::act(delta_time dt) {
    currentState_.top()->update(dt);

    //    t_passed_ += dt;
    //
    //    if (shortestPath_.empty())
    //    {
    //	reachedHare_ = true;
    //	return;
    //    };
    //
    //    graph::basic_node<map::map_node_info>* nextNode = shortestPath_.top();
    //
    //    auto& currentNode = node();
    //
    //    edge_type* nextEdge = nullptr;
    //
    //    for (auto& edge : *nextNode)
    //    {
    //	if (edge.from().node_id() == nextNode->node_id() || edge.to().node_id() == nextNode->node_id())
    //	{
    //	    nextEdge = &edge;
    //	}
    //    }
    //
    //    if (to_seconds(t_passed_) >= nextEdge->weight() * 0.2)
    //    {
    //	this->node(*nextNode);
    //	shortestPath_.pop();
    //	t_passed_ = from_seconds(0);
    //    }
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

void cow::walkShortestPath(kmint::delta_time deltaTime)
{
        t_passed_ += deltaTime;
    
        if (shortestPath_.empty()) // if (shortestPath_.top() == endNode ( reachedEnd_ = true;
        {
    	//reachedHare_ = true;
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

void cow::pushState(std::unique_ptr<state> state)
{
    if (currentState_.top().get() != state.get())
        currentState_.push(std::move(state));
}
