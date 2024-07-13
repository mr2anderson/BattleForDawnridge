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
#include "WindowTwoButtons.hpp"
#include "ResourceBar.hpp"
#include "WarehouseFood.hpp"
#include "WarehouseWood.hpp"
#include "WarehouseStone.hpp"
#include "WarehouseIron.hpp"
#include "WarehouseGold.hpp"
#include "Barracks.hpp"
#include "Playlist.hpp"
#include "Castle.hpp"
#include "Market.hpp"
#include "Arable.hpp"
#include "Sawmill.hpp"
#include "Quarry.hpp"
#include "Mine.hpp"
#include "Wall1.hpp"
#include "Wall2.hpp"
#include "Road.hpp"
#include "SoundQueue.hpp"
#include "BuildingMode.hpp"
#include "Sounds.hpp"
#include "WindowButton.hpp"
#include "Textures.hpp"
#include "Texts.hpp"







MainScreen* MainScreen::singletone = nullptr;






bool MainScreen::run(std::shared_ptr<Map> mapPtr, sf::RenderWindow& window) {
    this->init(mapPtr, window);
	sf::Event event{};
	for (; ;) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::MouseButtonPressed and event.mouseButton.button == sf::Mouse::Left) {
				if (this->elements.empty() and this->allNewMoveEventsHandled()) {
					if (this->selected == nullptr) {
						if (!this->handleButtonsClick()) {
							this->handleGameObjectClick();
						}
					}
					else {
						std::tuple<uint32_t, uint32_t> pos = this->getMousePositionBasedOnView();
						Events events = this->selected->unselect(std::get<0>(pos) / 64, std::get<1>(pos) / 64);
						this->handleEvent(events);
					}
				}
				else {
					Events events = this->elements.front()->click();
					this->handleEvent(events);
				}
			}
		}
		this->drawEverything(window);
		Playlist::get()->update();
		this->removeFinishedElements();
		this->handleNewMoveEvents();
		if (!this->elements.empty()) {
			this->elements.front()->update();
		}
		this->moveView();
        if (this->returnToMenu) {
			this->prepareToReturnToMenu(window);
            return true;
        }
		window.setMouseCursorVisible(this->curcorVisibility);
	}
}










