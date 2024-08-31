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
#include <cstdint>


#pragma once


class LoadingBar : public sf::Drawable {
public:
	LoadingBar();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setCenter(int32_t x, int32_t y);
	void setValue(uint32_t value);
private:
	int32_t x, y;
	uint32_t value;
};