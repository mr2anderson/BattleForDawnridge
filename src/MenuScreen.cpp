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
	MusicStorage::get()->get("intro")->stop();
	Playlist::get()->restartMusic();
	MusicStorage::get()->get("menu")->play();

	Button start2on1pc(10, 10, 400, 60, "To battle (2 players 1 PC)", 24);
	Button exit(10, 80, 400, 60, "Exit", 24);

	sf::Event event{};

	for (; ;) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed or (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Escape)) {
				return -1;
			}
			if (event.type == sf::Event::MouseButtonPressed) {
				auto pos = sf::Mouse::getPosition();
				if (start2on1pc.click(pos.x, pos.y)) {
					return 0;
				}
				if (exit.click(pos.x, pos.y)) {
					return -1;
				}
			}
		}
		window.clear(BACKGROUND_COLOR);
		window.draw(start2on1pc);
		window.draw(exit);
		window.display();
	}
}