void MainScreen::init(std::shared_ptr<Map> mapPtr, sf::RenderWindow& window) {
    this->initMap(mapPtr);
	this->initPlayerIsActiveTable();
	this->initCurrentPlayerIndex();
    this->initMoveCtr();
	this->initSelectable();
    this->initPlains();
    this->initGraphics(window);
	this->changeMove();
}
void MainScreen::initMap(std::shared_ptr<Map> mapPtr) {
    this->map = mapPtr;
}
void MainScreen::initPlayerIsActiveTable() {
	this->playerIsActive.resize(this->map->getPlayersNumber(), true);
}
void MainScreen::initCurrentPlayerIndex() {
	this->currentPlayerIndex = 0;
}
void MainScreen::initMoveCtr() {
	this->move = 0;
}
void MainScreen::initSelectable() {
	this->selected = nullptr;
}
void MainScreen::initPlains() {
    this->plains = PlainsGeneration(this->map->getW(), this->map->getH());
}
void MainScreen::initGraphics(sf::RenderWindow &window) {
    Events endMoveEvent;
	endMoveEvent.add(std::make_shared<ChangeMoveEvent>());

	std::shared_ptr<WindowTwoButtons> confirmEndMoveWindow = std::make_shared<WindowTwoButtons>("click", "click", *Texts::get()->get("confirm_end_move"), *Texts::get()->get("yes"), *Texts::get()->get("no"), endMoveEvent, Events());
	Events createConfirmEndMoveWindowEvent;
	createConfirmEndMoveWindowEvent.add(std::make_shared<CreateEEvent>(confirmEndMoveWindow));

    Events returnToMenuEvent;
	returnToMenuEvent.add(std::make_shared<ReturnToMenuEvent>());
    returnToMenuEvent.add(std::make_shared<PlaySoundEvent>("click"));

	std::shared_ptr<WindowTwoButtons> confirmReturnToMenuWindow = std::make_shared<WindowTwoButtons>("click", "click", *Texts::get()->get("confirm_return_to_menu"), *Texts::get()->get("yes"), *Texts::get()->get("no"), returnToMenuEvent, Events());
	Events createConfirmReturnToMenuWindowEvent;
	createConfirmReturnToMenuWindowEvent.add(std::make_shared<CreateEEvent>(confirmReturnToMenuWindow));

	Events createBuildingModeEvent;


	std::vector<HorizontalSelectionWindowComponent> buildMenuSectionMainComponents;
	createBuildingModeEvent = Events();
	createBuildingModeEvent.add(std::make_shared<TryToBuildEvent>(std::make_shared<Road>(0, 0, 0, this->map->getUnits(), this->map->getTobs())));
	buildMenuSectionMainComponents.emplace_back(Road().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<Road>()), true, true, createBuildingModeEvent);

	std::shared_ptr<HorizontalSelectionWindow> buildWindowSectionMain = std::make_shared<HorizontalSelectionWindow>("click", "click", buildMenuSectionMainComponents);
	Events createBuildWindowSectionMainEvent;
	createBuildWindowSectionMainEvent.add(std::make_shared<CreateEEvent>(buildWindowSectionMain));



	std::vector<HorizontalSelectionWindowComponent> buildMenuSectionResourceCollectorsComponents;
	createBuildingModeEvent = Events();
	createBuildingModeEvent.add(std::make_shared<TryToBuildEvent>(std::make_shared<Arable>(0, 0, 0, this->map->getUnits())));
	buildMenuSectionResourceCollectorsComponents.emplace_back(Arable().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<Arable>()), true, true, createBuildingModeEvent);
	createBuildingModeEvent = Events();
	createBuildingModeEvent.add(std::make_shared<TryToBuildEvent>(std::make_shared<Sawmill>(0, 0, 0, this->map->getUnits(), this->map->getResourcePoints())));
	buildMenuSectionResourceCollectorsComponents.emplace_back(Sawmill().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<Sawmill>()), true, true, createBuildingModeEvent);
	createBuildingModeEvent = Events();
	createBuildingModeEvent.add(std::make_shared<TryToBuildEvent>(std::make_shared<Quarry>(0, 0, 0, this->map->getUnits(), this->map->getResourcePoints())));
	buildMenuSectionResourceCollectorsComponents.emplace_back(Quarry().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<Quarry>()), true, true, createBuildingModeEvent);
	createBuildingModeEvent = Events();
	createBuildingModeEvent.add(std::make_shared<TryToBuildEvent>(std::make_shared<Mine>(0, 0, 0, this->map->getUnits(), this->map->getResourcePoints())));
	buildMenuSectionResourceCollectorsComponents.emplace_back(Mine().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<Mine>()), true, true, createBuildingModeEvent);

	std::shared_ptr<HorizontalSelectionWindow> buildWindowSectionResourceCollectors = std::make_shared<HorizontalSelectionWindow>("click", "click", buildMenuSectionResourceCollectorsComponents);
	Events createBuildWindowSectionResourceCollectorsEvent;
	createBuildWindowSectionResourceCollectorsEvent.add(std::make_shared<CreateEEvent>(buildWindowSectionResourceCollectors));



	std::vector<HorizontalSelectionWindowComponent> buildMenuSectionWarehousesComponents;
	createBuildingModeEvent = Events();
	createBuildingModeEvent.add(std::make_shared<TryToBuildEvent>(std::make_shared<WarehouseFood>(0, 0, 0, this->map->getUnits())));
	buildMenuSectionWarehousesComponents.emplace_back(WarehouseFood().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<WarehouseFood>()), true, true, createBuildingModeEvent);
	createBuildingModeEvent = Events();
	createBuildingModeEvent.add(std::make_shared<TryToBuildEvent>(std::make_shared<WarehouseWood>(0, 0, 0, this->map->getUnits())));
	buildMenuSectionWarehousesComponents.emplace_back(WarehouseWood().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<WarehouseWood>()), true, true, createBuildingModeEvent);
	createBuildingModeEvent = Events();
	createBuildingModeEvent.add(std::make_shared<TryToBuildEvent>(std::make_shared<WarehouseStone>(0, 0, 0, this->map->getUnits())));
	buildMenuSectionWarehousesComponents.emplace_back(WarehouseStone().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<WarehouseStone>()), true, true, createBuildingModeEvent);
	createBuildingModeEvent = Events();
	createBuildingModeEvent.add(std::make_shared<TryToBuildEvent>(std::make_shared<WarehouseIron>(0, 0, 0, this->map->getUnits())));
	buildMenuSectionWarehousesComponents.emplace_back(WarehouseIron().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<WarehouseIron>()), true, true, createBuildingModeEvent);
	createBuildingModeEvent = Events();
	createBuildingModeEvent.add(std::make_shared<TryToBuildEvent>(std::make_shared<WarehouseGold>(0, 0, 0, this->map->getUnits(), this->map->getTreasures())));
	buildMenuSectionWarehousesComponents.emplace_back(WarehouseGold().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<WarehouseGold>()), true, true, createBuildingModeEvent);
	
	std::shared_ptr<HorizontalSelectionWindow> buildWindowSectionWarehouses = std::make_shared<HorizontalSelectionWindow>("click", "click", buildMenuSectionWarehousesComponents);
	Events createBuildWindowSectionWarehousesEvent;
	createBuildWindowSectionWarehousesEvent.add(std::make_shared<CreateEEvent>(buildWindowSectionWarehouses));



	std::vector<HorizontalSelectionWindowComponent> buildMenuSectionTroops;



    std::vector<HorizontalSelectionWindowComponent> buildMenuComponents;
    buildMenuComponents.emplace_back("hammer_icon", *Texts::get()->get("leave"), true, true, Events());
    
    
    
    
	createBuildingModeEvent = Events();
    createBuildingModeEvent.add((std::make_shared<TryToBuildEvent>(std::make_shared<Market>(0, 0, 0, this->map->getUnits()))));
    buildMenuComponents.emplace_back(Market().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<Market>()), true, true, createBuildingModeEvent);
    createBuildingModeEvent = Events();
	createBuildingModeEvent.add((std::make_shared<TryToBuildEvent>(std::make_shared<Barracks>(0, 0, 0, this->map->getUnits(), this->map->getGO(), this->map->getW(), this->map->getH()))));
	buildMenuComponents.emplace_back(Barracks().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<Barracks>()), true, true, createBuildingModeEvent);
	createBuildingModeEvent = Events();
    createBuildingModeEvent.add((std::make_shared<TryToBuildEvent>(std::make_shared<Wall1>(0, 0, 0, this->map->getUnits()))));
    buildMenuComponents.emplace_back(Wall1().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<Wall1>()), true, true, createBuildingModeEvent);
    createBuildingModeEvent = Events();
	createBuildingModeEvent.add((std::make_shared<TryToBuildEvent>(std::make_shared<Wall2>(0, 0, 0, this->map->getUnits()))));
	buildMenuComponents.emplace_back(Wall2().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<Wall2>()), true, true, createBuildingModeEvent);
	createBuildingModeEvent = Events();
    createBuildingModeEvent.add(std::make_shared<TryToBuildEvent>(std::make_shared<Castle>(0, 0, 0, this->map->getUnits())));
    buildMenuComponents.emplace_back(Castle().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<Castle>()), true, true, createBuildingModeEvent);
    std::shared_ptr<HorizontalSelectionWindow> buildWindow = std::make_shared<HorizontalSelectionWindow>("click", "click", buildMenuComponents);
    Events buildEvent;
    buildEvent.add(std::make_shared<CreateEEvent>(buildWindow));

	this->windowW = window.getSize().x;
	this->windowH = window.getSize().y;
    this->returnToMenu = false;
	this->curcorVisibility = true;
	this->view = std::make_shared<sf::View>(window.getDefaultView());

	this->buttons.emplace_back(std::make_shared<Label>(this->windowW - 10 - 200, 40, 200, 30, *Texts::get()->get("new_move")), createConfirmEndMoveWindowEvent);
	this->buttons.emplace_back(std::make_shared<Image>(this->windowW - 10 - 200, 40 + 10 + 30, "hammer_icon"), buildEvent);
    this->buttons.emplace_back(std::make_shared<Label>(5, 40, 200, 30, *Texts::get()->get("to_menu")), createConfirmReturnToMenuWindowEvent);
}
std::wstring MainScreen::GET_BUILD_DESCRIPTION(std::unique_ptr<Building> b) {
	std::wstring description = b->getDescription() + L'\n' +
		*Texts::get()->get("cost") + b->getCost().getReadableInfo();
	return description;
}










