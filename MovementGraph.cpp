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


#include "MovementGraph.hpp"
#include "PathDoesNotExist.hpp"


MovementGraph::MovementGraph(uint32_t mapW, uint32_t mapH) {
	this->mapW = mapW;
	this->mapH = mapH;
}
void MovementGraph::set(uint32_t x, uint32_t y, bool canFitIn) {
	this->fitTable[std::make_tuple(x, y)] = canFitIn;
}
std::vector<std::tuple<uint32_t, uint32_t>> MovementGraph::getMoves(uint32_t x, uint32_t y, uint32_t movePoints) {
	std::map<std::tuple<uint32_t, uint32_t>, uint32_t> visited;
	this->bfs(std::make_tuple(x, y), std::nullopt, visited, movePoints);

	std::vector<std::tuple<uint32_t, uint32_t>> moves;
	for (const auto& a : visited) {
		moves.push_back(a.first);
	}
	return moves;
}
void MovementGraph::bfs(std::tuple<uint32_t, uint32_t> p, std::optional<std::tuple<uint32_t, uint32_t>> dst, std::map<std::tuple<uint32_t, uint32_t>, uint32_t>& visited, uint32_t movePoints, uint32_t l) {
	visited[p] = l;
	if ((dst.has_value() and dst.value() == p) or movePoints == 0) {
		return;
	}

	uint32_t x, y;
	std::tie(x, y) = p;

	if (x != 0) {
		auto to = std::make_tuple(x - 1, y);
		if (this->fitTable[to] and visited.find(to) == visited.end()) {
			this->bfs(to, dst, visited, movePoints - 1, l + 1);
		}
	}
	if (y != 0) {
		auto to = std::make_tuple(x, y - 1);
		if (this->fitTable[to] and visited.find(to) == visited.end()) {
			this->bfs(to, dst, visited, movePoints - 1, l + 1);
		}
	}
	if (x != this->mapW - 1) {
		auto to = std::make_tuple(x + 1, y);
		if (this->fitTable[to] and visited.find(to) == visited.end()) {
			this->bfs(to, dst, visited, movePoints - 1, l + 1);
		}
	}
	if (y != this->mapH - 1) {
		auto to = std::make_tuple(x, y + 1);
		if (this->fitTable[to] and visited.find(to) == visited.end()) {
			this->bfs(to, dst, visited, movePoints - 1, l + 1);
		}
	}
}