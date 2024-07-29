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


#include <iostream>
#include "HighlightColors.hpp"
#include "PlayerColors.hpp"


HighlightColors* HighlightColors::singletone = nullptr;


static sf::Color GET_PLAYER_COLOR(uint32_t playerId) {
    if (playerId == PlayerColors::get()->getBlueID()) {
        return {0, 0, 255};
    }
    if (playerId == PlayerColors::get()->getGreenID()) {
        return {0, 0, 150};
    }
    if (playerId == PlayerColors::get()->getPurpleID()) {
        return {147, 0, 255};
    }
    std::cerr << "HighlightColors: warning: unknown color" << std::endl;
    return {};
}


static sf::Color BLEND(sf::Color c1, sf::Color c2) {
    return sf::Color(c1.r / 2 + c2.r / 2, c1.g / 2 + c2.g / 2, c1.b / 2 + c2.b / 2);
}


static sf::Color EVAL(sf::Color c, uint32_t playerId) {
    c = BLEND(c, GET_PLAYER_COLOR(playerId));
    c.a = 50;
    return c;
}


HighlightColors::HighlightColors() = default;
sf::Color HighlightColors::getWarriorMovementColor(uint32_t playerId) const {
    return EVAL(sf::Color(0, 150, 0), playerId);
}
sf::Color HighlightColors::getWarriorAttackColor(uint32_t playerId) const {
    return EVAL(sf::Color(150, 0, 150), playerId);
}
sf::Color HighlightColors::getWarriorHealColor(uint32_t playerId) const {
    return EVAL(sf::Color(0, 100, 100), playerId);
}
sf::Color HighlightColors::getTerritoryExpandingColor(uint32_t playerId) const {
    return EVAL(sf::Color(0, 0, 0), playerId);
}
sf::Color HighlightColors::getBuildingAttackColor(uint32_t playerId) const {
    return EVAL(sf::Color(200, 0, 0), playerId);
}
sf::Color HighlightColors::getBuildingHealColor(uint32_t playerId) const {
    return EVAL(sf::Color(0, 100, 100), playerId);
}
sf::Color HighlightColors::getWarriorProducingColor(uint32_t playerId) const {
    return EVAL(sf::Color(0, 0, 0), playerId);
}
sf::Color HighlightColors::getResourceCollectingColor(uint32_t playerId) const {
    return EVAL(sf::Color(0, 0, 0), playerId);
}