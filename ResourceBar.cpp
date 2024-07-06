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


#include "ResourceBar.hpp"


ResourceBar::ResourceBar() = default;
void ResourceBar::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::View old = target.getView();
	target.setView(target.getDefaultView());
	this->drawEverything(target, states);
	target.setView(old);
}
void ResourceBar::plus(const Resource& resource) {
	this->resources.plus(resource);
}
void ResourceBar::minus(const Resource& resource) {
	this->resources.minus(resource);
}
void ResourceBar::plus(const Resources& resources2) {
	this->resources.plus(resources2);
}
void ResourceBar::minus(const Resources& resources2) {
	this->resources.minus(resources2);
}
int32_t ResourceBar::get(const std::string& type) const {
	return this->resources.get(type);
}
Resources ResourceBar::getResources() const {
	return this->resources;
}
void ResourceBar::drawEverything(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::RectangleShape rect;
	rect.setPosition(sf::Vector2f(5, 5));
	rect.setSize(sf::Vector2f(target.getSize().x - 10, 26));
	rect.setFillColor(COLOR_THEME::UI_COLOR);
	rect.setOutlineColor(sf::Color::Black);
	rect.setOutlineThickness(2);
	target.draw(rect, states);

	std::string res[5] = { "food", "wood", "stone", "iron", "gold" };
	for (uint32_t i = 0; i < 5; i = i + 1) {
		sf::Sprite sprite;
		sprite.setTexture(*Textures::get()->get(res[i] + "_icon"));
		sprite.setPosition(sf::Vector2f(rect.getPosition().x + rect.getSize().x * i / 5, rect.getPosition().y));
		sprite.setScale(rect.getSize().y / sprite.getTexture()->getSize().x, rect.getSize().y / sprite.getTexture()->getSize().y);
		target.draw(sprite, states);

		sf::Text text;
		text.setFont(*Fonts::get()->get("1"));
		text.setString(std::to_string(this->get(res[i])));
		text.setCharacterSize(16);
		text.setFillColor(sf::Color::White);
		text.setOutlineColor(sf::Color::Black);
		text.setOutlineThickness(1);
		text.setPosition(sf::Vector2f(sprite.getPosition().x + 10 + sprite.getLocalBounds().width, rect.getPosition().y + (rect.getSize().y - text.getLocalBounds().height) / 2));
		target.draw(text, states);
	}
}