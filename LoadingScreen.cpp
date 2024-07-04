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


#include "LoadingScreen.hpp"


LoadingScreen* LoadingScreen::singletone = nullptr;


void LoadingScreen::run(sf::RenderWindow &window) {
	this->setBaseScreen(window);
	this->loadBase();

	this->setNormalScreen(window);
	this->loadAll();
}
void LoadingScreen::setBaseScreen(sf::RenderWindow &window) {
	window.clear(UI_COLOR);
	window.display();
}
void LoadingScreen::loadBase() {
	Fonts::get()->add("1", "/fonts/1.ttf");
}
void LoadingScreen::setNormalScreen(sf::RenderWindow& window) {
	sf::Text t;
	t.setFont(*Fonts::get()->get("1"));
	t.setString(L"Пожалуйста, подождите");
	t.setCharacterSize(32);
	t.setFillColor(sf::Color::White);
	t.setOutlineColor(sf::Color::Black);
	t.setOutlineThickness(2);
	t.setPosition((window.getSize().x - t.getLocalBounds().width) / 2, window.getSize().y - t.getLocalBounds().height - 50);

	window.clear(UI_COLOR);
	window.draw(t);
	window.display();
}
void LoadingScreen::loadAll() {
	for (const std::string& a : { 
		"castle", "exit_icon", "food_icon", "forest", "gold_icon", "iron", 
		"market", "mine", "quarry", "sawmill", "stone", "stone_icon", "upgrade_icon", 
		"farm", "wood_icon", "iron_icon", "shield_icon", "hammer_icon", "trade_icon",
		"wall"}) {
		Textures::get()->add(a, "images/" + a + ".png");
	}
	for (uint32_t i = 1; i <= PlainsGeneration::TOTAL_PLAINS; i = i + 1) {
		Textures::get()->add(std::to_string(i), "images/" + std::to_string(i) + ".png");
	}
	for (const std::string& a : { "click", "food", "gold", "hooray", "iron", 
		"regeneration", "stone", "wood"}) {
		Sounds::get()->add(a, "sounds/" + a + ".ogg");
	}
	Music::get()->add("intro", "music/intro.ogg");
	Music::get()->add("menu", "music/menu.ogg");
	for (uint32_t i = 0; i < 10; i = i + 1) {
		Music::get()->add(std::to_string(i), "music/ingame_0" + std::to_string(i) + ".ogg");
	}
}