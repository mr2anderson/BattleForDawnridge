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


#include "BattleScreen.hpp"


BattleScreen* BattleScreen::singletone = nullptr;


int32_t BattleScreen::run(sf::RenderWindow& window) {
	this->initGameLogick();
	return this->start(window);
}
void BattleScreen::initGameLogick() {
	this->mapWidth = 50;
	this->mapHeight = 50;
	this->popUpWindow = nullptr;
	this->players[0] = Player(1);
	this->players[1] = Player(2);
	this->move = 1;
}
int32_t BattleScreen::start(sf::RenderWindow& window) {
	sf::Event event{};

	Button endMove(window.getSize().x - 20 - 150, window.getSize().y - 20 - 30, 150, 30, L"Конец хода", 18);

	this->view = window.getDefaultView();
	
	this->popUpWindow = new MessageWindow(window.getSize().x, window.getSize().y, L"Мастерская должна иметь хотя-бы 80% прочности, чтобы работать");

	for (; ;) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				return -1;
			}
			if (event.type == sf::Event::KeyPressed) {
				auto code = event.key.code;
				if (code == sf::Keyboard::Escape) {
					window.setView(window.getDefaultView());
					Playlist::get()->restartMusic();
					return 0;
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				if (this->popUpWindow == nullptr) {
					if (endMove.click(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y)) {
						this->newMove();
					}
				}
				else {
					if (event.type == sf::Event::MouseButtonPressed) {
						handlePopUpWindowEvent(this->popUpWindow->click(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y));
					}
				}
			}
		}

		window.clear(BACKGROUND_COLOR);
		window.setView(this->view);
		this->drawCells(window);
		if (this->popUpWindow != nullptr) {
			window.draw(*this->popUpWindow);
		}
		window.draw(*this->getCurrentPlayer()->getConstResourceBarPtr());
		window.draw(endMove);
		window.display();

		Playlist::get()->update();

		if (this->popUpWindow == nullptr) {
			auto pos = sf::Mouse::getPosition();
			if (pos.x < 10 or sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				this->viewToWest(window.getSize().x);
			}
			else if (pos.x > window.getSize().x - 10 or sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				this->viewToEast(window.getSize().x);
			}
			if (pos.y < 10 or sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				this->viewToNorth(window.getSize().y);
			}
			else if (pos.y > window.getSize().y - 10 or sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				this->viewToSouth(window.getSize().y);
			}
		}
	}
}
void BattleScreen::handleGameEvent(GameEvent event) {
	if (event.sound.has_value()) {
		SoundQueue::get()->push(SoundStorage::get()->get(event.sound.value()));
	}
}
void BattleScreen::handlePopUpWindowEvent(PopUpWindowEvent event) {
	if (event.close) {
		delete this->popUpWindow;
		this->popUpWindow = nullptr;
	}
	this->handleGameEvent(event.gameEvent);
}
void BattleScreen::newMove() {
	this->move = this->move + 1;
	SoundQueue::get()->push(SoundStorage::get()->get("newMove"));
}
Player* BattleScreen::getCurrentPlayer() {
	return &this->players[(move - 1) % 2];
}
void BattleScreen::drawCells(sf::RenderWindow &window) {
	for (uint32_t i = 0; i < this->mapWidth; i = i + 1) {
		for (uint32_t j = 0; j < this->mapHeight; j = j + 1) {
			if ((i + j) % 2 == 0) {
				sf::RectangleShape rect;
				rect.setPosition(sf::Vector2f(48 * i, 48 * j));
				rect.setSize(sf::Vector2f(48, 48));
				rect.setFillColor(CELL_COLOR);
				window.draw(rect);
			}
		}
	}
}
void BattleScreen::viewToNorth(uint32_t windowH) {
	float delta = std::min(5.f, view.getCenter().y - windowH / 2);
	view.setCenter(view.getCenter() - sf::Vector2f(0, delta));
}
void BattleScreen::viewToSouth(uint32_t windowH) {
	float delta = std::min(5.f, 48 * this->mapHeight - windowH / 2 - view.getCenter().y);
	view.setCenter(view.getCenter() + sf::Vector2f(0, delta));
}
void BattleScreen::viewToWest(uint32_t windowW) {
	float delta = std::min(5.f, view.getCenter().x - windowW / 2);
	view.setCenter(view.getCenter() - sf::Vector2f(delta, 0));
}
void BattleScreen::viewToEast(uint32_t windowW) {
	float delta = std::min(5.f, 48 * this->mapWidth - windowW / 2 - view.getCenter().x);
	view.setCenter(view.getCenter() + sf::Vector2f(delta, 0));
}