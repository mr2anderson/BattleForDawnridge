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


#include "MenuScreen.hpp"


MenuScreen* MenuScreen::singletone = nullptr;


int32_t MenuScreen::run(sf::RenderWindow& window) {
	MusicStorage::get()->get("menu")->play();

	Label license(10, window.getSize().y - 10 - 315, 500, 315, L"Conquesta\nCopyright (C) 2024 mr2anderson\n\nConquesta is free software: you can redistribute it and/or modify\nit under the terms of the GNU General Public License as published by\nhe Free Software Foundation, either version 3 of the License, or\n(at your option) any later version.\nConquesta is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\nGNU General Public License for more details.\n\nYou should have received a copy of the GNU General Public License\nalong with Conquesta.  If not, see <http://www.gnu.org/licenses/>.\n\nЕсли Вам понравился этот проект, то Вы можете поддержать\nразработчиков, отправив небольшую сумму на этот BTC кошелек:\nbc1ql9np8zlq487lmxv60v9ukrp83gtjthvxupwexs", 14);
	Button start2on1pc(10, 10, 400, 60, L"Начать (2 игрока 1 ПК)", 24);
	Button exit(10, 80, 400, 60, L"Завершить", 24);
	sf::Sprite sprite;
	sf::Text title;

	title.setFont(*FontStorage::get()->get("1"));
	title.setString("CONQUESTA");
	title.setCharacterSize(60);
	title.setFillColor(sf::Color::White);
	title.setPosition(window.getSize().x - 40 - title.getLocalBounds().width, window.getSize().y - 40 - title.getLocalBounds().height);

	sf::Clock clock;
	sprite.setTexture(*TextureStorage::get()->get("logotype0"));
	sprite.setPosition(sf::Vector2f(title.getPosition().x, title.getPosition().y - sprite.getLocalBounds().height));

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
					SoundQueue::get()->push(SoundStorage::get()->get("click"));
					MusicStorage::get()->get("menu")->stop();
					return 0;
				}
				if (exit.click(pos.x, pos.y)) {
					return -1;
				}
			}
		}

		window.clear(BACKGROUND_COLOR);
		window.draw(license);
		window.draw(start2on1pc);
		window.draw(exit);
		window.draw(title);
		window.draw(sprite);
		window.display();

		sprite.setTexture(*TextureStorage::get()->get("logotype" + std::to_string(clock.getElapsedTime().asMilliseconds() / 30 % 30)));
	}
}