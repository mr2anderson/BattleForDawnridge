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
	sf::Event event{};
	for (; ;) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				return -1;
			}
			if (event.type == sf::Event::KeyPressed) {
				auto code = event.key.code;
				if (code == sf::Keyboard::Escape) {
					this->prepareReturnToMenu(window);
					return 0;
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				if (this->popUpWindows.empty()) {
					if (endMove.click(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y)) {
						this->newMove();
					}
					else {
						this->handleGameObjectClick();
					}
				}
				else {
					if (event.type == sf::Event::MouseButtonPressed) {
						this->handlePopUpWindowEvent(this->popUpWindows.front()->click(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y));
					}
				}
			}
		}
		this->drawEverything(window);
		Playlist::get()->update();
		if (this->popUpWindows.empty()) {
			this->handleViewMovement();
		}
	}
}
void BattleScreen::init(sf::RenderWindow& window) {
	this->initLandscape();
	this->removeOldPopUpWindows();
	this->initPlayers();
	this->initMoveCtr();
	this->initHighlightTable();
	this->initGraphics(window);
}
void BattleScreen::initLandscape() {
	this->mapWidth = 30;
	this->mapHeight = 20;
	for (uint32_t i = 0; i < this->gameObjects.size(); i = i + 1) {
		delete this->gameObjects[i];
	}
	this->gameObjects.clear();
	this->units.clear();
	this->resourcePoints.clear();

	this->pushResourcePoint(new Plant(4, 4));
	this->pushResourcePoint(new Plant(4, 5));
	this->pushResourcePoint(new Tree(1, 3));
	this->pushResourcePoint(new Tree(1, 4));
	this->pushResourcePoint(new Mountain(2, 3));
	this->pushResourcePoint(new Mountain(2, 4));
	this->pushResourcePoint(new RedMountain(3, 3));
	this->pushResourcePoint(new RedMountain(3, 4));
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
	this->pushUnit(new Fort(1, 1, &this->players[0]));
	this->pushUnit(new Caravan(1, 2, &this->players[0]));
	this->pushUnit(new Caravan(2, 1, &this->players[0]));
	this->pushUnit(new Windmill(2, 2, &this->players[0], &this->resourcePoints));
	this->pushUnit(new Sawmill(3, 2, &this->players[0], &this->resourcePoints));
	this->pushUnit(new Quarry(4, 2, &this->players[0], &this->resourcePoints));
	this->pushUnit(new Mine(5, 2, &this->players[0], &this->resourcePoints));
	this->pushUnit(new Fort(this->mapWidth - 2, this->mapHeight - 2, &this->players[1]));
}
void BattleScreen::initMoveCtr() {
	this->move = 1;
}
void BattleScreen::initHighlightTable() {
	this->highlightTable.clear();
}
void BattleScreen::initGraphics(sf::RenderWindow &window) {
	this->windowW = window.getSize().x;
	this->windowH = window.getSize().y;
	this->view = window.getDefaultView();
	this->endMove = Button(this->windowW - 20 - 150, this->windowH - 20 - 30, 150, 30, L"Конец хода", 18);
}
void BattleScreen::handleGameEvent(const GameEvent &event) {
	this->handleTryToAttackEvent(event);
	this->handleTryToTradeEvent(event);
	this->handleAddResourceEvent(event);
	this->handleSubResourceEvent(event);
	this->handleAddResourcesEvent(event);
	this->handleSubResourcesEvent(event);
	this->handleChangeHighlightEvent(event);
	this->handleCollectEvent(event);
	this->handleTryToUpgradeEvent(event);
}
void BattleScreen::handleTryToAttackEvent(const GameEvent& event) {
	for (const auto& a : event.tryToAttack) {

	}
}
void BattleScreen::handleTryToTradeEvent(const GameEvent& event) {
	for (const auto& a : event.tryToTrade) {
		Caravan* caravan = std::get<Caravan*>(a);
		Trade trade = std::get<Trade>(a);
		if (this->getCurrentPlayer()->getResource(trade.sell.type) >= trade.sell.n) {
			GameObjectResponse response = caravan->doTrade(trade);
			if (response.gameEvent.has_value()) {
				this->handleGameEvent(response.gameEvent.value());
			}
			this->addPopUpWindows(response.popUpWindows);
		}
		else {
			MessageWindow* window = new MessageWindow("click", "click", L"Недостаточно ресурсов\nВы не можете совершить эту сделку.");
			this->popUpWindows.push(window);
			if (this->popUpWindows.size() == 1) {
				this->popUpWindows.front()->run(this->windowW, this->windowH);
			}
		}
	}
}
void BattleScreen::handleAddResourceEvent(const GameEvent& event) {
	for (const auto& a : event.addResource) {
		this->getCurrentPlayer()->addResource(a);
	}
}
void BattleScreen::handleSubResourceEvent(const GameEvent& event) {
	for (const auto& a : event.subResource) {
		this->getCurrentPlayer()->subResource(a);
	}
}
void BattleScreen::handleAddResourcesEvent(const GameEvent& event) {
	for (const auto& a : event.addResources) {
		this->getCurrentPlayer()->addResources(a);
	}
}
void BattleScreen::handleSubResourcesEvent(const GameEvent& event) {
	for (const auto& a : event.subResources) {
		this->getCurrentPlayer()->subResources(a);
	}
}
void BattleScreen::handleChangeHighlightEvent(const GameEvent& event) {
	for (const auto& a : event.changeHighlight) {
		const Unit* u = std::get<0>(a);
		uint32_t x = std::get<1>(a);
		uint32_t y = std::get<2>(a);
		if (x >= this->mapWidth or y >= this->mapHeight) {
			continue;
		}
		std::tuple<uint32_t, uint32_t> p = std::make_tuple(x, y);
		std::vector<const Unit*> v = this->highlightTable[p];
		bool found = false;
		for (uint32_t i = 0; i < v.size(); i = i + 1) {
			if (v[i] == u) {
				v.erase(v.begin() + i);
				found = true;
				break;
			}
		}
		if (!found) {
			v.push_back(u);
		}
		this->highlightTable[p] = v;
	}
}
void BattleScreen::handleCollectEvent(const GameEvent& event) {
	for (const auto& a : event.collect) {
		ResourcePoint* resourcePoint = std::get<ResourcePoint*>(a);
		uint32_t n = std::get<uint32_t>(a);
		this->getCurrentPlayer()->addResource(Resource(resourcePoint->getResourceType(), n));
		resourcePoint->subHp(n);
	}
}
void BattleScreen::handleTryToUpgradeEvent(const GameEvent& event) {
	for (const auto& a : event.tryToUpgrade) {
		Building* b = std::get<Building*>(a);
		Resources cost = std::get<Resources>(a);
		if (this->getCurrentPlayer()->getResources() >= cost) {
			GameObjectResponse response = b->upgrade();
			this->handleGameEvent(response.gameEvent.value());
			this->addPopUpWindows(response.popUpWindows);
		}
		else {
			MessageWindow* window = new MessageWindow("click", "click", L"Недостаточно ресурсов\nВы не можете улучшить это здание.");
			this->popUpWindows.push(window);
			if (this->popUpWindows.size() == 1) {
				this->popUpWindows.front()->run(this->windowW, this->windowH);
			}
		}
	}
}
void BattleScreen::handlePopUpWindowEvent(const PopUpWindowEvent &event) {
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
	this->changePlayerPOV();
	this->highlightTable.clear();
	for (uint32_t i = 0; i < this->gameObjects.size(); i = i + 1) {
		GameObjectResponse response = this->gameObjects[i]->newMove(*this->getCurrentPlayer(), this->windowW, this->windowH);
		if (response.gameEvent.has_value()) {
			this->handleGameEvent(response.gameEvent.value());
		}
		this->addPopUpWindows(response.popUpWindows);
		std::queue<PopUpWindow*> popUpWindows2 = response.popUpWindows;
	}
}
Player* BattleScreen::getCurrentPlayer() {
	return &this->players[(move - 1) % 2];
}
void BattleScreen::handleGameObjectClick() {
	uint32_t mouseX = sf::Mouse::getPosition().x + this->view.getCenter().x - this->windowW / 2;
	uint32_t mouseY = sf::Mouse::getPosition().y + this->view.getCenter().y - this->windowH / 2;
	for (uint32_t i = 0; i < this->gameObjects.size(); i = i + 1) {
		GameObjectResponse response = this->gameObjects[i]->click(*this->getCurrentPlayer(), mouseX, mouseY, this->windowW, this->windowH);
		if (response.gameEvent.has_value()) {
			this->handleGameEvent(response.gameEvent.value());
		}
		this->addPopUpWindows(response.popUpWindows);
	}
}
void BattleScreen::addPopUpWindows(std::queue<PopUpWindow*> windows) {
	bool popUpWindowsExist = !this->popUpWindows.empty();
	if (!windows.empty()) {
		while (!windows.empty()) {
			this->popUpWindows.push(windows.front());
			windows.pop();
		}
		if (!popUpWindowsExist) {
			this->popUpWindows.front()->run(this->windowW, this->windowH);
		}
	}
}
void BattleScreen::prepareReturnToMenu(sf::RenderWindow &window) {
	window.setView(window.getDefaultView());
	Playlist::get()->restartMusic();
}
void BattleScreen::drawEverything(sf::RenderWindow& window) {
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
}
void BattleScreen::drawCells(sf::RenderWindow &window) {
	for (uint32_t i = 0; i < this->mapWidth; i = i + 1) {
		for (uint32_t j = 0; j < this->mapHeight; j = j + 1) {
			sf::RectangleShape rect;
			rect.setPosition(sf::Vector2f(64 * i, 64 * j));
			rect.setSize(sf::Vector2f(64, 64));
			if ((i + j) % 2 == 0) {
				if (!this->highlightTable[std::make_tuple(i, j)].empty()) {
					rect.setFillColor(CELL_COLOR_HIGHLIGHTED_DARK);
				}
				else {
					rect.setFillColor(CELL_COLOR);
				}
				window.draw(rect);
			}
			else {
				if (!this->highlightTable[std::make_tuple(i, j)].empty()) {
					rect.setFillColor(CELL_COLOR_HIGHLIGHTED_LIGHT);
					window.draw(rect);
				}
			}
		}
	}
}
void BattleScreen::handleViewMovement() {
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
void BattleScreen::changePlayerPOV() {
	if (this->getCurrentPlayer()->getId() == 1) {
		this->view.setCenter(sf::Vector2f(this->windowW / 2, this->windowH / 2));
	}
	else {
		this->view.setCenter(sf::Vector2f(64 * this->mapWidth - this->windowW / 2, 64 * this->mapHeight - this->windowH / 2));
	}
}
void BattleScreen::viewToNorth() {
	float delta = std::min(10.f, view.getCenter().y - this->windowH / 2);
	view.setCenter(view.getCenter() - sf::Vector2f(0, delta));
}
void BattleScreen::viewToSouth() {
	float delta = std::min(10.f, 64 * this->mapHeight - this->windowH / 2 - view.getCenter().y);
	view.setCenter(view.getCenter() + sf::Vector2f(0, delta));
}
void BattleScreen::viewToWest() {
	float delta = std::min(10.f, view.getCenter().x - this->windowW / 2);
	view.setCenter(view.getCenter() - sf::Vector2f(delta, 0));
}
void BattleScreen::viewToEast() {
	float delta = std::min(10.f, 64 * this->mapWidth - this->windowW / 2 - view.getCenter().x);
	view.setCenter(view.getCenter() + sf::Vector2f(delta, 0));
}
void BattleScreen::pushUnit(Unit* unit) {
	this->gameObjects.push_back(unit);
	this->units.push_back(unit);
}
void BattleScreen::pushResourcePoint(ResourcePoint* resourcePoint) {
	this->gameObjects.push_back(resourcePoint);
	this->resourcePoints.push_back(resourcePoint);
}