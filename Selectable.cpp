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


#include "Selectable.hpp"
#include "UnselectEvent.hpp"
#include "Textures.hpp"
#include "EnableCursorEvent.hpp"


Events Selectable::unselect(uint32_t x, uint32_t y) {
	return this->getOnUnselectEvents();
}
Events Selectable::unselect() {
	return this->getOnUnselectEvents();
}
sf::Sprite Selectable::getSprite(uint32_t mouseX, uint32_t mouseY) {
	sf::Sprite sprite;
	sprite.setPosition(mouseX / 32 * 32, mouseY / 32 * 32);
	sprite.setTexture(*Textures::get()->get(this->getSelectableTextureName()));
	return sprite;
}
sf::Sprite Selectable::getSprite(std::tuple<uint32_t, uint32_t> mousePos) {
	return this->getSprite(std::get<0>(mousePos), std::get<1>(mousePos));
}
Events Selectable::getOnUnselectEvents() {
	Events events;
	events.add(std::make_shared<UnselectEvent>());
	events.add(std::make_shared<EnableCursorEvent>());
	return events;
}