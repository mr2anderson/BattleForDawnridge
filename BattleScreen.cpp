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
	this->init(window);
	return this->start(window);
}
void BattleScreen::init(sf::RenderWindow& window) {
	this->initLandscape();
	this->removeOldPopUpWindows();
	this->initPlayers();
	this->initMoveCtr();
	this->initGraphics(window);
}
void BattleScreen::initLandscape() {
	this->mapWidth = 50;
	this->mapHeight = 50;
	for (uint32_t i = 0; i < this->gameObjects.size(); i = i + 1) {
		delete this->gameObjects[i];
	}
	this->gameObjects.clear();
	this->gameObjects.push_back(new Fort(1, 1, &this->players[0]));
	this->gameObjects.push_back(new Caravan(1, 2, &this->players[0]));
	this->gameObjects.push_back(new Caravan(2, 1, &this->players[0]));
	this->gameObjects.push_back(new Caravan(2, 2, &this->players[0]));
	this->gameObjects.push_back(new Fort(this->mapWidth - 2, this->mapHeight - 2, &this->players[1]));
	this->gameObjects.push_back(new Plant(4, 4));
	this->gameObjects.push_back(new Plant(4, 5));
	this->gameObjects.push_back(new Plant(5, 4));
	this->gameObjects.push_back(new Plant(5, 5));
	this->gameObjects.push_back(new Tree(1, 3));
	this->gameObjects.push_back(new Tree(1, 4));
	this->gameObjects.push_back(new Mountain(2, 3));
	this->gameObjects.push_back(new Mountain(2, 4));
	this->gameObjects.push_back(new RedMountain(3, 3));
	this->gameObjects.push_back(new RedMountain(3, 4));
}
void BattleScreen::removeOldPopUpWindows() {
	while (!this->popUpWindows.empty()) {
		PopUpWindow* window = this->popUpWindows.front();
		delete window;
		this->popUpWindows.pop();
	}
}
void BattleScreen::initPlayers() {
	this->players[0] = Player(1);
	this->players[1] = Player(2);
}
void BattleScreen::initMoveCtr() {
	this->move = 1;
}
void BattleScreen::initGraphics(sf::RenderWindow &window) {
	this->windowW = window.getSize().x;
	this->windowH = window.getSize().y;
	this->view = window.getDefaultView();
	this->endMove = Button(this->windowW - 20 - 150, this->windowH - 20 - 30, 150, 30, L"Конец хода", 18);
}
int32_t BattleScreen::start(sf::RenderWindow& window) {
	sf::Event event{};
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
				if (this->popUpWindows.empty()) {
					if (endMove.click(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y)) {
						this->newMove();
					}
					else {
						uint32_t mouseX = sf::Mouse::getPosition().x + this->view.getCenter().x - this->windowW / 2;
						uint32_t mouseY = sf::Mouse::getPosition().y + this->view.getCenter().y - this->windowH / 2;
						for (uint32_t i = 0; i < this->gameObjects.size(); i = i + 1) {
							GameObjectResponse response = this->gameObjects[i]->click(*this->getCurrentPlayer(), mouseX, mouseY, window.getSize().x, window.getSize().y);
							if (response.gameEvent.has_value()) {
								handleGameEvent(response.gameEvent.value());
							}
							std::queue<PopUpWindow*> popUpWindows2 = response.popUpWindows;
							if (!popUpWindows2.empty()) {
								while (!popUpWindows2.empty()) {
									this->popUpWindows.push(popUpWindows2.front());
									popUpWindows2.pop();
								}
								this->popUpWindows.front()->run(window.getSize().x, window.getSize().y);
							}
						}
					}
				}
				else {
					if (event.type == sf::Event::MouseButtonPressed) {
						handlePopUpWindowEvent(this->popUpWindows.front()->click(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y));
					}
				}
			}
		}

		window.clear(BACKGROUND_COLOR);
		window.setView(this->view);
		this->drawCells(window);
		for (uint32_t i = 0; i < this->gameObjects.size(); i = i + 1) {
			window.draw(*this->gameObjects[i]);
		}
		if (!this->popUpWindows.empty()) {
			window.draw(*this->popUpWindows.front());
		}
		window.draw(*this->getCurrentPlayer()->getConstResourceBarPtr());
		window.draw(endMove);
		window.display();

		Playlist::get()->update();

		if (this->popUpWindows.empty()) {
			auto pos = sf::Mouse::getPosition();
			if (pos.x < 10 or sf::Keyboard::isKeyPressed(sf::Keyboard::A) or sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				this->viewToWest();
			}
			else if (pos.x > this->windowW - 10 or sf::Keyboard::isKeyPressed(sf::Keyboard::D) or sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				this->viewToEast();
			}
			if (pos.y < 10 or sf::Keyboard::isKeyPressed(sf::Keyboard::W) or sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				this->viewToNorth();
			}
			else if (pos.y > this->windowH - 10 or sf::Keyboard::isKeyPressed(sf::Keyboard::S) or sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				this->viewToSouth();
			}
		}
	}
}
void BattleScreen::handleGameEvent(GameEvent event) {
	if (event.tryToAttack.has_value()) {
		std::cout << "tryToAttack " << event.tryToAttack.value()->getX() << " " << event.tryToAttack.value()->getY() << std::endl;
	}
	if (event.tryToTrade.has_value()) {
		Caravan* caravan = std::get<Caravan*>(event.tryToTrade.value());
		Trade trade = std::get<Trade>(event.tryToTrade.value());
		if (this->getCurrentPlayer()->getResource(trade.sell.type) >= trade.sell.n) {
			GameObjectResponse response = caravan->doTrade(trade);
			if (response.gameEvent.has_value()) {
				this->handleGameEvent(response.gameEvent.value());
			}
			std::queue<PopUpWindow*> popUpWindows2 = response.popUpWindows;
			bool free = this->popUpWindows.empty();
			if (!popUpWindows2.empty()) {
				while (!popUpWindows2.empty()) {
					this->popUpWindows.push(popUpWindows2.front());
					popUpWindows2.pop();
				}
				if (free) {
					this->popUpWindows.front()->run(this->windowW, this->windowH);
				}
			}
		}
		else {
			MessageWindow* window = new MessageWindow("click", "click", L"Недостаточно ресурсов\nВы не можете совершить эту сделку.");
			this->popUpWindows.push(window);
			if (this->popUpWindows.size() == 1) {
				this->popUpWindows.front()->run(this->windowW, this->windowH);
			}
		}
	}
	if (event.startTrade.has_value()) {
		this->getCurrentPlayer()->subResource(event.startTrade.value());
	}
	if (event.finishTrade.has_value()) {
		this->getCurrentPlayer()->addResource(event.finishTrade.value());
	}
}
void BattleScreen::handlePopUpWindowEvent(PopUpWindowEvent event) {
	if (event.close) {
		delete this->popUpWindows.front();
		this->popUpWindows.pop();
		if (!this->popUpWindows.empty()) {
			this->popUpWindows.front()->run(this->windowW, this->windowH);
		}
	}
	this->handleGameEvent(event.gameEvent);
}
void BattleScreen::newMove() {
	this->move = this->move + 1;
	SoundQueue::get()->push(SoundStorage::get()->get("newMove"));
	if (this->getCurrentPlayer()->getId() == 1) {
		this->view.setCenter(sf::Vector2f(this->windowW / 2, this->windowH / 2));
	}
	else {
		this->view.setCenter(sf::Vector2f(48 * this->mapWidth - this->windowW / 2, 48 * this->mapHeight - this->windowH / 2));
	}
	for (uint32_t i = 0; i < this->gameObjects.size(); i = i + 1) {
		GameObjectResponse response = this->gameObjects[i]->newMove(*this->getCurrentPlayer(), this->windowW, this->windowH);
		if (response.gameEvent.has_value()) {
			this->handleGameEvent(response.gameEvent.value());
		}
		std::queue<PopUpWindow*> popUpWindows2 = response.popUpWindows;
		if (!popUpWindows2.empty()) {
			while (!popUpWindows2.empty()) {
				this->popUpWindows.push(popUpWindows2.front());
				popUpWindows2.pop();
			}
			this->popUpWindows.front()->run(this->windowW, this->windowH);
		}
	}
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
void BattleScreen::viewToNorth() {
	float delta = std::min(10.f, view.getCenter().y - this->windowH / 2);
	view.setCenter(view.getCenter() - sf::Vector2f(0, delta));
}
void BattleScreen::viewToSouth() {
	float delta = std::min(10.f, 48 * this->mapHeight - this->windowH / 2 - view.getCenter().y);
	view.setCenter(view.getCenter() + sf::Vector2f(0, delta));
}
void BattleScreen::viewToWest() {
	float delta = std::min(10.f, view.getCenter().x - this->windowW / 2);
	view.setCenter(view.getCenter() - sf::Vector2f(delta, 0));
}
void BattleScreen::viewToEast() {
	float delta = std::min(10.f, 48 * this->mapWidth - this->windowW / 2 - view.getCenter().x);
	view.setCenter(view.getCenter() + sf::Vector2f(delta, 0));
}