void MainScreen::drawEverything(sf::RenderWindow& window) {
	window.clear();
	window.setView(*this->view);
	this->drawCells(window);
	window.draw(*this->map);
	this->drawHighlightion(window);
	if (this->selected != nullptr) {
		window.draw(this->selected->getSprite(this->getMousePositionBasedOnView()));
	}
	if (!this->elements.empty()) {
		if (!this->elements.front()->isCameraDependent()) {
			window.setView(window.getDefaultView());
		}
		window.draw(*this->elements.front());
	}
	window.setView(window.getDefaultView());
	this->drawResourceBar(window);
	for (const auto& b : this->buttons) {
		window.draw(b);
	}
	window.display();
}
void MainScreen::drawResourceBar(sf::RenderWindow& window) {
	ResourceBar bar;
	bar.setResources(this->getCurrentPlayer()->getResources());
	bar.setLimit(this->getResourcesLimit());
	window.draw(bar);
}
void MainScreen::drawCells(sf::RenderWindow& window) {
	for (uint32_t i = 0; i < this->plains.getW(); i = i + 1) {
		for (uint32_t j = 0; j < this->plains.getH(); j = j + 1) {
			sf::Sprite s;
			s.setTexture(*Textures::get()->get(std::to_string(this->plains.getType(i, j) + 1)));
			s.setPosition(64 * i, 64 * j);
			window.draw(s);
		}
	}
}
void MainScreen::drawHighlightion(sf::RenderWindow& window) {
	std::vector<sf::RectangleShape> rects = this->highlightTable.getRects();
	for (const auto& rect : rects) {
		window.draw(rect);
	}
}










