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


#include "MainScreen.hpp"



MainScreen* MainScreen::singletone = nullptr;


bool MainScreen::run(Map *mapPtr, sf::RenderWindow& window) {
	this->reset(mapPtr, window);
	sf::Event event{};
	for (; ;) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				return false;
			}
			if (event.type == sf::Event::KeyPressed) {
				auto code = event.key.code;
				if (code == sf::Keyboard::Escape) {
					this->prepareToReturnToMenu(window);
					return true;
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				if (this->elements.empty()) {
					if (endMoveButton.click()) {
						this->changeMove();
					}
					else if (buildButton.click()) {
						this->createBuildMenu();
					}
					else {
						this->handleGameObjectClick();
					}
				}
				else {
					if (event.type == sf::Event::MouseButtonPressed) {
						this->handleEvent(this->elements.front()->click());
					}
				}
			}
		}
		this->drawEverything(window);
		Playlist::get()->update();
		this->removeFinishedElements();
		this->moveView();
		if (!this->elements.empty()) {
			this->elements.front()->update();
		}
	}
}
void MainScreen::reset(Map *mapPtr, sf::RenderWindow& window) {
	this->resetMap(mapPtr);
	this->resetMoveCtr();
	this->resetHighlightTable();
    this->resetPlains();
	this->resetGraphics(window);
	this->changeMove();
}
void MainScreen::resetMap(Map *mapPtr) {
    this->map = mapPtr;
}
void MainScreen::resetMoveCtr() {
	this->move = 0;
}
void MainScreen::resetHighlightTable() {
	this->highlightTable.clear();
}
void MainScreen::resetPlains() {
    this->plains = PlainsGeneration(this->map->getW(), this->map->getH());
}
void MainScreen::resetGraphics(sf::RenderWindow &window) {
	this->windowW = window.getSize().x;
	this->windowH = window.getSize().y;
	this->view = new sf::View(window.getDefaultView());
	this->endMoveButton = Button(std::make_shared<Label>(this->windowW - 20 - 150, this->windowH - 20 - 30, 150, 30, *Texts::get()->get("new_move")));
	this->buildButton = Button(std::make_shared<Image>(this->windowW - 20 - 150 - 20 - 64, this->windowH - 20 - 64, "hammer_icon"));
	while (!this->elements.empty()) {
		PopUpElement* w = this->elements.front();
		delete w;
		this->elements.pop();
	}
}
void MainScreen::handleEvent(const Event &e) {
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
	this->handleTryToBuild(e);
	this->handleBuild(e);
	this->handlePlaySoundEvent(e);
	this->handleCreatePopUpElementEvent(e);
}
void MainScreen::handleTryToAttackEvent(const Event& e) {
	for (const auto& a : e.tryToAttack) {

	}
}
void MainScreen::handleTryToTradeEvent(const Event& e) {
	for (const auto& a : e.tryToTrade) {
		Market* m = std::get<Market*>(a);
		Trade t = std::get<Trade>(a);
		if (this->getCurrentPlayer()->getResource(t.sell.type) >= t.sell.n) {
			this->handleEvent(m->doTrade(t));
		}
		else {
			WindowButton* w = new WindowButtonSounds("click", "click", *Texts::get()->get("no_resources_for_trade"), *Texts::get()->get("OK"));
			this->addPopUpWindow(w);
		}
	}
}
void MainScreen::handleAddResourceEvent(const Event& e) {
	for (const auto& a : e.addResource) {
		this->getCurrentPlayer()->addResource(a);
	}
}
void MainScreen::handleSubResourceEvent(const Event& e) {
	for (const auto& a : e.subResource) {
		this->getCurrentPlayer()->subResource(a);
	}
}
void MainScreen::handleAddResourcesEvent(const Event& e) {
	for (const auto& a : e.addResources) {
		this->getCurrentPlayer()->addResources(a);
	}
}
void MainScreen::handleSubResourcesEvent(const Event& e) {
	for (const auto& a : e.subResources) {
		this->getCurrentPlayer()->subResources(a);
	}
}
void MainScreen::handleChangeHighlightEvent(const Event& e) {
	for (const auto& a : e.changeHighlight) {
		const Unit* u = std::get<0>(a);
		uint32_t x = std::get<1>(a);
		uint32_t y = std::get<2>(a);
		if (x >= this->plains.getW() or y >= this->plains.getH()) {
			continue;
		}
		this->highlightTable.mark(x, y, u);
	}
}
void MainScreen::handleCollectEvent(const Event& e) {
	for (const auto& a : e.collect) {
		ResourcePoint* resourcePoint = std::get<ResourcePoint*>(a);
		uint32_t n = std::get<uint32_t>(a);
		this->getCurrentPlayer()->addResource(Resource(resourcePoint->getResourceType(), n));
		resourcePoint->subHp(n);
	}
}
void MainScreen::handleTryToUpgradeEvent(const Event& e) {
	for (const auto& a : e.tryToUpgrade) {
		UpgradeableB* b = std::get<UpgradeableB*>(a);
		Resources cost = std::get<Resources>(a);
		if (this->getCurrentPlayer()->getResources() >= cost) {
			this->handleEvent(b->startUpgrade());
		}
		else {
			WindowButton* w = new WindowButtonSounds("click", "click", *Texts::get()->get("no_resources_for_upgrade"), *Texts::get()->get("OK"));
			this->addPopUpWindow(w);
		}
	}
}
void MainScreen::handleAddHpEvent(const Event& e) {
	for (const auto& a : e.addHp) {
		HPGO* go = std::get<HPGO*>(a);
		uint32_t n = std::get<uint32_t>(a);
		go->addHp(n);
	}
}
void MainScreen::handleDecreaseUpgradeMovesLeftEvent(const Event& e) {
	for (const auto& a : e.decreaseUpgradeMovesLeft) {
		a->decreaseUpgradeMovesLeft();
	}
}
void MainScreen::handleIncreaseLevelEvent(const Event& e) {
	for (const auto& a : e.increaseLevel) {
		a->increaseLevel();
	}
}
void MainScreen::handleDecreaseCurrentTradeMovesLeft(const Event& e) {
	for (const auto& a : e.decreaseCurrentTradeMovesLeft) {
		a->decreaseCurrentTradeMovesLeft();
	}
}
void MainScreen::handleTryToBuild(const Event& e) {
	for (const auto& a : e.tryToBuild) {
		if (this->getCurrentPlayer()->getResources() >= a->getCost()) {
			this->addPopUpWindow(new BuildingMode(a, this->view, this->map->getGO(), this->map->getTbs(), this->getCurrentPlayer()));
		}
		else {
			WindowButton* w = new WindowButtonEndingSound("click", *Texts::get()->get("no_resources_for_building"), *Texts::get()->get("OK"));
			this->addPopUpWindow(w);
			delete a;
		}
	}
}
void MainScreen::handleBuild(const Event& e) {
	for (const auto& a : e.build) {
		this->map->add(a);
	}
}
void MainScreen::handlePlaySoundEvent(const Event& e) {
	for (const auto& a : e.playSound) {
		SoundQueue::get()->push(Sounds::get()->get(a));
	}
}
void MainScreen::handleCreatePopUpElementEvent(const Event& e) {
	for (const auto& a : e.createE) {
		this->addPopUpWindow(a);
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
		this->handleEvent(this->elements.front()->run(this->windowW, this->windowH));
	}
}
void MainScreen::changeMove() {
	this->move = this->move + 1;
	SoundQueue::get()->push(Sounds::get()->get("click"));
	this->updatePlayerViewPoint();
	this->highlightTable.clear();
	for (uint32_t i = 0; i < this->map->getGO()->size(); i = i + 1) {
		this->handleEvent(this->map->getGO()->at(i)->newMove(this->getCurrentPlayer()));
	}
}
void MainScreen::createBuildMenu() {
	std::vector<SelectionWComponent> components;
	components.emplace_back("hammer_icon", *Texts::get()->get("leave"), true, true, Event());

	Event buildEvent;
	buildEvent.tryToBuild.push_back(new Road(0, 0, this->getCurrentPlayer(), this->map->getTobs(), this->map->getTcbs()));
	components.emplace_back(Road().getTextureName(), GET_BUILD_DESCRIPTION(new Road()), true, true, buildEvent);
	
	buildEvent = Event();
	buildEvent.tryToBuild.push_back(new Farm(0, 0, this->getCurrentPlayer()));
	components.emplace_back(Farm().getTextureName(), GET_BUILD_DESCRIPTION(new Farm()), true, true, buildEvent);

	buildEvent = Event();
	buildEvent.tryToBuild.push_back(new Sawmill(0, 0, this->getCurrentPlayer(), this->map->getResourcePoints()));
	components.emplace_back(Sawmill().getTextureName(), GET_BUILD_DESCRIPTION(new Sawmill()), true, true, buildEvent);

	buildEvent = Event();
	buildEvent.tryToBuild.push_back(new Quarry(0, 0, this->getCurrentPlayer(), this->map->getResourcePoints()));
	components.emplace_back(Quarry().getTextureName(), GET_BUILD_DESCRIPTION(new Quarry()), true, true, buildEvent);

	buildEvent = Event();
	buildEvent.tryToBuild.push_back(new Mine(0, 0, this->getCurrentPlayer(), this->map->getResourcePoints()));
	components.emplace_back(Mine().getTextureName(), GET_BUILD_DESCRIPTION(new Mine()), true, true, buildEvent);

	buildEvent = Event();
	buildEvent.tryToBuild.push_back(new Market(0, 0, this->getCurrentPlayer()));
	components.emplace_back(Market().getTextureName(), GET_BUILD_DESCRIPTION(new Market()), true, true, buildEvent);

	buildEvent = Event();
	buildEvent.tryToBuild.push_back(new Wall(0, 0, this->getCurrentPlayer()));
	components.emplace_back(Wall().getTextureName(), GET_BUILD_DESCRIPTION(new Wall()), true, true, buildEvent);

	buildEvent = Event();
	buildEvent.tryToBuild.push_back(new Castle(0, 0, this->getCurrentPlayer()));
	components.emplace_back(Castle().getTextureName(), GET_BUILD_DESCRIPTION(new Castle()), true, true, buildEvent);

	SelectionW* w = new SelectionW("click", "click", components);
	this->addPopUpWindow(w);
}
std::wstring MainScreen::GET_BUILD_DESCRIPTION(Building* b) {
	std::wstring description = b->getDescription() + L'\n' +
		*Texts::get()->get("cost") + b->getCost().getReadableInfo();
	delete b;
	return description;
		
}
Player* MainScreen::getCurrentPlayer() {
	return this->map->getPlayer((this->move - 1) % this->map->getPlayersNumber());
}
void MainScreen::handleGameObjectClick() {
	uint32_t mouseX = sf::Mouse::getPosition().x + this->view->getCenter().x - this->windowW / 2;
	uint32_t mouseY = sf::Mouse::getPosition().y + this->view->getCenter().y - this->windowH / 2;
	for (uint32_t i = 0; i < this->map->getGO()->size(); i = i + 1) {
		this->handleEvent(this->map->getGO()->at(i)->click(this->getCurrentPlayer(), mouseX, mouseY));
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
		this->handleEvent(w->run(this->windowW, this->windowH));
	}
}
void MainScreen::prepareToReturnToMenu(sf::RenderWindow &window) {
	window.setView(window.getDefaultView());
    delete this->view;
    delete this->map;
	Playlist::get()->restartMusic();
}
void MainScreen::drawEverything(sf::RenderWindow& window) {
	window.clear();
	window.setView(*this->view);
	this->drawCells(window);
	window.draw(*this->map);
	if (!this->elements.empty()) {
		window.draw(*this->elements.front());
	}
	window.draw(*this->getCurrentPlayer()->getConstResourceBarPtr());
	window.draw(endMoveButton);
	window.draw(buildButton);
	window.display();
}
void MainScreen::drawCells(sf::RenderWindow &window) {
	for (uint32_t i = 0; i < this->plains.getW(); i = i + 1) {
		for (uint32_t j = 0; j < this->plains.getH(); j = j + 1) {
			sf::Sprite s;
			s.setTexture(*Textures::get()->get(std::to_string(this->plains.getType(i, j) + 1)));
			s.setPosition(32 * i, 32 * j);
			window.draw(s);
			if (this->highlightTable.highlighted(i, j)) {
				sf::RectangleShape r;
				r.setPosition(s.getPosition());
				r.setSize(sf::Vector2f(32, 32));
				r.setFillColor(COLOR_THEME::CELL_COLOR_HIGHLIGHTED);
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
		this->view->setCenter(sf::Vector2f(this->windowW / 2, this->windowH / 2));
	}
	else {
		this->view->setCenter(sf::Vector2f(32 * this->plains.getW() - this->windowW / 2, 32 * this->plains.getH() - this->windowH / 2));
	}
}
void MainScreen::moveViewToNorth() {
	float d = std::min(10.f, this->view->getCenter().y - this->windowH / 2);
	this->view->setCenter(this->view->getCenter() - sf::Vector2f(0, d));
}
void MainScreen::moveViewToSouth() {
	float d = std::min(10.f, 32 * this->plains.getH() - this->windowH / 2 - this->view->getCenter().y);
	this->view->setCenter(this->view->getCenter() + sf::Vector2f(0, d));
}
void MainScreen::moveViewToWest() {
	float d = std::min(10.f, this->view->getCenter().x - this->windowW / 2);
	this->view->setCenter(this->view->getCenter() - sf::Vector2f(d, 0));
}
void MainScreen::moveViewToEast() {
	float d = std::min(10.f, 32 * this->plains.getW() - this->windowW / 2 - this->view->getCenter().x);
	this->view->setCenter(this->view->getCenter() + sf::Vector2f(d, 0));
}