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
#include "Textures.hpp"
#include "Fonts.hpp"
#include "ColorTheme.hpp"


ResourceBar::ResourceBar() = default;
void ResourceBar::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	this->drawEverything(target, states);
}
void ResourceBar::setResources(const Resources& resources) {
	this->resources = resources;
}
void ResourceBar::setLimit(const Resources& limit) {
	this->limit = limit;
}
void ResourceBar::setPopulation(uint32_t population) {
	this->population = population;
}
void ResourceBar::setPopulationLimit(uint32_t populationLimit) {
	this->populationLimit = populationLimit;
}
void ResourceBar::drawEverything(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::RectangleShape rect;
	rect.setPosition(sf::Vector2f(5, 5));
	rect.setSize(sf::Vector2f(target.getSize().x - 15 - 200, 26));
	rect.setFillColor(COLOR_THEME::UI_COLOR_TRANSPARENT);
	rect.setOutlineColor(sf::Color::Black);
	rect.setOutlineThickness(2);
	target.draw(rect, states);

    std::vector<Resource> res = this->resources.getAll();
	for (uint32_t i = 0; i <= res.size(); i = i + 1) {
		sf::Sprite sprite;
		if (i == res.size()) {
			sprite.setTexture(*Textures::get().get("helmet_icon"));
		}
		else {
			sprite.setTexture(*Textures::get().get(res.at(i).type + "_icon"));
		}
		sprite.setPosition(sf::Vector2f(rect.getPosition().x + rect.getSize().x * i / (res.size() + 1), rect.getPosition().y));
		sprite.setScale(rect.getSize().y / sprite.getTexture()->getSize().x, rect.getSize().y / sprite.getTexture()->getSize().y);
		target.draw(sprite, states);

		sf::Text text;
		text.setFont(*Fonts::get().get("1"));
		if (i == res.size()) {
			text.setString(std::to_wstring(this->population) + L" / " + std::to_wstring(this->populationLimit));
		}
		else {
			text.setString(std::to_string(res.at(i).n) + " / " + std::to_string(this->limit.get(res.at(i).type)));
		}
		text.setCharacterSize(13);
		text.setFillColor(sf::Color::White);
		text.setOutlineColor(sf::Color::Black);
		text.setOutlineThickness(1);
		text.setPosition(sf::Vector2f(sprite.getPosition().x + 10 + sprite.getLocalBounds().width, rect.getPosition().y + (rect.getSize().y - text.getLocalBounds().height) / 2));
		target.draw(text, states);
	}
}