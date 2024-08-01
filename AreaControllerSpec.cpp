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
#include "AreaControllerSpec.hpp"
#include "SetHighlightEvent.hpp"
#include "LandscapeGraph.hpp"
#include "Building.hpp"
#include "ArchiveType.hpp"


AreaControllerSpec::AreaControllerSpec() {
	this->prevHash = std::numeric_limits<uint32_t>::max();
}
std::map<std::tuple<uint32_t, uint32_t>, uint32_t> AreaControllerSpec::getAvailable(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t playerId, MapState* state) {
    if (this->prevHash == this->getHash(state)) {
        return this->prevAvailable;
    }

    std::map<std::tuple<uint32_t, uint32_t>, bool> blocked;

    for (uint32_t i = 0; i < state->getCollectionsPtr()->totalGOs(); i = i + 1) {
        GO* go = state->getCollectionsPtr()->getGO(i, FILTER::DEFAULT_PRIORITY);
        if (!go->exist()) {
            continue;
        }
        if ((!this->ignoreLowObstacles() and go->isLowObstacle(playerId)) or
            (!this->ignoreHighObstacles() and go->isHighObstacle(playerId)) or
            (!this->ignoreUltraHighObstacles() and go->isUltraHighObstacle(playerId))) {
            for (uint32_t x = go->getX(); x < go->getX() + go->getSX(); x = x + 1) {
                for (uint32_t y = go->getY(); y < go->getY() + go->getSY(); y = y + 1) {
                    blocked[std::make_tuple(x, y)] = true;
                }
            }
        }
    }

    LandscapeGraph g(state->getMapSizePtr()->getWidth(), state->getMapSizePtr()->getHeight(), blocked);
    std::map<std::tuple<uint32_t, uint32_t>, uint32_t> result = g.getAvailable(x, y, sx, sy, this->getRadius());

    this->prevAvailable = result;
    this->prevHash = this->getHash(state);

    return result;
}
Events AreaControllerSpec::getHighlightEvent(const Building *building, MapState *state, uint8_t type) {
    if (type != HIGHLIGHT_TYPE::UNIVERSAL and type != this->getHighlightType()) {
        return Events();
    }

    std::map<std::tuple<uint32_t, uint32_t>, uint32_t> available = this->getAvailable(building->getX(), building->getY(), building->getSX(), building->getSY(), building->getPlayerId(), state);

    Events events;
    for (const auto& a : available) {
        events.add(std::make_shared<SetHighlightEvent>(this->getHighlightColor(building->getPlayerId()), std::get<0>(a.first), std::get<1>(a.first), 1, 1));
    }

    return events;
}
bool AreaControllerSpec::inRadius(const Building *building, MapState *state, uint32_t x2, uint32_t y2, uint32_t sx2, uint32_t sy2, uint8_t type) {
    std::map<std::tuple<uint32_t, uint32_t>, uint32_t> available = this->getAvailable(building->getX(), building->getY(), building->getSX(), building->getSY(), building->getPlayerId(), state);

    for (uint32_t x = x2; x < x2 + sx2; x = x + 1) {
        for (uint32_t y = y2; y < y2 + sy2; y = y + 1) {
            if (available.find(std::make_tuple(x, y)) == available.end()) {
                if (type == IN_RADIUS_TYPE::FULLY) {
                    return false;
                }
            }
            else {
                if (type == IN_RADIUS_TYPE::PARTIALLY) {
                    return true;
                }
            }
        }
    }

    return (type == IN_RADIUS_TYPE::FULLY);
}
bool AreaControllerSpec::ignoreUltraHighObstacles() const {
    return false;
}
bool AreaControllerSpec::ignoreHighObstacles() const {
    return false;
}
bool AreaControllerSpec::ignoreLowObstacles() const {
    return false;
}
uint64_t AreaControllerSpec::getHash(MapState *state) const {
    uint64_t exist = 0;
    for (uint32_t i = 0; i < state->getCollectionsPtr()->totalGOs(); i = i + 1) {
        exist = exist + state->getCollectionsPtr()->getGO(i, FILTER::DEFAULT_PRIORITY)->exist();
    }

    uint64_t total = state->getCollectionsPtr()->totalGOs();

    return (total << 20ull) + exist;
}


BOOST_CLASS_EXPORT_IMPLEMENT(AreaControllerSpec)