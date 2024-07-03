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
					SoundQueue::get()->push(Sounds::get()->get("click"));
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
	this->license = Label(10, windowH - 10 - 315, 500, 315, std::nullopt,
		L"Conquesta\n"
		"Copyright (C) 2024 mr2anderson\n\n"
		"Conquesta is free software: you can redistribute it and/or modify\n"
		"it under the terms of the GNU General Public License as published by\n"
		"he Free Software Foundation, either version 3 of the License, or\n"
		"(at your option) any later version.\n"
		"Conquesta is distributed in the hope that it will be useful,\n"
		"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
		"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
		"GNU General Public License for more details.\n\n"
		"You should have received a copy of the GNU General Public License\n"
		"along with Conquesta.  If not, see <http://www.gnu.org/licenses/>.\n\n"
		"Если Вам нравится Conquesta, то Вы можете поддержать\n"
		"разработчиков, отправив небольшую сумму на этот BTC кошелек:\n"
		"bc1ql9np8zlq487lmxv60v9ukrp83gtjthvxupwexs");
	this->start2on1pc = Button(10, 10, 400, 60, std::nullopt, L"Начать (2 игрока 1 ПК)");
	this->exit = Button(10, 80, 400, 60, std::nullopt, L"Завершить");

	this->title.setFont(*Fonts::get()->get("1"));
	this->title.setString("CONQUESTA");
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