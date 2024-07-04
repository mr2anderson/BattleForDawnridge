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


#include "MainScreen.hpp"


MainScreen* MainScreen::singletone = nullptr;


int32_t MainScreen::run(sf::RenderWindow& window) {
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
					this->prepareToReturnToMenu(window);
					return 0;
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				if (this->elements.empty()) {
					if (endMoveButton.click(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y)) {
						this->changeMove();
					}
					else {
						this->handleGameObjectClick();
					}
				}
				else {
					if (event.type == sf::Event::MouseButtonPressed) {
						this->handleEvents(this->elements.front()->click(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y));
					}
				}
			}
		}
		this->drawEverything(window);
		Playlist::get()->update();
		this->removeFinishedElements();
		this->moveView();
		if (this->elements.empty()) {
			this->moveView();
		}
		else {
			this->elements.front()->update();
		}
	}
}
void MainScreen::init(sf::RenderWindow& window) {
	this->initLandscape();
	this->removeOldPopUpWindows();
	this->initPlayers();
	this->initMoveCtr();
	this->initHighlightTable();
	this->initGraphics(window);
	this->changeMove();
}
void MainScreen::initLandscape() {
	this->mapW = 60;
	this->mapH = 34;
	for (uint32_t i = 0; i < this->gameObjects.size(); i = i + 1) {
		delete this->gameObjects[i];
	}
	this->gameObjects.clear();
	this->units.clear();
	this->resourcePoints.clear();
	this->addResourcePoint(new Forest(13, 7));
	this->addResourcePoint(new Forest(15, 7));
	this->addResourcePoint(new Stone(17, 7));
	this->addResourcePoint(new Stone(19, 7));
	this->addResourcePoint(new Iron(21, 7));
	this->addResourcePoint(new Iron(23, 7));
}
void MainScreen::removeOldPopUpWindows() {
	while (!this->elements.empty()) {
		PopUpElement* w = this->elements.front();
		delete w;
		this->elements.pop();
	}
}
void MainScreen::initPlayers() {
	this->players[0] = Player(1);
	this->players[1] = Player(2);
	this->addUnit(new Castle(4, 4, &this->players[0]));
	this->addUnit(new Market(7, 4, &this->players[0]));
	this->addUnit(new Farm(10, 4, &this->players[0]));
	this->addUnit(new Sawmill(13, 4, &this->players[0], &this->resourcePoints));
	this->addUnit(new Quarry(16, 4, &this->players[0], &this->resourcePoints));
	this->addUnit(new Mine(19, 4, &this->players[0], &this->resourcePoints));
	this->addUnit(new Castle(this->mapW - 7, this->mapH - 7, &this->players[1]));
}
void MainScreen::initMoveCtr() {
	this->move = 0;
}
void MainScreen::initHighlightTable() {
	this->highlightTable.clear();
}
void MainScreen::initGraphics(sf::RenderWindow &window) {
	this->windowW = window.getSize().x;
	this->windowH = window.getSize().y;
	this->view = window.getDefaultView();
	this->endMoveButton = Button(this->windowW - 20 - 150, this->windowH - 20 - 30, 150, 30, std::nullopt, L"����� ����");
}
void MainScreen::handleEvents(const Events& e) {
	this->handleGameEvent(e.gEvent);
	this->handleUIEvent(e.uiEvent);
}
void MainScreen::handleGameEvent(const GEvent &e) {
	this->handleTryToAttackEvent(e);
	this->handleTryToTradeEvent(e);
	this->handleAddResourceEvent(e);
	this->handleSubResourceEvent(e);
	this->handleAddResourcesEvent(e);
	this->handleSubResourcesEvent(e);
	this->handleChangeHighlightEvent(e);
	this->handleCollectEvent(e);
	this->handleTryToUpgradeEvent(e);
	this->handleAddHpEvent(e);
	this->handleDecreaseUpgradeMovesLeftEvent(e);
	this->handleIncreaseLevelEvent(e);
	this->handleDecreaseCurrentTradeMovesLeft(e);
}
void MainScreen::handleTryToAttackEvent(const GEvent& e) {
	for (const auto& a : e.tryToAttack) {

	}
}
void MainScreen::handleTryToTradeEvent(const GEvent& e) {
	for (const auto& a : e.tryToTrade) {
		Market* m = std::get<Market*>(a);
		Trade t = std::get<Trade>(a);
		if (this->getCurrentPlayer()->getResource(t.sell.type) >= t.sell.n) {
			this->handleGOR(m->doTrade(t));
		}
		else {
			MessageW* w = new MessageW("click", "click", 
				L"������������ ��������\n"
				"�� �� ������ ��������� ��� ������.");
			this->addPopUpWindow(w);
		}
	}
}
void MainScreen::handleAddResourceEvent(const GEvent& e) {
	for (const auto& a : e.addResource) {
		this->getCurrentPlayer()->addResource(a);
	}
}
void MainScreen::handleSubResourceEvent(const GEvent& e) {
	for (const auto& a : e.subResource) {
		this->getCurrentPlayer()->subResource(a);
	}
}
void MainScreen::handleAddResourcesEvent(const GEvent& e) {
	for (const auto& a : e.addResources) {
		this->getCurrentPlayer()->addResources(a);
	}
}
void MainScreen::handleSubResourcesEvent(const GEvent& e) {
	for (const auto& a : e.subResources) {
		this->getCurrentPlayer()->subResources(a);
	}
}
void MainScreen::handleChangeHighlightEvent(const GEvent& e) {
	for (const auto& a : e.changeHighlight) {
		const Unit* u = std::get<0>(a);
		uint32_t x = std::get<1>(a);
		uint32_t y = std::get<2>(a);
		if (x >= this->mapW or y >= this->mapH) {
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
void MainScreen::handleCollectEvent(const GEvent& e) {
	for (const auto& a : e.collect) {
		ResourcePoint* resourcePoint = std::get<ResourcePoint*>(a);
		uint32_t n = std::get<uint32_t>(a);
		this->getCurrentPlayer()->addResource(Resource(resourcePoint->getResourceType(), n));
		resourcePoint->subHp(n);
	}
}
void MainScreen::handleTryToUpgradeEvent(const GEvent& e) {
	for (const auto& a : e.tryToUpgrade) {
		UpgradeableB* b = std::get<UpgradeableB*>(a);
		Resources cost = std::get<Resources>(a);
		if (this->getCurrentPlayer()->getResources() >= cost) {
			this->handleGOR(b->startUpgrade());
		}
		else {
			MessageW* w = new MessageW("click", "click", 
				L"������������ ��������\n"
				"�� �� ������ �������� ��� ������.");
			this->addPopUpWindow(w);
		}
	}
}
void MainScreen::handleAddHpEvent(const GEvent& e) {
	for (const auto& a : e.addHp) {
		GO* go = std::get<GO*>(a);
		uint32_t n = std::get<uint32_t>(a);
		go->addHp(n);
	}
}
void MainScreen::handleDecreaseUpgradeMovesLeftEvent(const GEvent& e) {
	for (const auto& a : e.decreaseUpgradeMovesLeft) {
		a->decreaseUpgradeMovesLeft();
	}
}
void MainScreen::handleIncreaseLevelEvent(const GEvent& e) {
	for (const auto& a : e.increaseLevel) {
		a->increaseLevel();
	}
}
void MainScreen::handleDecreaseCurrentTradeMovesLeft(const GEvent& e) {
	for (const auto& a : e.decreaseCurrentTradeMovesLeft) {
		a->decreaseCurrentTradeMovesLeft();
	}
}
void MainScreen::handleUIEvent(const UIEvent& e) {
	this->handlePlaySoundEvent(e);
}
void MainScreen::handlePlaySoundEvent(const UIEvent& e) {
	for (const auto& a : e.playSound) {
		SoundQueue::get()->push(Sounds::get()->get(a));
	}
}
void MainScreen::handleGOR(const GOR& responce) {
	this->addPopUpWindows(responce.elements);
	for (const auto& gEvent : responce.events) {
		this->handleGameEvent(gEvent);
	}
}
void MainScreen::removeFinishedElements() {
	bool remove = false;
	while (!this->elements.empty()) {
		if (!this->elements.front()->finished()) {
			break;
		}
		delete this->elements.front();
		this->elements.pop();
		remove = true;
	}
	if (remove and !this->elements.empty()) {
		this->handleEvents(this->elements.front()->run(this->windowW, this->windowH));
	}
}
void MainScreen::changeMove() {
	this->move = this->move + 1;
	SoundQueue::get()->push(Sounds::get()->get("click"));
	this->updatePlayerViewPoint();
	this->highlightTable.clear();
	for (uint32_t i = 0; i < this->gameObjects.size(); i = i + 1) {
		this->handleGOR(this->gameObjects[i]->newMove(*this->getCurrentPlayer()));
	}
}
Player* MainScreen::getCurrentPlayer() {
	return &this->players[(move - 1) % 2];
}
void MainScreen::handleGameObjectClick() {
	uint32_t mouseX = sf::Mouse::getPosition().x + this->view.getCenter().x - this->windowW / 2;
	uint32_t mouseY = sf::Mouse::getPosition().y + this->view.getCenter().y - this->windowH / 2;
	for (uint32_t i = 0; i < this->gameObjects.size(); i = i + 1) {
		this->handleGOR(this->gameObjects[i]->click(*this->getCurrentPlayer(), mouseX, mouseY));
	}
}
void MainScreen::addPopUpWindows(std::queue<PopUpElement*> q) {
	while (!q.empty()) {
		this->addPopUpWindow(q.front());
		q.pop();
	}
}
void MainScreen::addPopUpWindow(PopUpElement* w) {
	this->elements.push(w);
	if (this->elements.size() == 1) {
		this->handleEvents(w->run(this->windowW, this->windowH));
	}
}
void MainScreen::prepareToReturnToMenu(sf::RenderWindow &window) {
	window.setView(window.getDefaultView());
	Playlist::get()->restartMusic();
}
void MainScreen::drawEverything(sf::RenderWindow& window) {
	window.clear();
	window.setView(this->view);
	this->drawCells(window);
	for (uint32_t i = 0; i < this->gameObjects.size(); i = i + 1) {
		window.draw(*this->gameObjects[i]);
	}
	if (!this->elements.empty()) {
		window.draw(*this->elements.front());
	}
	window.draw(*this->getCurrentPlayer()->getConstResourceBarPtr());
	window.draw(endMoveButton);
	window.display();
}
void MainScreen::drawCells(sf::RenderWindow &window) {
	for (uint32_t i = 0; i < this->mapW; i = i + 1) {
		for (uint32_t j = 0; j < this->mapH; j = j + 1) {
			sf::Sprite s;
			s.setTexture(*Textures::get()->get(std::to_string((i + j) % TOTAL_PLAINS + 1)));
			s.setPosition(32 * i, 32 * j);
			window.draw(s);
			if (!this->highlightTable[std::make_tuple(i, j)].empty()) {
				sf::RectangleShape r;
				r.setPosition(s.getPosition());
				r.setSize(sf::Vector2f(32, 32));
				r.setFillColor(CELL_COLOR_HIGHLIGHTED);
				window.draw(r);
			}
		}
	}
}
void MainScreen::moveView() {
	auto p = sf::Mouse::getPosition();
	if (p.x < 10 or sf::Keyboard::isKeyPressed(sf::Keyboard::A) or sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		this->moveViewToWest();
	}
	else if (p.x > this->windowW - 10 or sf::Keyboard::isKeyPressed(sf::Keyboard::D) or sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		this->moveViewToEast();
	}
	if (p.y < 10 or sf::Keyboard::isKeyPressed(sf::Keyboard::W) or sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		this->moveViewToNorth();
	}
	else if (p.y > this->windowH - 10 or sf::Keyboard::isKeyPressed(sf::Keyboard::S) or sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		this->moveViewToSouth();
	}
}
void MainScreen::updatePlayerViewPoint() {
	if (this->getCurrentPlayer()->getId() == 1) {
		this->view.setCenter(sf::Vector2f(this->windowW / 2, this->windowH / 2));
	}
	else {
		this->view.setCenter(sf::Vector2f(32 * this->mapW - this->windowW / 2, 32 * this->mapH - this->windowH / 2));
	}
}
void MainScreen::moveViewToNorth() {
	float d = std::min(10.f, view.getCenter().y - this->windowH / 2);
	view.setCenter(view.getCenter() - sf::Vector2f(0, d));
}
void MainScreen::moveViewToSouth() {
	float d = std::min(10.f, 32 * this->mapH - this->windowH / 2 - view.getCenter().y);
	view.setCenter(view.getCenter() + sf::Vector2f(0, d));
}
void MainScreen::moveViewToWest() {
	float d = std::min(10.f, view.getCenter().x - this->windowW / 2);
	view.setCenter(view.getCenter() - sf::Vector2f(d, 0));
}
void MainScreen::moveViewToEast() {
	float d = std::min(10.f, 32 * this->mapW - this->windowW / 2 - view.getCenter().x);
	view.setCenter(view.getCenter() + sf::Vector2f(d, 0));
}
void MainScreen::addUnit(Unit* u) {
	this->gameObjects.push_back(u);
	this->units.push_back(u);
}
void MainScreen::addResourcePoint(ResourcePoint* rP) {
	this->gameObjects.push_back(rP);
	this->resourcePoints.push_back(rP);
}