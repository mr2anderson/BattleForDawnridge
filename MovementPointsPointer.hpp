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


#include "PlayerPointer.hpp"


#pragma once


class MovementPointsPointer : public PlayerPointer {
public:
    MovementPointsPointer(float xInPixels, float yInPixels, uint32_t value);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
private:
    sf::Text text;

    static const sf::Color BLUE;
    static const sf::Color GREEN;
    static const sf::Color PURPLE;

    void setTypeBlue() override;
    void setTypeGreen() override;
    void setTypePurple() override;
};