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
#include "LandscapeGraph.hpp"


LandscapeGraph::LandscapeGraph(uint32_t mapW, uint32_t mapH, const std::map<std::tuple<uint32_t, uint32_t>, bool> blocked) {
    this->mapW = mapW;
    this->mapH = mapH;
    this->blocked = blocked;
}
std::map<std::tuple<uint32_t, uint32_t>, uint32_t> LandscapeGraph::getAvailable(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t l) {
    std::map<std::tuple<uint32_t, uint32_t>, uint32_t> dist;
    for (uint32_t x1 = x; x1 < x + sx; x1 = x1 + 1) {
        for (uint32_t y1 = y; y1 < y + sy; y1 = y1 + 1) {
            std::map<std::tuple<uint32_t, uint32_t>, uint32_t> distTmp;
            this->bfs(x1, y1, l, distTmp);
            for (const auto &a : distTmp) {
                if (dist.find(a.first) == dist.end()) {
                    dist[a.first] = a.second;
                }
                else {
                    dist[a.first] = std::min(dist[a.first], a.second);
                }
            }
        }
    }

    return dist;
}
void LandscapeGraph::bfs(uint32_t x, uint32_t y, uint32_t l, std::map<std::tuple<uint32_t, uint32_t>, uint32_t> &dist) {
    dist[std::make_tuple(x, y)] = 0;
    std::queue<std::tuple<uint32_t, uint32_t>> q;
    q.emplace(x, y);

    while (!q.empty()) {
        std::tuple<uint32_t, uint32_t> v = q.front();
        q.pop();
        uint32_t vx, vy;
        std::tie(vx, vy) = v;

        if (dist.find(v) == dist.end()) {
            dist[v] = std::numeric_limits<uint32_t>::max();
        }

        std::vector<std::tuple<uint32_t, uint32_t>> adj;
        if (vx >= 1) {
            auto to = std::make_tuple(vx - 1, vy);
            if (!this->blocked[to]) {
                adj.push_back(to);
            }
        }
        if (vy >= 1) {
            auto to = std::make_tuple(vx, vy - 1);
            if (!this->blocked[to]) {
                adj.push_back(to);
            }
        }
        if (vx + 1 < this->mapW) {
            auto to = std::make_tuple(vx + 1, vy);
            if (!this->blocked[to]) {
                adj.push_back(to);
            }
        }
        if (vy + 1 < this->mapH) {
            auto to = std::make_tuple(vx, vy + 1);
            if (!this->blocked[to]) {
                adj.push_back(to);
            }
        }

        for (std::tuple<uint32_t, uint32_t> u : adj) {
            uint32_t nDst = dist[v] + 1;

            if (nDst <= l) {
                if (dist.find(u) == dist.end()) {
                    dist[u] = std::numeric_limits<uint32_t>::max();
                }
                if (dist[u] > nDst) {
                    dist[u] = nDst;
                    q.push(u);
                }
            }
        }
    }
}