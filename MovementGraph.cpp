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


#include <queue>
#include <limits>
#include <algorithm>
#include "MovementGraph.hpp"
#include "PathDoesNotExist.hpp"


MovementGraph::MovementGraph(uint32_t mapW, uint32_t mapH) {
	this->mapW = mapW;
	this->mapH = mapH;
}
void MovementGraph::set(uint32_t x, uint32_t y, bool canStay, bool canMoveThrough) {
	this->fitTable[std::make_tuple(x, y)] = FitTableElement(canStay, canMoveThrough);
}
std::vector<Move> MovementGraph::getMoves(uint32_t x, uint32_t y, uint32_t movePoints) {
	std::map<std::tuple<uint32_t, uint32_t>, std::tuple<uint32_t, uint32_t>> fromToStory;
	std::map<std::tuple<uint32_t, uint32_t>, uint32_t> dist = this->bfs(std::make_tuple(x, y), movePoints, fromToStory);
	
	std::vector<Move> moves;
	for (const auto& a : dist) {
		if (this->fitTable[a.first].canStay) {
			Move move;
			move.finalX = std::get<0>(a.first);
			move.finalY = std::get<1>(a.first);
			
			std::tuple<uint32_t, uint32_t> current = a.first;
			while (current != std::make_tuple(x, y)) {
				std::tuple<uint32_t, uint32_t> previous = fromToStory.at(current);
				if (std::get<0>(current) > std::get<0>(previous)) {
					move.route.push_back('e');
				}
				else if (std::get<0>(current) < std::get<0>(previous)) {
					move.route.push_back('w');
				}
				else if (std::get<0>(current) > std::get<0>(previous)) {
					move.route.push_back('s');
				}
				else {
					move.route.push_back('n');
				}
				current = previous;
			}
			std::reverse(move.route.begin(), move.route.end());

			moves.push_back(move);
		}
	}
	return moves;
}
std::map<std::tuple<uint32_t, uint32_t>, uint32_t> MovementGraph::bfs(std::tuple<uint32_t, uint32_t> s, uint32_t movePoints, std::map<std::tuple<uint32_t, uint32_t>, std::tuple<uint32_t, uint32_t>>& fromToStory) {
	std::map<std::tuple<uint32_t, uint32_t>, uint32_t> dist;
	dist[s] = 0;
	std::queue<std::tuple<uint32_t, uint32_t>> q;
	q.push(s);

	while (!q.empty()) {
		std::tuple<uint32_t, uint32_t> v = q.front();
		q.pop();
		uint32_t vx, vy;
		std::tie(vx, vy) = v;

		if (dist.find(v) == dist.end()) {
			dist[v] = std::numeric_limits<uint32_t>::max();
		}
		else if (dist[v] >= movePoints) {
			continue;
		}

		std::vector<std::tuple<uint32_t, uint32_t>> adj;
		if (vx >= 1) {
			auto to = std::make_tuple(vx - 1, vy);
			if (this->fitTable[to].canMoveThrough) {
				adj.push_back(to);
			}
		}
		if (vy >= 1) {
			auto to = std::make_tuple(vx, vy - 1);
			if (this->fitTable[to].canMoveThrough) {
				adj.push_back(to);
			}
		}
		if (vx + 2 < this->mapW) {
			auto to = std::make_tuple(vx + 1, vy);
			if (this->fitTable[to].canMoveThrough) {
				adj.push_back(to);
			}
		}
		if (vx + 2 < this->mapH) {
			auto to = std::make_tuple(vx, vy + 1);
			if (this->fitTable[to].canMoveThrough) {
				adj.push_back(to);
			}
		}

		for (std::tuple<uint32_t, uint32_t> u : adj) {

			if (dist.find(u) == dist.end()) {
				dist[u] = std::numeric_limits<uint32_t>::max();
			}

			if (dist[u] > dist[v] + 1) {
				dist[u] = dist[v] + 1;
				fromToStory[u] = v;
				q.push(u);
			}
		}
	}

	return dist;
}