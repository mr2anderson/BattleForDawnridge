/*
 *  Battle for Dawnridge
 *  Copyright (C) 2024 mr2anderson
 *
 *  Battle for Dawnridge is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Battle for Dawnridge is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Battle for Dawnridge.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <limits>
#include <algorithm>
#include "MovementGraph.hpp"
#include "DijkstraQueueElement.hpp"
#include "MoveDoesNotExist.hpp"


MovementGraph::MovementGraph(uint32_t mapW, uint32_t mapH) {
	this->mapW = mapW;
	this->mapH = mapH;
}
void MovementGraph::setCantStay(uint32_t x, uint32_t y) {
	this->table[std::make_tuple(x, y)].canStay = false;
}
void MovementGraph::setCanAttack(uint32_t x, uint32_t y) {
	this->table[std::make_tuple(x, y)].canAttack = true;
}
void MovementGraph::updateMovementCost(uint32_t x, uint32_t y, uint32_t movementCost) {
	this->table[std::make_tuple(x, y)].movementCost = std::max(this->table[std::make_tuple(x, y)].movementCost, movementCost);
}
Move MovementGraph::getMove(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t movePoints) {
    std::tuple<uint32_t, uint32_t> from = std::make_tuple(x1, y1);
    std::tuple<uint32_t, uint32_t> to = std::make_tuple(x2, y2);

    HashTableMapPosition<uint32_t> dist;
    HashTableMapPosition<std::tuple<uint32_t, uint32_t>> fromToStory;
    this->djkstra(from, movePoints, dist, fromToStory);

    if (dist.find(to) == dist.end() or !this->table[to].canStay) {
        throw MoveDoesNotExist();
    }

    Move move;
    move.finalX = x2;
    move.finalY = y2;
    move.dst = dist[to];

    std::tuple<uint32_t, uint32_t> current = to;
    while (current != from) {
        std::tuple<uint32_t, uint32_t> previous = fromToStory.at(current);
        if (std::get<0>(current) > std::get<0>(previous)) {
            move.route.push_back("e");
        }
        else if (std::get<0>(current) < std::get<0>(previous)) {
            move.route.push_back("w");
        }
        else if (std::get<1>(current) > std::get<1>(previous)) {
            move.route.push_back("s");
        }
        else {
            move.route.push_back("n");
        }
        current = previous;
    }

	std::reverse(move.route.begin(), move.route.end());

    return move;
}
std::vector<std::tuple<uint32_t, uint32_t>> MovementGraph::getMoves(uint32_t x, uint32_t y, uint32_t movePoints) {
    HashTableMapPosition<uint32_t> dist;
    HashTableMapPosition<std::tuple<uint32_t, uint32_t>> fromToStory;
	this->djkstra(std::make_tuple(x, y), movePoints, dist, fromToStory);
	
	std::vector<std::tuple<uint32_t, uint32_t>> moves;
	for (const auto& a : dist) {
		if (this->table[a.first].canStay) {
            moves.emplace_back(a.first);
		}
	}
	return moves;
}
void MovementGraph::djkstra(std::tuple<uint32_t, uint32_t> s, uint32_t movePoints, 
	HashTableMapPosition<uint32_t> &dist,
	HashTableMapPosition<std::tuple<uint32_t, uint32_t>>& fromToStory) {

	dist[s] = 0;

	std::priority_queue<DijkstraQueueElement, std::vector<DijkstraQueueElement>, std::greater<DijkstraQueueElement>> q;
	
	q.emplace(0, s);

	while (!q.empty()) {
		DijkstraQueueElement c = q.top();
		q.pop();
		uint32_t dst = c.dst;
		uint32_t vx = c.x;
		uint32_t vy = c.y;
		std::tuple<uint32_t, uint32_t> v = std::make_tuple(vx, vy);

		if (dist.find(v) == dist.end()) {
			dist[v] = std::numeric_limits<uint32_t>::max();
		}

		if (dist[v] < dst or dst == movePoints) {
			continue;
		}

		std::vector<DijkstraQueueElement> graph;
		if (vx >= 1) {
			auto to = std::make_tuple(vx - 1, vy);
			graph.emplace_back(this->table[to].movementCost, to);
		}
		if (vy >= 1) {
			auto to = std::make_tuple(vx, vy - 1);
			graph.emplace_back(this->table[to].movementCost, to);
		}
		if (vx + 1 < this->mapW) {
			auto to = std::make_tuple(vx + 1, vy);
			graph.emplace_back(this->table[to].movementCost, to);
		}
		if (vy + 1 < this->mapH) {
			auto to = std::make_tuple(vx, vy + 1);
			graph.emplace_back(this->table[to].movementCost, to);
		}

		for (DijkstraQueueElement e : graph) {
			uint32_t len = e.dst;
			std::tuple<uint32_t, uint32_t> u = std::make_tuple(e.x, e.y);

            uint32_t nDst = dst + len;
            if (nDst <= movePoints) {
                if (dist.find(u) == dist.end()) {
                    dist[u] = std::numeric_limits<uint32_t>::max();
                }

                if (nDst < dist[u]) {
                    dist[u] = nDst;
                    fromToStory[u] = v;
                    q.emplace(nDst, u);
                }
            }
		}
	}
}