void MainScreen::removeFinishedElements() {
	bool remove = false;
	while (!this->elements.empty()) {
		if (!this->elements.front()->finished()) {
			break;
		}
		this->elements.front()->restart();
		this->elements.pop();
		remove = true;
	}
	if (remove and !this->elements.empty()) {
		Events events = this->elements.front()->run(this->windowW, this->windowH);
		this->handleEvent(events);
	}
}
void MainScreen::handleNewMoveEvents() {
	while (this->currentGOIndexNewMoveEvent != this->totalGONewMoveEvents) {
		if (!this->elements.empty()) {
			break;
		}
		Events event = this->map->getGO()->at(this->currentGOIndexNewMoveEvent)->newMove(this->getCurrentPlayer()->getId());
		this->handleEvent(event);
		this->currentGOIndexNewMoveEvent = this->currentGOIndexNewMoveEvent + 1;
	}
}
bool MainScreen::allNewMoveEventsHandled() const {
	return (this->currentGOIndexNewMoveEvent == this->totalGONewMoveEvents);
}
void MainScreen::changeMove() {
	this->move = this->move + 1;
	this->currentGOIndexNewMoveEvent = 0;
	this->totalGONewMoveEvents = this->map->getGO()->size();
	do {
		this->currentPlayerIndex = (this->currentPlayerIndex + 1) % this->map->getPlayersNumber();
	} while (!this->playerIsActive.at(this->currentPlayerIndex));
	this->updatePlayerViewPoint();
	this->highlightTable.clear();
}
Player* MainScreen::getCurrentPlayer() {
	return this->map->getPlayer((this->move + 1) % this->map->getPlayersNumber());
}
Resources MainScreen::getResourcesLimit() {
	Resources limit;
	std::shared_ptr<GOCollection<ResourceStorageB>> rsbs = this->map->getRsbs();
	for (uint32_t i = 0; i < rsbs->size(); i = i + 1) {
		ResourceStorageB* rsb = rsbs->at(i);
		if (rsb->exist() and rsb->getPlayerId() == this->getCurrentPlayer()->getId()) {
			limit.plus(rsb->getLimit());
		}
	}
	return limit;
}
bool MainScreen::handleButtonsClick() {
	for (uint32_t i = 0; i < this->buttons.size(); i = i + 1) {
		Events event = this->buttons.at(i).click();
		if (!event.empty()) {
			if (this->selected != nullptr) {
				this->selected->unselect();
			}
			this->handleEvent(event);
			return true;
		}
	}
	return false;
}
void MainScreen::handleGameObjectClick() {
	uint32_t mouseX, mouseY;
	std::tie(mouseX, mouseY) = this->getMousePositionBasedOnView();

	for (int8_t c = 1; c >= 0; c = c - 1) {
		for (uint32_t i = 0; i < this->map->getGO()->size(); i = i + 1) {
			GO* go = this->map->getGO()->at(i);
			if (go->highClickPriority() == c) {
				Events events = go->click(this->getCurrentPlayer()->getId(), mouseX, mouseY);
				if (!events.empty()) {
					this->handleEvent(events);
					return;
				}
			}
		}
	}
}
void MainScreen::addPopUpWindow(std::shared_ptr<PopUpElement> w) {
	this->elements.push(w);
	if (this->elements.size() == 1) {
		Events events = w->run(this->windowW, this->windowH);
		this->handleEvent(events);
	}
}
void MainScreen::prepareToReturnToMenu(sf::RenderWindow& window) {
	this->highlightTable.clear();
	this->playerIsActive.clear();
	Playlist::get()->restartMusic();
}
std::tuple<uint32_t, uint32_t> MainScreen::getMousePositionBasedOnView() const {
	uint32_t mouseX = sf::Mouse::getPosition().x + this->view->getCenter().x - this->windowW / 2;
	uint32_t mouseY = sf::Mouse::getPosition().y + this->view->getCenter().y - this->windowH / 2;
	return std::make_tuple(mouseX, mouseY);
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
	std::shared_ptr<GOCollection<VictoryConditionB>> vcbs = this->map->getVcbs();
	for (uint32_t i = 0; i < vcbs->size(); i = i + 1) {
		VictoryConditionB* vcb = vcbs->at(i);
		if (vcb->exist() and vcb->getPlayerId() == this->getCurrentPlayer()->getId()) {
			this->view->setCenter(64 * vcb->getX(), 64 * vcb->getY());
			this->verifyViewNorth();
			this->verifyViewSouth();
			this->verifyViewWest();
			this->verifyViewEast();
		}
	}
}










