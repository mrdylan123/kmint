#include "kmint/main.hpp"          // voor de main loop
#include "kmint/graphics.hpp"      // kleuren en afbeeldingen
#include "kmint/math/vector2d.hpp" // voor window en app
#include "kmint/play.hpp"          // voor stage
#include "kmint/ui.hpp"            // voor window en app
#include "kmint/map/map.hpp"	   // Voor de kaart
#include <iostream>
#include "hare.h"
#include "cow.h"
#include <map>
#include <queue>
#include <stack>

using namespace kmint; // alles van libkmint bevindt zich in deze namespace

static const char *map_description = R"graph(32 24 32
resources/firstmap.png
G 1 1
C 1 1
H 1 1
W 0 0
B 1 8

WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
WWWHGGGGGGGGGGGGGGGGGGGGGGGGHWWW
WWWGGGGGGGGGGGGGGGGGGGGGGGGGGWWW
WWWGGGGGGGGGGGGGBGGGGGGGGGGGGWWW
WWWGGGGGGWWWWWWWBWWWWWWGGGGGGWWW
WWWGGGGGGWWWWWWWBWWWWWWGGGGGGWWW
WWWGGGGGGWWWWWWWBWWWWWWGGGGGGWWW
WWWGGGGGGWWWWWGGBGWWWWWGGGGGGWWW
WWWGGGGGGWWWWWGGGGWWWWWGGGGGGWWW
WWWGGGGGGWWWWWGGGGWWWWWGGGGGGWWW
WWWGGGGGBBBBBBBGCGGGGGGGGGGGGWWW
WWWGGGGGGWWWWWGGGGWWWWWGGGGGGWWW
WWWGGGGGGWWWWWGGBGWWWWWGGGGGGWWW
WWWGGGGGGWWWWWWWBWWWWWWGGGGGGWWW
WWWGGGGGGWWWWWWWBWWWWWWGGGGGGWWW
WWWGGGGGGWWWWWWWBWWWWWWGGGGGGWWW
WWWGGGGGGGGGGGGGBGGGGGGGGGGGGWWW
WWWGGGGGGGGGGGGGGGGGGGGGGGGGGWWW
WWWHGGGGGGGGGGGGGGGGGGGGGGGGHWWW
WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
)graph";

map::map m{ map::read_map(map_description) };

map::map_node &find_cow_node(map::map_graph &graph) {
    for (std::size_t i = 0; i < graph.num_nodes(); ++i) {
	if (graph[i].node_info().kind == 'C') {
	    return graph[i];
	}
    }
    throw "could not find starting point";
}

void dijkstra(map::map_graph& graph, graph::basic_graph<map::map_node_info>::node_type const& start, graph::basic_graph<map::map_node_info>::node_type const& end)
{
    std::priority_queue<std::pair<int, graph::basic_node<map::map_node_info>*>,
	std::vector<std::pair<int, graph::basic_node<map::map_node_info>*>>,
	std::greater<std::pair<int, graph::basic_node<map::map_node_info>*>>> priorityQueue;

    std::vector<graph::basic_node<map::map_node_info>*> visitedNodes{};

    graph::basic_node<map::map_node_info>* startNode = nullptr;
    graph::basic_node<map::map_node_info>* endNode = nullptr;

    for (std::size_t i = 0; i < graph.num_nodes(); ++i) {
	if (graph[i].node_id() == start.node_id())
	{
	    startNode = &graph[i];
	    graph[i].set_shortest_distance(0);
	}
    }

    priorityQueue.push(std::make_pair(startNode->shortest_distance(), startNode));

    int checks = 0;

    /* Looping till priority queue becomes empty (or all
      distances are not finalized) */
    while (!priorityQueue.empty())
    {
	checks++;

	// The first vertex in pair is the minimum distance 
	// vertex, extract it from priority queue. 
	// vertex label is stored in second of pair (it 
	// has to be done this way to keep the vertices 
	// sorted distance (distance must be first item 
	// in pair) 
	graph::basic_node<map::map_node_info>* node = priorityQueue.top().second;
	priorityQueue.pop();

	visitedNodes.emplace_back(node);

	if (node->node_id() == end.node_id())
	    endNode = node;

	// 'i' is used to get all adjacent vertices of a vertex 
	for (auto& i : *node)
	{
	    graph::basic_node<map::map_node_info>* oppositeNode;

	    if (node == &i.from())
	    {
		oppositeNode = &i.to();
	    }
	    else
	    {
		oppositeNode = &i.from();
	    }

	    if (std::find(visitedNodes.begin(), visitedNodes.end(), oppositeNode) == visitedNodes.end())
	    {
		//  If there is shorted path to v through u. 
		if (oppositeNode->shortest_distance() > node->shortest_distance() + i.weight())
		{
		    // Updating distance of v 
		    oppositeNode->set_shortest_distance(node->shortest_distance() + i.weight());
		    oppositeNode->set_from_node(node);
		    priorityQueue.push(std::make_pair(oppositeNode->shortest_distance(), oppositeNode));
		}
	    }
	}
    }

    std::cout << checks << std::endl;

    if (endNode->from_node() != nullptr || endNode->node_id() == start.node_id())
    {
	while (endNode != nullptr)
	{
	    endNode->tagged(true);
	    endNode = endNode->from_node();
	}
    }
}

