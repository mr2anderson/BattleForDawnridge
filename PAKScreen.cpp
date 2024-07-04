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


#include "PAKScreen.hpp"


PAKScreen* PAKScreen::singletone = nullptr;


int32_t PAKScreen::run(sf::RenderWindow &window) {
	Music::get()->get("intro")->play();
	this->initGraphics(window.getSize().x, window.getSize().y);
	sf::Clock clock;
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
				Music::get()->get("intro")->stop();
				return 0;
			}
		}
		this->drawEverything(window);
	}
}
void PAKScreen::initGraphics(uint32_t windowW, uint32_t windowH) {
	this->t.setFont(*Fonts::get()->get("1"));
	this->t.setString(L"������� �� ����� ������� ��� �����������...");
	this->t.setCharacterSize(32);
	this->t.setFillColor(sf::Color::White);
	this->t.setOutlineColor(sf::Color::Black);
	this->t.setOutlineThickness(2);
	this->t.setPosition((windowW - t.getLocalBounds().width) / 2, windowH - t.getLocalBounds().height - 50);

	this->s.setTexture(*Textures::get()->get("logotype0"));
	this->s.setPosition((windowW - s.getLocalBounds().width) / 2, t.getPosition().y - s.getLocalBounds().height - 20);
}
void PAKScreen::drawEverything(sf::RenderWindow& window) {
	window.clear(UI_COLOR);
	window.draw(this->t);
	window.draw(this->s);
	window.display();
}