/*
 *  Conquesta
 *  Copyright (C) 2024 mr2anderson
 *
 *  Conquesta is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Conquesta is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Conquesta.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <map>
#include <set>
#include <tuple>
#include <cstdint>


#pragma once


class ConductionGraph {
public:
	ConductionGraph();

	void addConductor(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy);
	void addOrigin(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy);
	bool connectedToOrigin(uint32_t x, uint32_t y) const;
private:
	std::map<std::tuple<uint32_t, uint32_t>, std::set<std::tuple<uint32_t, uint32_t>>> paths;
	std::map<std::tuple<uint32_t, uint32_t>, bool> isOrigin;

	void add(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, bool origin);
	void addPossiblePaths(std::tuple<uint32_t, uint32_t> p);
	void addPathBetween(std::tuple<uint32_t, uint32_t> p1, std::tuple<uint32_t, uint32_t> p2);
	bool bfs(std::tuple<uint32_t, uint32_t> current, std::map<std::tuple<uint32_t, uint32_t>, bool>& visited) const;
};