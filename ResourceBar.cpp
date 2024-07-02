/*
 *  Conquesta
 *  Copyright (C) 2024 mr2anderson
 *
 *  Conquesta is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Conquesta is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Conquesta.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "ResourceBar.hpp"


ResourceBar::ResourceBar() = default;
void ResourceBar::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::View old = target.getView();
	target.setView(target.getDefaultView());

	sf::RectangleShape rect;
	rect.setPosition(sf::Vector2f(5, 5));
	rect.setSize(sf::Vector2f(target.getSize().x - 10, 64));
	rect.setFillColor(UI_COLOR);
	rect.setOutlineColor(sf::Color::Black);
	rect.setOutlineThickness(2);
	target.draw(rect, states);

	std::array<std::string, 5> res = { "food", "wood", "stone", "iron", "gold" };
	for (uint32_t i = 0; i < 5; i = i + 1) {
		sf::Sprite sprite;
		sprite.setTexture(*TextureStorage::get()->get(res[i]));
		sprite.setPosition(sf::Vector2f(rect.getPosition().x + rect.getSize().x * i / 5, rect.getPosition().y));
		target.draw(sprite, states);

		sf::Text text;
		text.setFont(*FontStorage::get()->get("1"));
		text.setString(std::to_string(this->get(res[i])));
		text.setCharacterSize(16);
		text.setFillColor(sf::Color::White);
		text.setOutlineColor(sf::Color::Black);
		text.setOutlineThickness(1);
		text.setPosition(sf::Vector2f(sprite.getPosition().x + 10 + sprite.getLocalBounds().width, rect.getPosition().y + (rect.getSize().y - text.getLocalBounds().height) / 2));
		target.draw(text, states);
	}

	target.setView(old);
}
void ResourceBar::plus(const Resource &resource) {
	this->map[resource.type] += resource.n;
}
void ResourceBar::minus(const Resource &resource) {
	this->map[resource.type] -= resource.n;
}
int32_t ResourceBar::get(const std::string &id) const {
	if (this->map.find(id) == this->map.end()) {
		return 0;
	}
	return this->map.at(id);
}