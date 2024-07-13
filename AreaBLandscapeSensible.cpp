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
#include "AreaBLandscapeSensible.hpp"
#include "ChangeHighlightEvent.hpp"
#include "ColorTheme.hpp"
#include "LandscapeGraph.hpp"


AreaBLandscapeSensible::AreaBLandscapeSensible() = default;
AreaBLandscapeSensible::AreaBLandscapeSensible(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units, std::shared_ptr<GOCollection<GO>> go, uint32_t mapW, uint32_t mapH) :
    AreaB(x, y, sx, sy, maxHp, playerId, units) {
    this->go = go;
    this->mapW = mapW;
    this->mapH = mapH;
    this->prevHash = std::numeric_limits<uint32_t>::max();
}
bool AreaBLandscapeSensible::inRadius(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy) {
    std::map<std::tuple<uint32_t, uint32_t>, uint32_t> available = this->getAvailable();
    for (uint32_t x1 = x; x1 < x + sx; x1 = x1 + 1) {
        for (uint32_t y1 = y; y1 < y + sy; y1 = y1 + 1) {
            if (available.find(std::make_tuple(x1, y1)) == available.end()) {
                return false;
            }
        }
    }
    return true;
}
Events AreaBLandscapeSensible::getHighlightEvent() {
    std::map<std::tuple<uint32_t, uint32_t>, uint32_t> available = this->getAvailable();

    Events events;
    for (const auto &a : available) {
        events.add(std::make_shared<ChangeHighlightEvent>(this, COLOR_THEME::CELL_COLOR_HIGHLIGHTED_DARK, std::get<0>(a.first), std::get<1>(a.first), 1, 1));
    }

    return events;
}
std::shared_ptr<GOCollection<GO>> AreaBLandscapeSensible::getGO() {
    return this->go;
}
uint32_t AreaBLandscapeSensible::getMapW() const {
    return this->mapW;
}
uint32_t AreaBLandscapeSensible::getMapH() const {
    return this->mapH;
}
std::map<std::tuple<uint32_t, uint32_t>, uint32_t> AreaBLandscapeSensible::getAvailable() {
    if (this->prevHash == this->getHash()) {
        return this->prevAvailable;
    }

    std::map<std::tuple<uint32_t, uint32_t>, bool> blocked;

    for (uint32_t i = 0; i < this->go->size(); i = i + 1) {
        GO *o = this->go->at(i);
        if (!o->exist()) {
            continue;
        }
        if ((!this->ignoreLowObstacles() and o->isLowObstacle(this->getPlayerId())) or (!this->ignoreHighObstacles() and o->isHighObstacle(this->getPlayerId()))) {
            for (uint32_t x = o->getX(); x < o->getX() + o->getSX(); x = x + 1) {
                for (uint32_t y = o->getY(); y < o->getY() + o->getSY(); y = y + 1) {
                    blocked[std::make_tuple(x, y)] = true;
                }
            }
        }
    }

    LandscapeGraph g(this->mapW, this->mapH, blocked);
    std::map<std::tuple<uint32_t, uint32_t>, uint32_t> result = g.getAvailable(this->getX(), this->getY(), this->getSX(), this->getSY(), this->getRadius());

    this->prevAvailable = result;
    this->prevHash = this->getHash();

    return result;
}
uint64_t AreaBLandscapeSensible::getHash() const {
    uint64_t exist = 0;
    for (uint32_t i = 0; i < this->go->size(); i = i + 1) {
        exist = exist + this->go->at(i)->exist();
    }

    uint64_t total = this->go->size();

    return (total << 20) + exist;
}