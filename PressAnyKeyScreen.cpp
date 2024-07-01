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


#include "PressAnyKeyScreen.hpp"


PressAnyKeyScreen* PressAnyKeyScreen::singletone = nullptr;


int32_t PressAnyKeyScreen::run(sf::RenderWindow &window) {
	return this->wait(window);
}
int32_t PressAnyKeyScreen::wait(sf::RenderWindow &window) {
	MusicStorage::get()->get("intro")->play();

	sf::Text text;
	sf::Sprite sprite;
	sf::Clock clock;

	text.setFont(*FontStorage::get()->get("1"));
	text.setString(L"Нажмите на любую клавишу для продолжения...");
	text.setCharacterSize(32);
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(2);
	text.setPosition((window.getSize().x - text.getLocalBounds().width) / 2, window.getSize().y - text.getLocalBounds().height - 50);

	sprite.setTexture(*TextureStorage::get()->get("logotype0"));
	sprite.setPosition((window.getSize().x - sprite.getLocalBounds().width) / 2, text.getPosition().y - sprite.getLocalBounds().height - 20);

	sf::Event event{};

	for (; ;) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				return -1;
			}
			if (event.type == sf::Event::KeyPressed) {
				auto code = event.key.code;
				if (code == sf::Keyboard::Escape) {
					return -1;
				}
				MusicStorage::get()->get("intro")->stop();
				return 0;
			}
		}

		window.clear(BACKGROUND_COLOR);
		window.draw(text);
		window.draw(sprite);
		window.display();

		sprite.setTexture(*TextureStorage::get()->get("logotype" + std::to_string(clock.getElapsedTime().asMilliseconds() / 30 % 30)));
	}
}