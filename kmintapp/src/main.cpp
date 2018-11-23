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
#include "distance.h"
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
	std::vector<graph::basic_graph<map::map_node_info>::node_type*> shortestPath;

	std::vector<graph::basic_node<map::map_node_info>*> nodes = std::vector<graph::basic_node<map::map_node_info>*>{};

	graph::basic_node<map::map_node_info>* endNode = nullptr;

	for (std::size_t i = 0; i < graph.num_nodes(); ++i) {
		if (graph[i].node_id() == start.node_id())
			graph[i].set_shortest_distance(0);

		nodes.emplace_back(&graph[i]);
	}
	//distance dist{};
	//distances.insert(std::pair<graph::basic_graph<map::map_node_info>::node_type const*, distance*>(&start, &dist));

	while (!nodes.empty())
	{
		graph::basic_node<map::map_node_info>* shortestDistanceNode = nullptr;
		
		for (graph::basic_node<map::map_node_info>* node : nodes)
		{
			if (shortestDistanceNode == nullptr || node->shortest_distance() < shortestDistanceNode->shortest_distance())
			{
				shortestDistanceNode = node;
			}
		}

		//nodes.erase(std::find(nodes.begin(), nodes.end(), shortestDistanceNode), nodes.end());

		nodes.erase(std::remove_if(nodes.begin(), nodes.end(), [shortestDistanceNode](graph::basic_node<map::map_node_info>* node) { return node->node_id() == shortestDistanceNode->node_id(); }), nodes.end());

		if (shortestDistanceNode->node_id() != end.node_id())
		{
			for (std::size_t i = 0; i < shortestDistanceNode->num_edges(); ++i) {
				//graph::basic_edge<map::map_node_info>::node_type const& from = (*shortestDistanceNode)[i].from();
				//graph::basic_edge<map::map_node_info>::node_type const& to = (*shortestDistanceNode)[i].to();
				graph::basic_node<map::map_node_info>* oppositeNode;

				std::cout << shortestDistanceNode->node_info().kind;

				if (shortestDistanceNode == &(*shortestDistanceNode)[i].from())
				{
					oppositeNode = &(*shortestDistanceNode)[i].to();
				}
				else
				{
					oppositeNode = &(*shortestDistanceNode)[i].from();
				}

				if ((endNode != nullptr && endNode->node_id() == oppositeNode->node_id()) || std::find(nodes.begin(), nodes.end(), oppositeNode) != nodes.end())
				{
					const int totalDistance = shortestDistanceNode->shortest_distance() +
						(*shortestDistanceNode)[i].weight();

					if (totalDistance < oppositeNode->shortest_distance())
					{
						oppositeNode->set_shortest_distance(totalDistance);
						oppositeNode->set_from_node(shortestDistanceNode);
					}
				}
			}
		}
		else
		{
			endNode = shortestDistanceNode;
		}
	}

	// End room has been reached
	if (endNode->from_node() != nullptr || endNode->node_id() == start.node_id())
	{
		while (endNode != nullptr)
		{
			shortestPath.emplace_back(endNode);
			endNode = endNode->from_node();

		}
	}

	for (std::size_t i = 0; i < graph.num_nodes(); ++i)
	{
		for (std::size_t j = 0; j < shortestPath.size(); ++j)
		{
			if (shortestPath[j] == &graph[i])
			{
				graph[i].tagged(true);
			}
		}
	}
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
	}

	// weight() -> Gewicht ophalen
	//auto &edge = node[0];
	//float weight = edge.weight();
	//std::cout << "Gewicht: " << weight << "\n";

  // Maak een event_source aan (hieruit kun je alle events halen, zoals
  // toetsaanslagen)
	ui::events::event_source event_source{};

	dijkstra(graph ,my_cow.node(), my_hare.node());

	// main_loop stuurt alle actors aan.
	main_loop(s, window, [&](delta_time dt, loop_controls &ctl) {
		// gebruik dt om te kijken hoeveel tijd versterken is
		// sinds de vorige keer dat deze lambda werd aangeroepen
		// loop controls is een object met eigenschappen die je kunt gebruiken om de
		// main-loop aan te sturen.

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
