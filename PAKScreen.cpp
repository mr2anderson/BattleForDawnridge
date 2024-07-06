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


#include "PAKScreen.hpp"


PAKScreen* PAKScreen::singletone = nullptr;


bool PAKScreen::run(sf::RenderWindow &window) {
	Music::get()->get("intro")->play();
	this->initGraphics(window.getSize().x, window.getSize().y);
	sf::Clock clock;
	sf::Event event{};
	for (; ;) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				return false;
			}
			if (event.type == sf::Event::KeyPressed) {
				auto code = event.key.code;
				if (code == sf::Keyboard::Escape) {
					return false;
				}
				Music::get()->get("intro")->stop();
				return true;
			}
		}
		this->drawEverything(window);
	}
}
void PAKScreen::initGraphics(uint32_t windowW, uint32_t windowH) {
	this->t.setFont(*Fonts::get()->get("1"));
	this->t.setString(*Texts::get()->get("press_any_key"));
	this->t.setCharacterSize(32);
	this->t.setFillColor(sf::Color::White);
	this->t.setOutlineColor(sf::Color::Black);
	this->t.setOutlineThickness(2);
	this->t.setPosition((windowW - t.getLocalBounds().width) / 2, windowH - t.getLocalBounds().height - 50);

}
void PAKScreen::drawEverything(sf::RenderWindow& window) {
	window.clear(COLOR_THEME::UI_COLOR);
	window.draw(this->t);
	window.display();
}