void MainScreen::moveViewToNorth() {
	this->view->setCenter(this->view->getCenter() - sf::Vector2f(0, 10));
	this->verifyViewNorth();
}
void MainScreen::moveViewToSouth() {
	this->view->setCenter(this->view->getCenter() + sf::Vector2f(0, 10));
	this->verifyViewSouth();
}
void MainScreen::moveViewToWest() {
	this->view->setCenter(this->view->getCenter() - sf::Vector2f(10, 0));
	this->verifyViewWest();
}
void MainScreen::moveViewToEast() {
	this->view->setCenter(this->view->getCenter() + sf::Vector2f(10, 0));
	this->verifyViewEast();
}
void MainScreen::verifyViewNorth() {
	if (this->view->getCenter().y < this->windowH / 2) {
		this->view->setCenter(sf::Vector2f(this->view->getCenter().x, this->windowH / 2));
	}
}
void MainScreen::verifyViewSouth() {
	if (this->view->getCenter().y > 64 * this->plains.getH() - this->windowH / 2) {
		this->view->setCenter(sf::Vector2f(this->view->getCenter().x, 64 * this->plains.getH() - this->windowH / 2));
	}
}
void MainScreen::verifyViewWest() {
	if (this->view->getCenter().x < this->windowW / 2) {
		this->view->setCenter(sf::Vector2f(this->windowW / 2, this->view->getCenter().y));
	}
}
void MainScreen::verifyViewEast() {
	if (this->view->getCenter().x > 64 * this->plains.getW() - this->windowW / 2) {
		this->view->setCenter(sf::Vector2f(64 * this->plains.getW() - this->windowW / 2, this->view->getCenter().y));
	}
}










