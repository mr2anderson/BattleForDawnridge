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


class PlayerPointerCircle : public PlayerPointer {
public:
    enum ORIENTATION {
        LEFT_UP,
        RIGHT_UP
    };

    PlayerPointerCircle(float xInPixels, float yInPixels, uint32_t sx, uint32_t sy, uint8_t orientation = ORIENTATION::RIGHT_UP);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
private:
    float xInPixels, yInPixels;
    uint32_t sx;
    uint8_t orientation;
    std::string type;

    void setTypeBlue() override;
    void setTypeGreen() override;
    void setTypePurple() override;
    float getCurrentScale() const;
    sf::Vector2f getCurrentPosition() const;
};