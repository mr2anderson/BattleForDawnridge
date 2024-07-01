/*
 *  Conquesta
 *  Copyright (C) 2023 mr2anderson
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
	window.clear(BACKGROUND_COLOR);
	window.display();
}
void LoadingScreen::loadBase() {
	TextureStorage::get()->add("logotype0", "images/logotype/Plant Wind 1_00000.png");
	FontStorage::get()->add("1", "/fonts/1.otf");
}
void LoadingScreen::setNormalScreen(sf::RenderWindow& window) {
	sf::Text text;
	sf::Sprite sprite;

	text.setFont(*FontStorage::get()->get("1"));
	text.setString("Please, wait...");
	text.setCharacterSize(32);
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(2);
	text.setPosition((window.getSize().x - text.getLocalBounds().width) / 2, window.getSize().y - text.getLocalBounds().height - 50);

	sprite.setTexture(*TextureStorage::get()->get("logotype0"));
	sprite.setPosition((window.getSize().x - sprite.getLocalBounds().width) / 2, text.getPosition().y - sprite.getLocalBounds().height - 20);

	window.clear(BACKGROUND_COLOR);
	window.draw(text);
	window.draw(sprite);
	window.display();
}
void LoadingScreen::loadAll() {
	for (uint32_t i = 1; i < 30; i = i + 1) {
		std::string s = std::to_string(i);
		while (s.size() < 5) {
			s = ("0" + s);
		}
		TextureStorage::get()->add("logotype" + std::to_string(i), "images/logotype/Plant Wind 1_" + s + ".png");
	}

	MusicStorage::get()->add("intro", "music/intro.ogg");
	MusicStorage::get()->add("menu", "music/menu.ogg");
	for (uint32_t i = 0; i < 10; i = i + 1) {
		MusicStorage::get()->add(std::to_string(i), "music/ingame_0" + std::to_string(i) + ".ogg");
	}
}