int distanceBetween(math::vector2d vector1, math::vector2d vector2)
{
    math::vector2d resultVector = vector2 - vector1;

    return abs(resultVector.x() + resultVector.y()) / 32;
}

std::stack<graph::basic_node<map::map_node_info>*> aStar(map::map_graph& graph, graph::basic_graph<map::map_node_info>::node_type const& start, graph::basic_graph<map::map_node_info>::node_type const& end)
{
    std::stack<graph::basic_node<map::map_node_info>*> shortestPath = std::stack<graph::basic_node<map::map_node_info>*>{};

    std::priority_queue<std::pair<int, graph::basic_node<map::map_node_info>*>,
	std::vector<std::pair<int, graph::basic_node<map::map_node_info>*>>,
	std::greater<std::pair<int, graph::basic_node<map::map_node_info>*>>> priorityQueue;

    std::vector<graph::basic_node<map::map_node_info>*> visitedNodes{};

    graph::basic_node<map::map_node_info>* startNode = nullptr;
    graph::basic_node<map::map_node_info>* endNode = nullptr;

    for (std::size_t i = 0; i < graph.num_nodes(); ++i) {
	if (graph[i].node_id() == start.node_id())
	{
	    startNode = &graph[i];
	    graph[i].set_shortest_distance(0);
	}
    }

    priorityQueue.push(std::make_pair(startNode->shortest_distance(), startNode));

    int checks = 0;

    /* Looping till priority queue becomes empty (or all
      distances are not finalized) */
    while (!priorityQueue.empty())
    {
	checks++;

	// The first vertex in pair is the minimum distance 
	// vertex, extract it from priority queue. 
	// vertex label is stored in second of pair (it 
	// has to be done this way to keep the vertices 
	// sorted distance (distance must be first item 
	// in pair) 
	graph::basic_node<map::map_node_info>* node = priorityQueue.top().second;

	if (node->node_id() == end.node_id())
	{
	    endNode = node;
	    break;
	}

	priorityQueue.pop();

	node->tagged(true);
	visitedNodes.emplace_back(node);

	// 'i' is used to get all adjacent vertices of a vertex 
	for (auto& vertex : *node)
	{
	    graph::basic_node<map::map_node_info>* oppositeNode;

	    if (node == &vertex.from())
	    {
		oppositeNode = &vertex.to();
	    }
	    else
	    {
		oppositeNode = &vertex.from();
	    }

	    if (std::find(visitedNodes.begin(), visitedNodes.end(), oppositeNode) == visitedNodes.end())
	    {
		//  If there is shorted path to v through u. 
		if (oppositeNode->shortest_distance() > node->shortest_distance() + vertex.weight() + distanceBetween(node->location(), end.location()))
		{
		    // Updating distance of v 
		    oppositeNode->set_shortest_distance(node->shortest_distance() + vertex.weight() + distanceBetween(oppositeNode->location(), end.location()));
		    oppositeNode->set_from_node(node);
		    priorityQueue.push(std::make_pair(oppositeNode->shortest_distance(), oppositeNode));
		}
	    }
	}
    }

    std::cout << checks << std::endl;

    if (endNode->from_node() != nullptr || endNode->node_id() == start.node_id())
    {
	while (endNode != nullptr)
	{
	    shortestPath.push(endNode);
	    endNode->tagged(true);
	    endNode = endNode->from_node();
	}
    }

    for (auto& node : graph)
    {
	node.reset();
    }

    return shortestPath;
}

