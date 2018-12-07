#ifndef KMINTAPP_ASTAR_H
#define KMINTAPP_ASTAR_H

#include "kmint/map/map.hpp"
#include <stack>
#include "kmint/graph/graph.hpp"
#include <iostream>

#include <queue>

class aStar
{
public:
    static int distanceBetween(kmint::math::vector2d vector1, kmint::math::vector2d vector2)
    {
        kmint::math::vector2d resultVector = vector2 - vector1;

        int distance = abs(resultVector.x()) + abs(resultVector.y()) / 32;

        std::cout << distance << std::endl;

        return distance;
    }

    static std::stack<kmint::graph::basic_node<kmint::map::map_node_info>*> run(
        kmint::map::map_graph& graph, kmint::graph::basic_graph<kmint::map::map_node_info>::node_type const& start,
        kmint::graph::basic_graph<kmint::map::map_node_info>::node_type const& end)
    {
        std::stack<kmint::graph::basic_node<kmint::map::map_node_info>*> shortestPath = std::stack<kmint::graph::basic_node<kmint::map::map_node_info>*>{};

        std::priority_queue<std::pair<int, kmint::graph::basic_node<kmint::map::map_node_info>*>,
            std::vector<std::pair<int, kmint::graph::basic_node<kmint::map::map_node_info>*>>,
            std::greater<std::pair<int, kmint::graph::basic_node<kmint::map::map_node_info>*>>> priorityQueue;

        std::vector<kmint::graph::basic_node<kmint::map::map_node_info>*> visitedNodes{};

        kmint::graph::basic_node<kmint::map::map_node_info>* startNode = nullptr;
        kmint::graph::basic_node<kmint::map::map_node_info>* endNode = nullptr;

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
            kmint::graph::basic_node<kmint::map::map_node_info>* node = priorityQueue.top().second;

            if (node->node_id() == end.node_id())
            {
                endNode = node;
                break;
            }

            priorityQueue.pop();

            //node->tagged(true);
            visitedNodes.emplace_back(node);

            // 'i' is used to get all adjacent vertices of a vertex 
            for (auto& vertex : *node)
            {
                kmint::graph::basic_node<kmint::map::map_node_info>* oppositeNode;

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
                    if (oppositeNode->shortest_distance() > node->shortest_distance() + vertex.weight() + distanceBetween(oppositeNode->location(), end.location()))
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
                //endNode->tagged(true);
                endNode = endNode->from_node();
            }
        }

        for (auto& node : graph)
        {
            node.reset();
        }

        return shortestPath;
    }
};
#endif
