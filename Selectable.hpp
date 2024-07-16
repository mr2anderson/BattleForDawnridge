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


#include <cstdint>
#include <SFML/Graphics.hpp>
#include <tuple>
#include "Events.hpp"


class Selectable {
public:
	virtual Events unselect(uint32_t x, uint32_t y, uint8_t button);
	virtual Events unselect();
	virtual std::shared_ptr<sf::Drawable> getSelectablePointer(uint32_t mouseX, uint32_t mouseY) const = 0;
	std::shared_ptr<sf::Drawable> getSelectablePointer(std::tuple<uint32_t, uint32_t> mousePos) const;
private:
	Events getOnUnselectEvents();
};