class rectangle_drawable : public ui::drawable {
public:
    rectangle_drawable(play::actor const &actor) : drawable{}, actor_{ &actor } {}
    void draw(ui::frame &f) const override;
private:
    play::actor const *actor_;
};
void rectangle_drawable::draw(ui::frame &f) const {
    f.draw_rectangle(actor_->location(), { 10.0, 10.0 }, graphics::colors::white);
}
class hello_actor : public play::free_roaming_actor {
public:
    hello_actor(math::vector2d location)
	: free_roaming_actor{ location }, drawable_{ *this } {}
    const ui::drawable &drawable() const override { return drawable_; }
    void move(math::vector2d delta) { location(location() + delta); }
private:
    rectangle_drawable drawable_;
};

int main() {
    // een app object is nodig om
    ui::app app{};

    //  maak een venster aan
    ui::window window{ app.create_window({1024, 768}, "hello") };

    // maak een podium aan
    play::stage s{};

    math::vector2d center{ 512.0, 384.0 };

    s.build_actor<play::background>( // Draws background
	math::size(1024, 768),
	graphics::image{ m.background_image() });
    s.build_actor<play::map_actor>( // Draws graph corresponding to map
	math::vector2d{ 0.0f, 0.0f },
	m.graph());

    // Draw the rest on top
    auto &my_actor = s.build_actor<hello_actor>(center);

    // Vind startnode van koe
    auto &cow_node = find_cow_node(m.graph());
    auto &my_cow = s.build_actor<cow>(m.graph(), cow_node);
    auto &my_hare = s.build_actor<hare>(m.graph());
    my_hare.set_cow(my_cow);


    //map::map m{ map::read_map(map_description) };
    auto &graph = m.graph();

    //// num_nodes -> Haal alle knopen op
    //for (std::size_t i = 0; i < graph.num_nodes(); ++i) {
    //	std::cout << "Knoop op: " << graph[i].location().x() << ", "
    //		<< graph[i].location().y() << "\n";
    //}

    // num_edges -> Haal alle edges op van een knoop
    auto &node = graph[0];

    for (std::size_t i = 0; i < node.num_edges(); ++i) {
	auto &from = node[i].from();
	auto &to = node[i].to();
	std::cout << "Kant van: " << from.location().x() << ", "
	    << from.location().y() << " naar " << to.location().x() << ", "
	    << to.location().y() << "\n";
	std::cout << "Weight: " << node[i].weight() << "\n";
	std::cout << node.node_info().kind << std::endl;
    }

    // weight() -> Gewicht ophalen
    //auto &edge = node[0];
    //float weight = edge.weight();
    //std::cout << "Gewicht: " << weight << "\n";

    my_cow.set_shortest_path(aStar(graph, my_cow.node(), my_hare.node()));

    // Maak een event_source aan (hieruit kun je alle events halen, zoals
    // toetsaanslagen)
    ui::events::event_source event_source{};

    // main_loop stuurt alle actors aan.
    main_loop(s, window, [&](delta_time dt, loop_controls &ctl) {
	// gebruik dt om te kijken hoeveel tijd versterken is
	// sinds de vorige keer dat deze lambda werd aangeroepen
	// loop controls is een object met eigenschappen die je kunt gebruiken om de
	// main-loop aan te sturen.

	if (my_cow.reached_hare())
	{
	    graph.untag_all();
	    my_hare.getNewNode(graph);
	    my_cow.set_shortest_path(aStar(graph, my_cow.node(), my_hare.node()));
	}

	for (ui::events::event &e : event_source) {
	    // event heeft een methode handle_quit die controleert
	    // of de gebruiker de applicatie wilt sluiten, en zo ja
	    // de meegegeven functie (of lambda) aanroept om met het
	    // bijbehorende quit_event
	    //
	    e.handle_quit([&ctl](ui::events::quit_event qe) { ctl.quit = true; });
	    e.handle_key_up([&my_actor](ui::events::key_event k) {
		switch (k.key) {
		case ui::events::key::up:
		    my_actor.move({ 0, -5.0f });
		    break;
		case ui::events::key::down:
		    my_actor.move({ 0, 5.0f });
		    break;
		case ui::events::key::left:
		    my_actor.move({ -5.0f, 0 });
		    break;
		case ui::events::key::right:
		    my_actor.move({ 5.0f, 0 });
		    break;
		default:
		    break;
		}
	    });
	}

    });
}
