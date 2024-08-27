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
#include "ColorBlender.hpp"
#include "Player.hpp"


static sf::Color GET_PLAYER_COLOR(uint32_t playerId) {
    if (playerId == Player::BLUE) {
        return sf::Color(0, 0, 255);
    }
    if (playerId == Player::GREEN) {
        return sf::Color(0, 255, 0);
    }
    if (playerId == Player::PURPLE) {
        return sf::Color(128, 0, 128);
    }
    std::cerr << "HighlightColors: warning: unknown color" << std::endl;
    return {};
}


static sf::Color EVAL(sf::Color c, uint32_t playerId) {
    c = ColorBlender::get().blend(c, GET_PLAYER_COLOR(playerId));
    c.a = 40;
    return c;
}


HighlightColors::HighlightColors() = default;



sf::Color HighlightColors::getWarriorMovementColor(uint32_t playerId) const {
    return EVAL(sf::Color(0, 255, 0), playerId);
}
sf::Color HighlightColors::getWarriorAttackColor(uint32_t playerId) const {
    return EVAL(sf::Color(255, 0, 0), playerId);
}
sf::Color HighlightColors::getWarriorHealColor(uint32_t playerId) const {
    return EVAL(sf::Color(0, 0, 255), playerId);
}
sf::Color HighlightColors::getTerritoryExpandingColor(uint32_t playerId) const {
    return EVAL(sf::Color(255, 192, 203), playerId);
}
sf::Color HighlightColors::getBuildingAttackColor(uint32_t playerId) const {
    return EVAL(sf::Color(255, 0, 0), playerId);
}
sf::Color HighlightColors::getBuildingHealColor(uint32_t playerId) const {
    return EVAL(sf::Color(0, 0, 255), playerId);
}
sf::Color HighlightColors::getWarriorProducingColor(uint32_t playerId) const {
    return EVAL(sf::Color(0, 0, 255), playerId);
}
sf::Color HighlightColors::getResourceCollectingColor(uint32_t playerId) const {
    return EVAL(sf::Color(0, 128, 255), playerId);
}