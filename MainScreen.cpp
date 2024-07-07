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
					if (!this->handleButtonsClick()) {
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
        if (this->exit) {
            return false;
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
    Event endMoveEvent;
    endMoveEvent.addChangeMoveEvent();

    std::vector<GameActionWindowComponent> components;
    components.emplace_back("hammer_icon", *Texts::get()->get("leave"), true, true, Event());
    Event event;
    event.addTryToBuildEvent(new Road(0, 0, this->getCurrentPlayer(), this->map->getTobs(), this->map->getTcbs()));
    components.emplace_back(Road().getTextureName(), GET_BUILD_DESCRIPTION(new Road()), true, true, event);
    event = Event();
    event.addTryToBuildEvent(new Farm(0, 0, this->getCurrentPlayer()));
    components.emplace_back(Farm().getTextureName(), GET_BUILD_DESCRIPTION(new Farm()), true, true, event);
    event = Event();
    event.addTryToBuildEvent(new Sawmill(0, 0, this->getCurrentPlayer(), this->map->getResourcePoints()));
    components.emplace_back(Sawmill().getTextureName(), GET_BUILD_DESCRIPTION(new Sawmill()), true, true, event);
    event = Event();
    event.addTryToBuildEvent(new Quarry(0, 0, this->getCurrentPlayer(), this->map->getResourcePoints()));
    components.emplace_back(Quarry().getTextureName(), GET_BUILD_DESCRIPTION(new Quarry()), true, true, event);
    event = Event();
    event.addTryToBuildEvent(new Mine(0, 0, this->getCurrentPlayer(), this->map->getResourcePoints()));
    components.emplace_back(Mine().getTextureName(), GET_BUILD_DESCRIPTION(new Mine()), true, true, event);
    event = Event();
    event.addTryToBuildEvent(new Market(0, 0, this->getCurrentPlayer()));
    components.emplace_back(Market().getTextureName(), GET_BUILD_DESCRIPTION(new Market()), true, true, event);
    event = Event();
    event.addTryToBuildEvent(new Wall(0, 0, this->getCurrentPlayer()));
    components.emplace_back(Wall().getTextureName(), GET_BUILD_DESCRIPTION(new Wall()), true, true, event);
    event = Event();
    event.addTryToBuildEvent(new Castle(0, 0, this->getCurrentPlayer()));
    components.emplace_back(Castle().getTextureName(), GET_BUILD_DESCRIPTION(new Castle()), true, true, event);
    std::shared_ptr<GameActionWindow> w = std::make_shared<GameActionWindow>("click", "click", components);
    Event buildEvent;
    buildEvent.addCreateEEvent(w);

	this->windowW = window.getSize().x;
	this->windowH = window.getSize().y;
    this->exit = false;
	this->view = new sf::View(window.getDefaultView());

	this->buttons.emplace_back(std::make_shared<Label>(this->windowW - 20 - 150, this->windowH - 20 - 30, 150, 30, *Texts::get()->get("new_move")), endMoveEvent);
	this->buttons.emplace_back(std::make_shared<Image>(this->windowW - 20 - 150 - 20 - 64, this->windowH - 20 - 64, "hammer_icon"), buildEvent);
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
    this->handleChangeMoveEvent(e);
    this->handleExitEvent(e);
}
void MainScreen::handleTryToAttackEvent(const Event& e) {
    const std::vector<Unit*>* tryToAttack = e.getTryToAttackEvent();
	for (uint32_t i = 0; i < tryToAttack->size(); i = i + 1) {

    }
}
void MainScreen::handleTryToTradeEvent(const Event& e) {
    const std::vector<std::tuple<Market*, Trade>>* tryToTrade = e.getTryToTradeEvent();
	for (uint32_t i = 0; i < tryToTrade->size(); i = i + 1) {
		Market* m = std::get<Market*>(tryToTrade->at(i));
		Trade t = std::get<Trade>(tryToTrade->at(i));
		if (this->getCurrentPlayer()->getResource(t.sell.type) >= t.sell.n) {
			this->handleEvent(m->doTrade(t));
		}
		else {
			std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>("click", "click", *Texts::get()->get("no_resources_for_trade"), *Texts::get()->get("OK"));
			this->addPopUpWindow(w);
		}
	}
}
void MainScreen::handleAddResourceEvent(const Event& e) {
    const std::vector<Resource>* addResource = e.getAddResourceEvent();
    for (uint32_t i = 0; i < addResource->size(); i = i + 1) {
        this->getCurrentPlayer()->addResource(addResource->at(i));
    }
}
void MainScreen::handleSubResourceEvent(const Event& e) {
    const std::vector<Resource>* subResource = e.getSubResourceEvent();
    for (uint32_t i = 0; i < subResource->size(); i = i + 1) {
        this->getCurrentPlayer()->addResource(subResource->at(i));
    }
}
void MainScreen::handleAddResourcesEvent(const Event& e) {
    const std::vector<Resources>* addResources = e.getAddResourcesEvent();
    for (uint32_t i = 0; i < addResources->size(); i = i + 1) {
        this->getCurrentPlayer()->addResources(addResources->at(i));
    }
}
void MainScreen::handleSubResourcesEvent(const Event& e) {
    const std::vector<Resources>* subResources = e.getSubResourcesEvent();
    for (uint32_t i = 0; i < subResources->size(); i = i + 1) {
        this->getCurrentPlayer()->subResources(subResources->at(i));
    }
}
void MainScreen::handleChangeHighlightEvent(const Event& e) {
    const std::vector<std::tuple<const Unit*, uint32_t, uint32_t>>* changeHighlight = e.getChangeHighlightEvent();
	for (uint32_t i = 0; i < changeHighlight->size(); i = i + 1) {
		const Unit* u = std::get<0>(changeHighlight->at(i));
		uint32_t x = std::get<1>(changeHighlight->at(i));
		uint32_t y = std::get<2>(changeHighlight->at(i));
		if (x >= this->plains.getW() or y >= this->plains.getH()) {
			continue;
		}
		this->highlightTable.mark(x, y, u);
	}
}
void MainScreen::handleCollectEvent(const Event& e) {
    const std::vector<std::tuple<ResourcePoint*, uint32_t>>* collect = e.getCollectEvent();
	for (uint32_t i = 0; i < collect->size(); i = i + 1) {
		ResourcePoint* resourcePoint = std::get<ResourcePoint*>(collect->at(i));
		uint32_t n = std::get<uint32_t>(collect->at(i));
		this->getCurrentPlayer()->addResource(Resource(resourcePoint->getResourceType(), n));
		resourcePoint->subHp(n);
	}
}
void MainScreen::handleTryToUpgradeEvent(const Event& e) {
    const std::vector<std::tuple<UpgradeableB*, Resources>>* tryToUpgrade = e.getTryToUpgradeEvent();
	for (uint32_t i = 0; i < tryToUpgrade->size(); i = i + 1) {
		UpgradeableB* b = std::get<UpgradeableB*>(tryToUpgrade->at(i));
		Resources cost = std::get<Resources>(tryToUpgrade->at(i));
		if (this->getCurrentPlayer()->getResources() >= cost) {
			this->handleEvent(b->startUpgrade());
		}
		else {
			std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>("click", "click", *Texts::get()->get("no_resources_for_upgrade"), *Texts::get()->get("OK"));
			this->addPopUpWindow(w);
		}
	}
}
void MainScreen::handleAddHpEvent(const Event& e) {
    const std::vector<std::tuple<HPGO*, uint32_t>>* addHp = e.getAddHpEvent();
	for (uint32_t i = 0; i < addHp->size(); i = i + 1) {
		HPGO* go = std::get<HPGO*>(addHp->at(i));
		uint32_t n = std::get<uint32_t>(addHp->at(i));
		go->addHp(n);
	}
}
void MainScreen::handleDecreaseUpgradeMovesLeftEvent(const Event& e) {
    const std::vector<UpgradeableB*>* decreaseUpgradeMovesLeft = e.getDecreaseUpgradeMovesLeftEvent();
	for (uint32_t i = 0; i < decreaseUpgradeMovesLeft->size(); i = i + 1) {
		decreaseUpgradeMovesLeft->at(i)->decreaseUpgradeMovesLeft();
	}
}
void MainScreen::handleIncreaseLevelEvent(const Event& e) {
    const std::vector<UpgradeableB*>* increaseLevelEvent = e.getIncreaseLevelEvent();
	for (uint32_t i = 0; i < increaseLevelEvent->size(); i = i + 1) {
		increaseLevelEvent->at(i)->increaseLevel();
	}
}
void MainScreen::handleDecreaseCurrentTradeMovesLeft(const Event& e) {
    const std::vector<Market*>* decreaseCurrentTradeMovesLeft = e.getDecreaseCurrentTradeMovesLeftEvent();
	for (uint32_t i = 0; i < decreaseCurrentTradeMovesLeft->size(); i = i + 1) {
		decreaseCurrentTradeMovesLeft->at(i)->decreaseUpgradeMovesLeft();
	}
}
void MainScreen::handleTryToBuild(const Event& e) {
    const std::vector<Building*>* tryToBuild = e.getTryToBuildEvent();
	for (uint32_t i = 0; i < tryToBuild->size(); i = i + 1) {
		if (this->getCurrentPlayer()->getResources() >= tryToBuild->at(i)->getCost()) {
			this->addPopUpWindow(std::make_shared<BuildingMode>(tryToBuild->at(i), this->view, this->map->getGO(), this->map->getTbs(), this->getCurrentPlayer()));
		}
		else {
			std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>("", "click", *Texts::get()->get("no_resources_for_building"), *Texts::get()->get("OK"));
			this->addPopUpWindow(w);
			delete tryToBuild->at(i);
		}
	}
}
void MainScreen::handleBuild(const Event& e) {
    const std::vector<Building*>* build = e.getBuildEvent();
	for (uint32_t i = 0; i < build->size(); i = i + 1) {
		this->map->add(build->at(i));
	}
}
void MainScreen::handlePlaySoundEvent(const Event& e) {
    const std::vector<std::string>* playSound = e.getPlaySoundEvent();
	for (uint32_t i = 0; i < playSound->size(); i = i + 1) {
		SoundQueue::get()->push(Sounds::get()->get(playSound->at(i)));
	}
}
void MainScreen::handleCreatePopUpElementEvent(const Event& e) {
    const std::vector<std::shared_ptr<PopUpElement>>* createE = e.getCreateEEvent();
	for (uint32_t i = 0; i < createE->size(); i = i + 1) {
		this->addPopUpWindow(createE->at(i));
	}
}
void MainScreen::handleChangeMoveEvent(const Event &e) {
    uint32_t changeMove = e.getChangeMoveEvent();
    for (uint32_t i = 0; i < changeMove; i = i + 1) {
        this->changeMove();
    }
}
void MainScreen::handleExitEvent(const Event &e) {
    if (e.getExitEvent()) {
        this->exit = true;
    }
}
void MainScreen::removeFinishedElements() {
	bool remove = false;
	while (!this->elements.empty()) {
		if (!this->elements.front()->finished()) {
			break;
		}
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
std::wstring MainScreen::GET_BUILD_DESCRIPTION(Building* b) {
	std::wstring description = b->getDescription() + L'\n' +
		*Texts::get()->get("cost") + b->getCost().getReadableInfo();
	delete b;
	return description;
		
}
Player* MainScreen::getCurrentPlayer() {
	return this->map->getPlayer((this->move - 1) % this->map->getPlayersNumber());
}
bool MainScreen::handleButtonsClick() {
    for (uint32_t i = 0; i < this->buttons.size(); i = i + 1) {
        Event event = this->buttons[i].click();
        if (!event.empty()) {
            this->handleEvent(event);
            return true;
        }
    }
    return false;
}
void MainScreen::handleGameObjectClick() {
	uint32_t mouseX = sf::Mouse::getPosition().x + this->view->getCenter().x - this->windowW / 2;
	uint32_t mouseY = sf::Mouse::getPosition().y + this->view->getCenter().y - this->windowH / 2;
	for (uint32_t i = 0; i < this->map->getGO()->size(); i = i + 1) {
		this->handleEvent(this->map->getGO()->at(i)->click(this->getCurrentPlayer(), mouseX, mouseY));
	}
}
void MainScreen::addPopUpWindow(std::shared_ptr<PopUpElement> w) {
	this->elements.push(w);
	if (this->elements.size() == 1) {
		this->handleEvent(w->run(this->windowW, this->windowH));
	}
}
void MainScreen::prepareToReturnToMenu(sf::RenderWindow &window) {
	window.setView(window.getDefaultView());
    this->removeFinishedElements();
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
	for (const auto &b : this->buttons) {
        window.draw(b);
    }
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