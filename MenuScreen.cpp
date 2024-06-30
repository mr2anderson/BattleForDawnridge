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


#include "MenuScreen.hpp"


MenuScreen* MenuScreen::singletone = nullptr;


int32_t MenuScreen::run(sf::RenderWindow& window) {
	return this->getConnection(window);
}
int32_t MenuScreen::getConnection(sf::RenderWindow& window) {
	sf::Event event{};

	for (; ;) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed or (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Escape)) {
				return -1;
			}
			if (event.type == sf::Event::KeyPressed) {
				return 0;
			}
		}
		window.clear(sf::Color::Yellow);
		window.display();
	}
}