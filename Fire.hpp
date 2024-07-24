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


#include <SFML/Graphics.hpp>


#pragma once


class Fire : public sf::Drawable {
public:
	Fire();
	Fire(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	static const uint32_t TOTAL_FRAMES;
private:
	float x, y;
	float scaleX, scaleY;

	uint32_t getCurrentFrame() const;
};