void MainScreen::handleEvent(Events& e) {
	for (uint32_t i = 0; i < e.size(); i = i + 1) {
		if (std::shared_ptr<TryToTradeEvent> tryToTradeEvent = std::dynamic_pointer_cast<TryToTradeEvent>(e.at(i))) {
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
		else if (std::shared_ptr<AddHpEvent> addHpEvent = std::dynamic_pointer_cast<AddHpEvent>(e.at(i))) {
			this->handleAddHpEvent(addHpEvent);
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
		else if (std::shared_ptr<DestroyEvent> destroyEvent = std::dynamic_pointer_cast<DestroyEvent>(e.at(i))) {
			this->handleDestroyEvent(destroyEvent);
		}
		else if (std::shared_ptr<ResourceStorageBDestroyedEvent> resourceStorageBDestroyedEvent = std::dynamic_pointer_cast<ResourceStorageBDestroyedEvent>(e.at(i))) {
			this->handleResourceStorageBDestroyedEvent(resourceStorageBDestroyedEvent);
		}
		else if (std::shared_ptr<VictoryConditionBDestroyedEvent> victoryConditionBDestroyedEvent = std::dynamic_pointer_cast<VictoryConditionBDestroyedEvent>(e.at(i))) {
			this->handleVictoryConditionBDestroyedEvent(victoryConditionBDestroyedEvent);
		}
		else if (std::shared_ptr<DecreaseCurrentProducingMovesLeftEvent> decreaseCurrentProducingMovesLeftEvent = std::dynamic_pointer_cast<DecreaseCurrentProducingMovesLeftEvent>(e.at(i))) {
			this->handleDecreaseCurrentProdusingMovesLeftEvent(decreaseCurrentProducingMovesLeftEvent);
		}
		else if (std::shared_ptr<TryToProduceEvent> tryToProduceEvent = std::dynamic_pointer_cast<TryToProduceEvent>(e.at(i))) {
			this->handleTryToProduceEvent(tryToProduceEvent);
		}
		else if (std::shared_ptr<WarriorProducingFinishedEvent> warriorProducingFinishedEvent = std::dynamic_pointer_cast<WarriorProducingFinishedEvent>(e.at(i))) {
			this->handleWarriorProducingFinishedEvent(warriorProducingFinishedEvent);
		}
		else if (std::shared_ptr<SelectEvent> selectEvent = std::dynamic_pointer_cast<SelectEvent>(e.at(i))) {
			this->handleSelectEvent(selectEvent);
		}
		else if (std::shared_ptr<UnselectEvent> unselectEvent = std::dynamic_pointer_cast<UnselectEvent>(e.at(i))) {
			this->handleUnselectEvent(unselectEvent);
		}
		else if (std::shared_ptr<StartWarriorClickAnimationEvent> startWarriorClickAnimationEvent = std::dynamic_pointer_cast<StartWarriorClickAnimationEvent>(e.at(i))) {
			this->handleStartWarriorClickAnimationEvent(startWarriorClickAnimationEvent);
		}
		else if (std::shared_ptr<TryToCollectEvent> tryToCollectEvent = std::dynamic_pointer_cast<TryToCollectEvent>(e.at(i))) {
			this->handleTryToCollectEvent(tryToCollectEvent);
		}
		else if (std::shared_ptr<RefreshMovementPointsEvent> refreshMovementPointsEvent = std::dynamic_pointer_cast<RefreshMovementPointsEvent>(e.at(i))) {
			this->handleRefreshMovementPointsEvent(refreshMovementPointsEvent);
		}
		else if (std::shared_ptr<EnableCursorEvent> enableCursorEvent = std::dynamic_pointer_cast<EnableCursorEvent>(e.at(i))) {
			this->handleEnableCursorEvent(enableCursorEvent);
		}
		else if (std::shared_ptr<DisableCursorEvent> disableCursorEvent = std::dynamic_pointer_cast<DisableCursorEvent>(e.at(i))) {
			this->handleDisableCursorEvent(disableCursorEvent);
		}
	}
}
void MainScreen::handleTryToTradeEvent(std::shared_ptr<TryToTradeEvent> e) {
	TradingB* b = e->getBuilding();
	Trade t = e->getTrade();
	if (this->getCurrentPlayer()->getResource(t.sell.type) >= t.sell.n) {
		Events events = b->doTrade(t);
		this->handleEvent(events);
	}
	else {
		std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>("click", "click", *Texts::get()->get("no_resources_for_trade"), *Texts::get()->get("OK"));
		this->addPopUpWindow(w);
	}
}
void MainScreen::handleAddResourceEvent(std::shared_ptr<AddResourceEvent> e) {
	this->getCurrentPlayer()->addResource(e->getResource(), this->getResourcesLimit().get(e->getResource().type));
}
void MainScreen::handleSubResourceEvent(std::shared_ptr<SubResourceEvent> e) {
	this->getCurrentPlayer()->subResource(e->getResource());
}
void MainScreen::handleAddResourcesEvent(std::shared_ptr<AddResourcesEvent> e) {
	this->getCurrentPlayer()->addResources(e->getResources(), this->getResourcesLimit());
}
void MainScreen::handleSubResourcesEvent(std::shared_ptr<SubResourcesEvent> e) {
	this->getCurrentPlayer()->subResources(e->getResources());
}
void MainScreen::handleChangeHighlightEvent(std::shared_ptr<ChangeHighlightEvent> e) {
	this->highlightTable.mark(*e);
}
void MainScreen::handleCollectEvent(std::shared_ptr<CollectEvent> e) {
	ResourcePoint* resourcePoint = e->getRp();
	uint32_t n = e->getN();
	resourcePoint->subHp(n);
	std::shared_ptr<AddResourceEvent> addResourceEvent = std::make_shared<AddResourceEvent>(Resource(resourcePoint->getResourceType(), n));
	this->handleAddResourceEvent(addResourceEvent);
}
void MainScreen::handleAddHpEvent(std::shared_ptr<AddHpEvent> e) {
	HPGO* go = e->getHPGO();
	uint32_t n = e->getN();
	go->addHp(n);
}
void MainScreen::handleDecreaseCurrentTradeMovesLeft(std::shared_ptr<DecreaseCurrentTradeMovesLeftEvent> e) {
	e->getBuilding()->decreaseCurrentTradeMovesLeft();
}
void MainScreen::handleTryToBuild(std::shared_ptr<TryToBuildEvent> e) {
	if (this->getCurrentPlayer()->getResources() >= e->getBuilding()->getCost()) {
		this->addPopUpWindow(std::make_shared<BuildingMode>(e->getBuilding(), this->view, this->map->getGO(), this->map->getTbs(), this->getCurrentPlayer()->getId(), this->map->getW(), this->map->getH()));
	}
	else {
		std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>("", "click", *Texts::get()->get("no_resources_for_building"), *Texts::get()->get("OK"));
		this->addPopUpWindow(w);
	}
}
void MainScreen::handleBuild(std::shared_ptr<BuildEvent> e) {
	Building* b = e->getBuilding();
	b->changePlayer(this->getCurrentPlayer()->getId());
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
void MainScreen::handleDestroyEvent(std::shared_ptr<DestroyEvent> e) {
	Events events = e->getBuilding()->destroy();
	this->handleEvent(events);
}
void MainScreen::handleResourceStorageBDestroyedEvent(std::shared_ptr<ResourceStorageBDestroyedEvent> e) {
	this->map->getPlayer(e->getPlayerId() - 1)->limitResources(this->getResourcesLimit());
}
void MainScreen::handleVictoryConditionBDestroyedEvent(std::shared_ptr<VictoryConditionBDestroyedEvent> e) {
	std::shared_ptr<GOCollection<VictoryConditionB>> vcbs = this->map->getVcbs();
	for (uint32_t i = 0; i < vcbs->size(); i = i + 1) {
		VictoryConditionB* vcb = vcbs->at(i);
		if (vcb->getPlayerId() == e->getPlayerId() and vcb->exist()) {
			return;
		}
	}
	this->playerIsActive.at(this->currentPlayerIndex) = false;
	uint32_t count = 0;
	for (uint32_t i = 0; i < this->playerIsActive.size(); i = i + 1) {
		count = count + this->playerIsActive.at(i);
	}
	std::shared_ptr<WindowButton> w;
	if (count == 1) {
		Events returnToMenu;
		returnToMenu.add(std::make_shared<ReturnToMenuEvent>());
		w = std::make_shared<WindowButton>("", "click", *Texts::get()->get("game_finished"), *Texts::get()->get("OK"), returnToMenu);
	}
	else {
		w = std::make_shared<WindowButton>("", "click", *Texts::get()->get("player_is_out"), *Texts::get()->get("OK"));
	}
	std::shared_ptr<CreateEEvent> createW = std::make_shared<CreateEEvent>(w);
	this->handleCreatePopUpElementEvent(createW);
}
void MainScreen::handleDecreaseCurrentProdusingMovesLeftEvent(std::shared_ptr<DecreaseCurrentProducingMovesLeftEvent> e) {
	e->getBuilding()->decreaseCurrentProducingMovesLeft();
}
void MainScreen::handleTryToProduceEvent(std::shared_ptr<TryToProduceEvent> e) {
	if (this->getCurrentPlayer()->getResources() >= e->getWarrior()->getCost()) {
		Events events;
		events.add(std::make_shared<SubResourcesEvent>(e->getWarrior()->getCost()));
		events = events + e->getProducer()->startProducing(e->getWarrior());
		this->handleEvent(events);
	}
	else {
		std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>("click", "click", *Texts::get()->get("no_resources_for_producing"), *Texts::get()->get("OK"));
		this->addPopUpWindow(w);
	}
}
void MainScreen::handleWarriorProducingFinishedEvent(std::shared_ptr<WarriorProducingFinishedEvent> e) {
	e->getProducer()->stopProducing();
	this->map->add(e->getWarrior()->cloneWarrior());
}
void MainScreen::handleSelectEvent(std::shared_ptr<SelectEvent> e) {
	this->selected = e->getSelectable();
}
void MainScreen::handleUnselectEvent(std::shared_ptr<UnselectEvent> e) {
	this->selected = nullptr;
}
void MainScreen::handleStartWarriorClickAnimationEvent(std::shared_ptr<StartWarriorClickAnimationEvent> e) {
	e->getWarrior()->startClickAnimation();
}
void MainScreen::handleTryToCollectEvent(std::shared_ptr<TryToCollectEvent> e) {
	uint32_t id = e->getPlayerId();
	ResourcePoint* rp = e->getResourcePoint();
	uint32_t value = e->getValue();

	uint32_t limit = this->getResourcesLimit().get(rp->getResourceType()) - this->getCurrentPlayer()->getResource(rp->getResourceType());
	if (value > limit) {
		value = limit;
	}

	Events events = rp->tryToCollect(id, value);
	this->handleEvent(events);
}
void MainScreen::handleRefreshMovementPointsEvent(std::shared_ptr<RefreshMovementPointsEvent> e) {
	e->getWarrior()->refreshMovementPoints();
}
void MainScreen::handleEnableCursorEvent(std::shared_ptr<EnableCursorEvent> e) {
	this->curcorVisibility = true;
}
void MainScreen::handleDisableCursorEvent(std::shared_ptr<DisableCursorEvent> e) {
	this->curcorVisibility = false;
}