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


#include "AreaControllerSpec.hpp"
#include "SetHighlightEvent.hpp"
#include "LandscapeGraph.hpp"
#include "Building.hpp"


HashTableMapPosition<uint32_t> IAreaControllerSpec::getAvailable(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t playerId, MapState* state) const {
    HashTableMapPosition<bool> blocked;

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
    return g.getAvailable(x, y, sx, sy, this->getRadius());
}
bool IAreaControllerSpec::IN_RADIUS(const HashTableMapPosition<uint32_t>& available, const GO* target, uint8_t type) {
    for (uint32_t x = target->getX(); x < target->getX() + target->getSX(); x = x + 1) {
        for (uint32_t y = target->getY(); y < target->getY() + target->getSY(); y = y + 1) {
            bool found = (available.find(std::make_tuple(x, y)) != available.end());
            if (found and type == IN_RADIUS_TYPE::PARTIALLY) {
                return true;
            }
            if (!found and type == IN_RADIUS_TYPE::FULLY) {
                return false;
            }
        }
    }
    return (type == IN_RADIUS_TYPE::FULLY);
}
Events IAreaControllerSpec::getHighlightEvent(const Building *building, MapState *state, uint8_t type) const {
    if (type != HIGHLIGHT_TYPE::UNIVERSAL and type != this->getHighlightType()) {
        return Events();
    }

    HashTableMapPosition<uint32_t> available = this->getAvailable(building->getX(), building->getY(), building->getSX(), building->getSY(), building->getPlayerId(), state);

    Events events;
    for (const auto& a : available) {
        events.add(std::make_shared<SetHighlightEvent>(this->getHighlightColor(building->getPlayerId()), std::get<0>(a.first), std::get<1>(a.first)));
    }

    return events;
}
bool IAreaControllerSpec::ignoreUltraHighObstacles() const {
    return false;
}
bool IAreaControllerSpec::ignoreHighObstacles() const {
    return false;
}
bool IAreaControllerSpec::ignoreLowObstacles() const {
    return false;
}


BOOST_CLASS_EXPORT_IMPLEMENT(IAreaControllerSpec)