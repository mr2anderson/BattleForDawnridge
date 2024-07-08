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


bool MainScreen::run(std::shared_ptr<Map> mapPtr, sf::RenderWindow& window) {
    this->init(mapPtr, window);
	sf::Event event{};
	for (; ;) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				return false;
			}
			else if (event.type == sf::Event::MouseButtonPressed and event.mouseButton.button == sf::Mouse::Left) {
				if (this->elements.empty()) {
					if (!this->handleButtonsClick()) {
                        this->handleGameObjectClick();
                    }
				}
				else if (!this->elements.empty()) {
					Events events = this->elements.front()->click();
					this->handleEvent(events);
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
        if (this->returnToMenu) {
			this->prepareToReturnToMenu(window);
            return true;
        }
	}
}
void MainScreen::init(std::shared_ptr<Map> mapPtr, sf::RenderWindow& window) {
    this->initMap(mapPtr);
    this->initMoveCtr();
    this->initPlains();
    this->initGraphics(window);
	this->changeMove();
}
void MainScreen::initMap(std::shared_ptr<Map> mapPtr) {
    this->map = mapPtr;
}
void MainScreen::initMoveCtr() {
	this->move = 0;
}
void MainScreen::initPlains() {
    this->plains = PlainsGeneration(this->map->getW(), this->map->getH());
}
void MainScreen::initGraphics(sf::RenderWindow &window) {
    Events endMoveEvent;
	endMoveEvent.add(std::make_shared<ChangeMoveEvent>());
    endMoveEvent.add(std::make_shared<PlaySoundEvent>("click"));

    Events returnToMenuEvent;
	returnToMenuEvent.add(std::make_shared<ReturnToMenuEvent>());
    returnToMenuEvent.add(std::make_shared<PlaySoundEvent>("click"));

	std::shared_ptr<WindowTwoButtons> confirmReturnToMenuWindow = std::make_shared<WindowTwoButtons>("click", "click", *Texts::get()->get("confirm_return_to_menu"), *Texts::get()->get("yes"), *Texts::get()->get("no"), returnToMenuEvent, Events());
	Events createConfirmReturnToMenuWindowEvent;
	createConfirmReturnToMenuWindowEvent.add(std::make_shared<CreateEEvent>(confirmReturnToMenuWindow));

    std::vector<GameActionWindowComponent> components;
    components.emplace_back("hammer_icon", *Texts::get()->get("leave"), true, true, Events());
    Events event;
    event.add(std::make_shared<TryToBuildEvent>(std::make_shared<Road>(0, 0, nullptr, this->map->getTobs(), this->map->getTcbs())));
    components.emplace_back(Road().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<Road>()), true, true, event);
    event = Events();
    event.add(std::make_shared<TryToBuildEvent>(std::make_shared<Windmill>(0, 0, nullptr, this->map->getArables())));
    components.emplace_back(Windmill().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<Windmill>()), true, true, event);
    event = Events();
    event.add(std::make_shared<TryToBuildEvent>(std::make_shared<Sawmill>(0, 0, nullptr, this->map->getResourcePoints())));
    components.emplace_back(Sawmill().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<Sawmill>()), true, true, event);
    event = Events();
    event.add(std::make_shared<TryToBuildEvent>(std::make_shared<Quarry>(0, 0, nullptr, this->map->getResourcePoints())));
    components.emplace_back(Quarry().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<Quarry>()), true, true, event);
    event = Events();
    event.add(std::make_shared<TryToBuildEvent>(std::make_shared<Mine>(0, 0, nullptr, this->map->getResourcePoints())));
    components.emplace_back(Mine().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<Mine>()), true, true, event);
    event = Events();
    event.add((std::make_shared<TryToBuildEvent>(std::make_shared<Market>(0, 0, nullptr))));
    components.emplace_back(Market().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<Market>()), true, true, event);
    event = Events();
    event.add((std::make_shared<TryToBuildEvent>(std::make_shared<Wall>(0, 0, nullptr))));
    components.emplace_back(Wall().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<Wall>()), true, true, event);
    event = Events();
    event.add(std::make_shared<TryToBuildEvent>(std::make_shared<Castle>(0, 0, nullptr)));
    components.emplace_back(Castle().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<Castle>()), true, true, event);
    std::shared_ptr<GameActionWindow> w = std::make_shared<GameActionWindow>("click", "click", components);
    Events buildEvent;
    buildEvent.add(std::make_shared<CreateEEvent>(w));

	this->windowW = window.getSize().x;
	this->windowH = window.getSize().y;
    this->returnToMenu = false;
	this->view = std::make_shared<sf::View>(window.getDefaultView());

	this->buttons.emplace_back(std::make_shared<Label>(this->windowW - 20 - 150, this->windowH - 20 - 30, 150, 30, *Texts::get()->get("new_move")), endMoveEvent);
	this->buttons.emplace_back(std::make_shared<Image>(this->windowW - 20 - 150 - 20 - 64, this->windowH - 20 - 64, "hammer_icon"), buildEvent);
    this->buttons.emplace_back(std::make_shared<Label>(this->windowW - 20 - 150, 30, 150, 30, *Texts::get()->get("to_menu")), createConfirmReturnToMenuWindowEvent);
}
void MainScreen::handleEvent(Events &e) {
	for (uint32_t i = 0; i < e.size(); i = i + 1) {
		if (std::shared_ptr<TryToAttackEvent> tryToAttackEvent = std::dynamic_pointer_cast<TryToAttackEvent>(e.at(i))) {
			this->handleTryToAttackEvent(tryToAttackEvent);
		}
		else if (std::shared_ptr<TryToTradeEvent> tryToTradeEvent = std::dynamic_pointer_cast<TryToTradeEvent>(e.at(i))) {
			this->handleTryToTradeEvent(tryToTradeEvent);
		}
		else if (std::shared_ptr<AddResourceEvent> addResourceEvent = std::dynamic_pointer_cast<AddResourceEvent>(e.at(i))) {
			this->handleAddResourceEvent(addResourceEvent);
		}
		else if (std::shared_ptr<SubResourceEvent> subResourceEvent = std::dynamic_pointer_cast<SubResourceEvent>(e.at(i))) {
			this->handleSubResourceEvent(subResourceEvent);
		}
		else if (std::shared_ptr<AddResourcesEvent> addResourcesEvent = std::dynamic_pointer_cast<AddResourcesEvent>(e.at(i))) {
			this->handleAddResourcesEvent(addResourcesEvent);
		}
		else if (std::shared_ptr<SubResourcesEvent> subResourcesEvent = std::dynamic_pointer_cast<SubResourcesEvent>(e.at(i))) {
			this->handleSubResourcesEvent(subResourcesEvent);
		}
		else if (std::shared_ptr<ChangeHighlightEvent> changeHighlightEvent = std::dynamic_pointer_cast<ChangeHighlightEvent>(e.at(i))) {
			this->handleChangeHighlightEvent(changeHighlightEvent);
		}
		else if (std::shared_ptr<CollectEvent> collectEvent = std::dynamic_pointer_cast<CollectEvent>(e.at(i))) {
			this->handleCollectEvent(collectEvent);
		}
		else if (std::shared_ptr<TryToUpgradeEvent> tryToUpgradeEvent = std::dynamic_pointer_cast<TryToUpgradeEvent>(e.at(i))) {
			this->handleTryToUpgradeEvent(tryToUpgradeEvent);
		}
		else if (std::shared_ptr<AddHpEvent> addHpEvent = std::dynamic_pointer_cast<AddHpEvent>(e.at(i))) {
			this->handleAddHpEvent(addHpEvent);
		}
		else if (std::shared_ptr<DecreaseUpgradeMovesLeftEvent> decreaseUpgradeMovesLeftEvent = std::dynamic_pointer_cast<DecreaseUpgradeMovesLeftEvent>(e.at(i))) {
			this->handleDecreaseUpgradeMovesLeftEvent(decreaseUpgradeMovesLeftEvent);
		}
		else if (std::shared_ptr<IncreaseLevelEvent> increaseLevelEvent = std::dynamic_pointer_cast<IncreaseLevelEvent>(e.at(i))) {
			this->handleIncreaseLevelEvent(increaseLevelEvent);
		}
		else if (std::shared_ptr<DecreaseCurrentTradeMovesLeftEvent> decreaseCurrentTradeMovesLeftEvent = std::dynamic_pointer_cast<DecreaseCurrentTradeMovesLeftEvent>(e.at(i))) {
			this->handleDecreaseCurrentTradeMovesLeft(decreaseCurrentTradeMovesLeftEvent);
		}
		else if (std::shared_ptr<TryToBuildEvent> tryToBuildEvent = std::dynamic_pointer_cast<TryToBuildEvent>(e.at(i))) {
			this->handleTryToBuild(tryToBuildEvent);
		}
		else if (std::shared_ptr<BuildEvent> buildEvent = std::dynamic_pointer_cast<BuildEvent>(e.at(i))) {
			this->handleBuild(buildEvent);
		}
		else if (std::shared_ptr<PlaySoundEvent> playSoundEvent = std::dynamic_pointer_cast<PlaySoundEvent>(e.at(i))) {
			this->handlePlaySoundEvent(playSoundEvent);
		}
		else if (std::shared_ptr<CreateEEvent> createEEvent = std::dynamic_pointer_cast<CreateEEvent>(e.at(i))) {
			this->handleCreatePopUpElementEvent(createEEvent);
		}
		else if (std::shared_ptr<ChangeMoveEvent> changeMoveEvent = std::dynamic_pointer_cast<ChangeMoveEvent>(e.at(i))) {
			this->handleChangeMoveEvent(changeMoveEvent);
		}
		else if (std::shared_ptr<ReturnToMenuEvent> returnToMenuEvent = std::dynamic_pointer_cast<ReturnToMenuEvent>(e.at(i))) {
			this->handleReturnToMenuEvent(returnToMenuEvent);
		}
	}
}
void MainScreen::handleTryToAttackEvent(std::shared_ptr<TryToAttackEvent> e) {
    
}
void MainScreen::handleTryToTradeEvent(std::shared_ptr<TryToTradeEvent> e) {
	Market* m = e->getMarket();
	Trade t = e->getTrade();
	if (this->getCurrentPlayer()->getResource(t.sell.type) >= t.sell.n) {
		Events events = m->doTrade(t);
		this->handleEvent(events);
	}
	else {
		std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>("click", "click", *Texts::get()->get("no_resources_for_trade"), *Texts::get()->get("OK"));
		this->addPopUpWindow(w);
	}
}
void MainScreen::handleAddResourceEvent(std::shared_ptr<AddResourceEvent> e) {
	this->getCurrentPlayer()->addResource(e->getResource());
}
void MainScreen::handleSubResourceEvent(std::shared_ptr<SubResourceEvent> e) {
	this->getCurrentPlayer()->subResource(e->getResource());
}
void MainScreen::handleAddResourcesEvent(std::shared_ptr<AddResourcesEvent> e) {
	this->getCurrentPlayer()->addResources(e->getResources());
}
void MainScreen::handleSubResourcesEvent(std::shared_ptr<SubResourcesEvent> e) {
	this->getCurrentPlayer()->subResources(e->getResources());
}
void MainScreen::handleChangeHighlightEvent(std::shared_ptr<ChangeHighlightEvent> e) {
	const Unit* u = e->getUnit();
	uint32_t x = e->getX();
	uint32_t y = e->getY();
	if (x >= this->plains.getW() or y >= this->plains.getH()) {
		return;
	}
	this->highlightTable.mark(x, y, u);
}
void MainScreen::handleCollectEvent(std::shared_ptr<CollectEvent> e) {
	ResourcePoint* resourcePoint = e->getRp();
	uint32_t n = e->getN();
	this->getCurrentPlayer()->addResource(Resource(resourcePoint->getResourceType(), n));
	resourcePoint->subHp(n);
}
void MainScreen::handleTryToUpgradeEvent(std::shared_ptr<TryToUpgradeEvent> e) {
	UpgradeableB* b = e->getBuilding();
	Resources cost = b->getUpgradeCost();
	if (this->getCurrentPlayer()->getResources() >= cost) {
		Events events = b->startUpgrade();
		this->handleEvent(events);
	}
	else {
		std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>("click", "click", *Texts::get()->get("no_resources_for_upgrade"), *Texts::get()->get("OK"));
		this->addPopUpWindow(w);
	}
}
void MainScreen::handleAddHpEvent(std::shared_ptr<AddHpEvent> e) {
	HPGO* go = e->getHPGO();
	uint32_t n = e->getN();
	go->addHp(n);
}
void MainScreen::handleDecreaseUpgradeMovesLeftEvent(std::shared_ptr<DecreaseUpgradeMovesLeftEvent> e) {
	e->getBuilding()->decreaseUpgradeMovesLeft();
}
void MainScreen::handleIncreaseLevelEvent(std::shared_ptr<IncreaseLevelEvent> e) {
	e->getB()->increaseLevel();
}
void MainScreen::handleDecreaseCurrentTradeMovesLeft(std::shared_ptr<DecreaseCurrentTradeMovesLeftEvent> e) {
	e->getMarket()->decreaseCurrentTradeMovesLeft();
}
void MainScreen::handleTryToBuild(std::shared_ptr<TryToBuildEvent> e) {
	if (this->getCurrentPlayer()->getResources() >= e->getBuilding()->getCost()) {
		this->addPopUpWindow(std::make_shared<BuildingMode>(e->getBuilding(), this->view, this->map->getGO(), this->map->getTbs(), this->getCurrentPlayer()));
	}
	else {
		std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>("", "click", *Texts::get()->get("no_resources_for_building"), *Texts::get()->get("OK"));
		this->addPopUpWindow(w);
	}
}
void MainScreen::handleBuild(std::shared_ptr<BuildEvent> e) {
	Building* b = e->getBuilding();
	b->changePlayer(this->getCurrentPlayer());
	this->map->add(b);
}
void MainScreen::handlePlaySoundEvent(std::shared_ptr<PlaySoundEvent> e) {
	SoundQueue::get()->push(Sounds::get()->get(e->getSoundName()));
}
void MainScreen::handleCreatePopUpElementEvent(std::shared_ptr<CreateEEvent> e) {
	this->addPopUpWindow(e->getElement());
}
void MainScreen::handleChangeMoveEvent(std::shared_ptr<ChangeMoveEvent> e) {
	this->changeMove();
}
void MainScreen::handleReturnToMenuEvent(std::shared_ptr<ReturnToMenuEvent> e) {
	this->returnToMenu = true;
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
		Events events = this->elements.front()->run(this->windowW, this->windowH);
		this->handleEvent(events);
	}
}
void MainScreen::changeMove() {
	this->move = this->move + 1;
	this->updatePlayerViewPoint();
	this->highlightTable.clear();
	for (uint32_t i = 0; i < this->map->getGO()->size(); i = i + 1) {
		Events events = this->map->getGO()->at(i)->newMove(this->getCurrentPlayer());
		this->handleEvent(events);
	}
}
std::wstring MainScreen::GET_BUILD_DESCRIPTION(std::unique_ptr<Building> b) {
	std::wstring description = b->getDescription() + L'\n' +
		*Texts::get()->get("cost") + b->getCost().getReadableInfo();
	return description;
		
}
std::shared_ptr<Player> MainScreen::getCurrentPlayer() {
	return this->map->getPlayer((this->move + 1) % this->map->getPlayersNumber());
}
bool MainScreen::handleButtonsClick() {
    for (uint32_t i = 0; i < this->buttons.size(); i = i + 1) {
        Events event = this->buttons[i].click();
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
		Events events = this->map->getGO()->at(i)->click(this->getCurrentPlayer(), mouseX, mouseY);
		this->handleEvent(events);
	}
}
void MainScreen::addPopUpWindow(std::shared_ptr<PopUpElement> w) {
	this->elements.push(w);
	if (this->elements.size() == 1) {
		Events events = w->run(this->windowW, this->windowH);
		this->handleEvent(events);
	}
}
void MainScreen::prepareToReturnToMenu(sf::RenderWindow &window) {
    this->removeFinishedElements();
    this->highlightTable.clear();
	Playlist::get()->restartMusic();
}
void MainScreen::drawEverything(sf::RenderWindow& window) {
	window.clear();
	window.setView(*this->view);
	this->drawCells(window);
	window.draw(*this->map);
	if (!this->elements.empty()) {
		if (!this->elements.front()->isCameraDependent()) {
			window.setView(window.getDefaultView());
		}
		window.draw(*this->elements.front());
	}
	window.setView(window.getDefaultView());
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