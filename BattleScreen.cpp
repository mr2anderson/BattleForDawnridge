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


#include "BattleScreen.hpp"


BattleScreen* BattleScreen::singletone = nullptr;


int32_t BattleScreen::run(sf::RenderWindow& window) {
	sf::Event event{};

	HPBar a(0, 1000, 5, 5);
	MessageWindow w(1920, 1080, "House was damaged or is not built yet");
	bool wClosed = false;

	for (; ;) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed or (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Escape)) {
				return -1;
			}
			if (event.type == sf::Event::MouseButtonPressed) {
				auto pos = sf::Mouse::getPosition();
				if (w.click(pos.x, pos.y) == "close") {
					wClosed = true;
				}
			}
		}
		window.clear(BACKGROUND_COLOR);
		window.draw(a);
		if (!wClosed) {
			window.draw(w);
		}
		a = a + 1;
		window.display();
	}
}