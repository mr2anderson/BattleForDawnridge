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


#include "Menu.hpp"


Menu* Menu::singletone = nullptr;


int32_t Menu::run(sf::RenderWindow& window) {
	Music::get()->get("menu")->play();
	if (!this->graphicsInited) {
		this->initGraphics(window.getSize().x, window.getSize().y);
	}
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
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				auto pos = sf::Mouse::getPosition();
				if (start2on1pc.click(pos.x, pos.y)) {
					Music::get()->get("menu")->stop();
					return 0;
				}
				if (exit.click(pos.x, pos.y)) {
					return -1;
				}
			}
		}
		this->drawEverything(window);
	}
}
void Menu::initGraphics(uint32_t windowW, uint32_t windowH) {
	this->graphicsInited = true;
	this->license = Label(10, windowH - 10 - 280, 500, 280, std::nullopt, *Texts::get()->get("license"));
	this->start2on1pc = Button(10, 10, 400, 60, std::nullopt, *Texts::get()->get("start_game_2p_1pc"));
	this->exit = Button(10, 80, 400, 60, std::nullopt, *Texts::get()->get("exit"));

	this->title.setFont(*Fonts::get()->get("1"));
	this->title.setString(*Texts::get()->get("title"));
	this->title.setCharacterSize(60);
	this->title.setFillColor(sf::Color::White);
	this->title.setPosition(windowW - 40 - title.getLocalBounds().width, windowH - 40 - title.getLocalBounds().height);
}
void Menu::drawEverything(sf::RenderWindow &window) {
	window.clear(UI_COLOR);
	window.draw(this->license);
	window.draw(this->start2on1pc);
	window.draw(this->exit);
	window.draw(this->title);
	window.display();
}