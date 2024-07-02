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
	FontStorage::get()->add("1", "/fonts/1.ttf");
}
void LoadingScreen::setNormalScreen(sf::RenderWindow& window) {
	sf::Text text;

	text.setFont(*FontStorage::get()->get("1"));
	text.setString(L"Пожалуйста, подождите");
	text.setCharacterSize(32);
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(2);
	text.setPosition((window.getSize().x - text.getLocalBounds().width) / 2, window.getSize().y - text.getLocalBounds().height - 50);

	window.clear(UI_COLOR);
	window.draw(text);
	window.display();
}
void LoadingScreen::loadAll() {
	for (const std::string& a : { 
		"castle", "cell", "exit_icon", "food_icon", "forest", "gold_icon", "iron", 
		"market", "mine", "quarry", "sawmill", "stone", "stone_icon", "upgrade_icon", 
		"windmill", "wood_icon", "iron_icon"}) {
		TextureStorage::get()->add(a, "images/" + a + ".png");
	}
	for (const std::string& a : { "click", "newMove", "leaves", "sawmill", "quarry", "mine", "hooray", "horse"}) {
		SoundStorage::get()->add(a, "sounds/" + a + ".ogg");
	}
	MusicStorage::get()->add("intro", "music/intro.ogg");
	MusicStorage::get()->add("menu", "music/menu.ogg");
	for (uint32_t i = 0; i < 10; i = i + 1) {
		MusicStorage::get()->add(std::to_string(i), "music/ingame_0" + std::to_string(i) + ".ogg");
	}
}