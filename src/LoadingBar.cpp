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


#include "LoadingBar.hpp"
#include "Textures.hpp"


LoadingBar::LoadingBar() {
	this->x = 0;
	this->y = 0;
	this->value = 0;
}
void LoadingBar::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::Sprite main;
	main.setPosition(this->x, this->y);
	main.setTexture(*Textures::get().get("loading_bar0"));
	main.setOrigin(main.getGlobalBounds().width / 2, main.getGlobalBounds().height / 2);

	sf::Sprite sub;
	sub.setPosition(main.getPosition().x - main.getGlobalBounds().width / 2, main.getPosition().y - main.getGlobalBounds().height / 2);
	sub.setTexture(*Textures::get().get("loading_bar1"));
	sub.setTextureRect(sf::IntRect(0, 0, sub.getGlobalBounds().width * this->value / 100, sub.getGlobalBounds().height));

	target.draw(main, states);
	target.draw(sub, states);
}
void LoadingBar::setCenter(int32_t x, int32_t y) {
	this->x = x;
	this->y = y;
}
void LoadingBar::setValue(uint32_t value) {
	this->value = value;
}