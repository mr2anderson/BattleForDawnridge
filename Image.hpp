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


#include "RectangularUIElement.hpp"
#include "Textures.hpp"


#pragma once


class Image : public RectangularUiElement {
public:
    Image();
    Image(int32_t x, int32_t y, const std::string& imageName); // creating image with default size
    Image(int32_t x, int32_t y, uint32_t h, const std::string &imageName); // creating image making it fit in specified height by scaling
    Image(int32_t x, int32_t y, uint32_t w, uint32_t h, bool left, bool up, const std::string& imageName); // creating image cropping it to specific size

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void setX(int32_t newX) override;
    void setY(int32_t newY) override;
private:
    sf::Sprite sprite;

    void rerenderBasedOnPosition();
};