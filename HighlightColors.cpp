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


HighlightColors* HighlightColors::singletone = nullptr;


static const sf::Color BLUE = sf::Color(0, 191, 255); // Bright colors were chosen cuz it is easier to blend them
static const sf::Color GREEN = sf::Color(154, 205, 50);
static const sf::Color PURPLE = sf::Color(238, 130, 238);


static sf::Color GET_PLAYER_COLOR(uint32_t playerId) {
    if (playerId == Player::BLUE) {
        return BLUE;
    }
    if (playerId == Player::GREEN) {
        return GREEN;
    }
    if (playerId == Player::PURPLE) {
        return PURPLE;
    }
    std::cerr << "HighlightColors: warning: unknown color" << std::endl;
    return {};
}


static sf::Color EVAL(sf::Color c, uint32_t playerId) {
    c = ColorBlender::get()->blend(c, GET_PLAYER_COLOR(playerId));
    c.a = 35;
    return c;
}


HighlightColors::HighlightColors() = default;


static const sf::Color SPRING_GREEN = sf::Color(0, 255, 127);
static const sf::Color ORANGE_RED = sf::Color(255, 69, 0);
static const sf::Color AQUA = sf::Color(0, 255, 255);
static const sf::Color PERU = sf::Color(205, 133, 63);
static const sf::Color RED = sf::Color(255, 0, 0);
static const sf::Color TURQUOISE = sf::Color(64, 224, 208);
static const sf::Color GOLDEN_ROD = sf::Color(218, 165, 32);
static const sf::Color OLIVE_DRAB = sf::Color(107, 142, 35);


sf::Color HighlightColors::getWarriorMovementColor(uint32_t playerId) const {
    return EVAL(SPRING_GREEN, playerId);
}
sf::Color HighlightColors::getWarriorAttackColor(uint32_t playerId) const {
    return EVAL(ORANGE_RED, playerId);
}
sf::Color HighlightColors::getWarriorHealColor(uint32_t playerId) const {
    return EVAL(AQUA, playerId);
}
sf::Color HighlightColors::getTerritoryExpandingColor(uint32_t playerId) const {
    return EVAL(PERU, playerId);
}
sf::Color HighlightColors::getBuildingAttackColor(uint32_t playerId) const {
    return EVAL(RED, playerId);
}
sf::Color HighlightColors::getBuildingHealColor(uint32_t playerId) const {
    return EVAL(TURQUOISE, playerId);
}
sf::Color HighlightColors::getWarriorProducingColor(uint32_t playerId) const {
    return EVAL(GOLDEN_ROD, playerId);
}
sf::Color HighlightColors::getResourceCollectingColor(uint32_t playerId) const {
    return EVAL(OLIVE_DRAB, playerId);
}