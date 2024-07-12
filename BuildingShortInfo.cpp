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


#include "BuildingShortInfo.hpp"
#include "Textures.hpp"
#include "Fonts.hpp"


BuildingShortInfo::BuildingShortInfo(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, const std::string& pictureName, const std::string& text) {
	this->rect.setFillColor(sf::Color::Black);
	this->rect.setSize(sf::Vector2f(64 * sx, 64 * sy / 2));
	this->rect.setPosition(64 * x, 64 * y + 64 * sy / 2);

	this->sprite.setTexture(*Textures::get()->get(pictureName));
	this->sprite.setScale(rect.getSize().y / sprite.getTexture()->getSize().x, rect.getSize().y / sprite.getTexture()->getSize().y);
	this->sprite.setPosition(rect.getPosition());

	this->text.setFont(*Fonts::get()->get("1"));
	this->text.setString(text);
	this->text.setCharacterSize(25);
	this->text.setFillColor(sf::Color::White);
	this->text.setOutlineColor(sf::Color::Black);
	this->text.setOutlineThickness(1);
	this->text.setPosition(sprite.getPosition() + sf::Vector2f(rect.getSize().x / 2 + (rect.getSize().x / 2 + this->text.getLocalBounds().width) / 2, (rect.getSize().y / 2 + this->text.getLocalBounds().height) / 2));
}
void BuildingShortInfo::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(this->rect, states);
	target.draw(this->sprite, states);
	target.draw(this->text, states);
}