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


#include "ConductionGraph.hpp"


ConductionGraph::ConductionGraph() = default;
void ConductionGraph::addConductor(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy) {
	this->add(x, y, sx, sy, false);
}
void ConductionGraph::addDestination(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy) {
	this->add(x, y, sx, sy, true);
}
bool ConductionGraph::connectedToDestination(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy) {
	this->addConductor(x, y, sx, sy);
	this->addAllPossiblePaths();
	HashTableMapPosition<bool> visited;
	return this->bfs(std::make_tuple(x, y), visited);
}
void ConductionGraph::add(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, bool destination) {
	for (uint32_t i = 0; i < sx; i = i + 1) {
		for (uint32_t j = 0; j < sy; j = j + 1) {
			auto p = std::make_tuple(x + i, y + j);
			if (this->isDestination.find(p) == this->isDestination.end()) {
				this->isDestination[p] = destination;
			}
			else {
				this->isDestination[p] = this->isDestination[p] or destination;
			}
		}
	}
}
void ConductionGraph::addAllPossiblePaths() {
	for (const auto& a : this->isDestination) {
		this->addPossiblePaths(a.first);
	}
}
void ConductionGraph::addPossiblePaths(std::tuple<uint32_t, uint32_t> p) {
	if (std::get<0>(p) != 0) {
		this->addPathBetween(p, std::make_tuple(std::get<0>(p) - 1, std::get<1>(p)));
	}
	if (std::get<1>(p) != 0) {
		this->addPathBetween(p, std::make_tuple(std::get<0>(p), std::get<1>(p) - 1));
	}
	this->addPathBetween(p, std::make_tuple(std::get<0>(p) + 1, std::get<1>(p)));
	this->addPathBetween(p, std::make_tuple(std::get<0>(p), std::get<1>(p) + 1));
}
void ConductionGraph::addPathBetween(std::tuple<uint32_t, uint32_t> p1, std::tuple<uint32_t, uint32_t> p2) {
	if (this->isDestination.find(p2) == this->isDestination.end()) {
		return;
	}
	this->paths[p1].insert(p2);
	this->paths[p2].insert(p1);
}
bool ConductionGraph::bfs(std::tuple<uint32_t, uint32_t> current, HashTableMapPosition<bool>& visited) const {
	if (this->isDestination.find(current) != this->isDestination.end() and this->isDestination.at(current)) {
		return true;
	}

	visited[current] = true;
	if (this->paths.find(current) == this->paths.end()) {
		return false;
	}
	for (const auto& to : this->paths.at(current)) {
		if (visited[to]) {
			continue;
		}
		if (this->bfs(to, visited)) {
			return true;
		}
	}
	return false;
}