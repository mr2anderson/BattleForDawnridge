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
#include "Gates1.hpp"
#include "Wall2.hpp"
#include "Gates2.hpp"
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
			if (event.type == sf::Event::MouseButtonPressed) {
                if (!this->animation.has_value() and this->viewMovingQueue.empty()) {
                    if (this->element == nullptr) {
                        if (this->baseEvents.empty() and this->allNewMoveEventsAdded()) {
                            if (this->selected == nullptr and event.mouseButton.button == sf::Mouse::Button::Left) {
                                this->addButtonClickEventToQueue();
                                if (this->baseEvents.empty()) {
                                    this->addGameObjectClickEventToQueue();
                                }
                            }
                            else if (event.mouseButton.button == sf::Mouse::Button::Left or event.mouseButton.button == sf::Mouse::Button::Right) {
                                std::tuple<uint32_t, uint32_t> pos = this->getMousePositionBasedOnView();
                                Events unselectEvent = this->selected->unselect(std::get<0>(pos) / 64, std::get<1>(pos) / 64, event.mouseButton.button);
                                this->addEvents(unselectEvent);
                            }
                        }
                    }
                    else if (event.mouseButton.button == sf::Mouse::Button::Left) {
                        Events elementClickEvent = this->element->click();
                        this->addEvents(elementClickEvent);
                    }
                }
			}
		}
		this->drawEverything(window);
		Playlist::get()->update();
		this->removeFinishedElement();
        this->addNewMoveEvent();
        this->processBaseEvents();
		if (this->element != nullptr) {
			this->element->update();
		}
        if (this->animation.has_value()) {
            Events animationEvent;
            animationEvent = this->animation.value().process();
            this->addEvents(animationEvent);
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
    Events clickSoundEvent;
    clickSoundEvent.add(std::make_shared<PlaySoundEvent>("click"));



    Events endMoveEvent = clickSoundEvent;
	endMoveEvent.add(std::make_shared<ChangeMoveEvent>());

	std::shared_ptr<WindowTwoButtons> confirmEndMoveWindow = std::make_shared<WindowTwoButtons>(*Texts::get()->get("confirm_end_move"), *Texts::get()->get("yes"), *Texts::get()->get("no"), endMoveEvent, clickSoundEvent);
	Events createConfirmEndMoveWindowEvent = clickSoundEvent;
	createConfirmEndMoveWindowEvent.add(std::make_shared<CreateEEvent>(confirmEndMoveWindow));




    Events returnToMenuEvent = clickSoundEvent;
	returnToMenuEvent.add(std::make_shared<ReturnToMenuEvent>());

	std::shared_ptr<WindowTwoButtons> confirmReturnToMenuWindow = std::make_shared<WindowTwoButtons>(*Texts::get()->get("confirm_return_to_menu"), *Texts::get()->get("yes"), *Texts::get()->get("no"), returnToMenuEvent, clickSoundEvent);
	Events createConfirmReturnToMenuWindowEvent = clickSoundEvent;
	createConfirmReturnToMenuWindowEvent.add(std::make_shared<CreateEEvent>(confirmReturnToMenuWindow));




	Events createBuildingModeEvent;
	std::vector<HorizontalSelectionWindowComponent> buildMenuSectionMainComponents;
    buildMenuSectionMainComponents.emplace_back("hammer_icon", *Texts::get()->get("leave"), true, clickSoundEvent);
	createBuildingModeEvent = clickSoundEvent;
	createBuildingModeEvent.add(std::make_shared<TryToBuildEvent>(std::make_shared<Road>(0, 0, 0, this->map->getUnits(), this->map->getGO(), this->map->getW(), this->map->getH(), this->map->getTobs())));
	buildMenuSectionMainComponents.emplace_back(Road().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<Road>()), true, createBuildingModeEvent);

	std::shared_ptr<HorizontalSelectionWindow> buildWindowSectionMain = std::make_shared<HorizontalSelectionWindow>(buildMenuSectionMainComponents);
	Events createBuildWindowSectionMainEvent = clickSoundEvent;
	createBuildWindowSectionMainEvent.add(std::make_shared<CreateEEvent>(buildWindowSectionMain));



	std::vector<HorizontalSelectionWindowComponent> buildMenuSectionResourceCollectorsComponents;
    buildMenuSectionResourceCollectorsComponents.emplace_back("hammer_icon", *Texts::get()->get("leave"), true, clickSoundEvent);
	createBuildingModeEvent = clickSoundEvent;
	createBuildingModeEvent.add(std::make_shared<TryToBuildEvent>(std::make_shared<Arable>(0, 0, 0, this->map->getUnits())));
	buildMenuSectionResourceCollectorsComponents.emplace_back(Arable().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<Arable>()), true, createBuildingModeEvent);
	createBuildingModeEvent = clickSoundEvent;
	createBuildingModeEvent.add(std::make_shared<TryToBuildEvent>(std::make_shared<Sawmill>(0, 0, 0, this->map->getUnits(), this->map->getResourcePoints(), this->map->getGO(), this->map->getW(), this->map->getH())));
	buildMenuSectionResourceCollectorsComponents.emplace_back(Sawmill().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<Sawmill>()), true, createBuildingModeEvent);
	createBuildingModeEvent = clickSoundEvent;
	createBuildingModeEvent.add(std::make_shared<TryToBuildEvent>(std::make_shared<Quarry>(0, 0, 0, this->map->getUnits(), this->map->getResourcePoints(), this->map->getGO(), this->map->getW(), this->map->getH())));
	buildMenuSectionResourceCollectorsComponents.emplace_back(Quarry().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<Quarry>()), true, createBuildingModeEvent);
	createBuildingModeEvent = clickSoundEvent;
	createBuildingModeEvent.add(std::make_shared<TryToBuildEvent>(std::make_shared<Mine>(0, 0, 0, this->map->getUnits(), this->map->getResourcePoints(), this->map->getGO(), this->map->getW(), this->map->getH())));
	buildMenuSectionResourceCollectorsComponents.emplace_back(Mine().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<Mine>()), true, createBuildingModeEvent);

	std::shared_ptr<HorizontalSelectionWindow> buildWindowSectionResourceCollectors = std::make_shared<HorizontalSelectionWindow>(buildMenuSectionResourceCollectorsComponents);
	Events createBuildWindowSectionResourceCollectorsEvent = clickSoundEvent;
	createBuildWindowSectionResourceCollectorsEvent.add(std::make_shared<CreateEEvent>(buildWindowSectionResourceCollectors));



	std::vector<HorizontalSelectionWindowComponent> buildMenuSectionWarehousesComponents;
    buildMenuSectionWarehousesComponents.emplace_back("hammer_icon", *Texts::get()->get("leave"), true, clickSoundEvent);
	createBuildingModeEvent = clickSoundEvent;
	createBuildingModeEvent.add(std::make_shared<TryToBuildEvent>(std::make_shared<WarehouseFood>(0, 0, 0, this->map->getUnits())));
	buildMenuSectionWarehousesComponents.emplace_back(WarehouseFood().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<WarehouseFood>()), true, createBuildingModeEvent);
	createBuildingModeEvent = clickSoundEvent;
	createBuildingModeEvent.add(std::make_shared<TryToBuildEvent>(std::make_shared<WarehouseWood>(0, 0, 0, this->map->getUnits())));
	buildMenuSectionWarehousesComponents.emplace_back(WarehouseWood().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<WarehouseWood>()), true, createBuildingModeEvent);
	createBuildingModeEvent = clickSoundEvent;
	createBuildingModeEvent.add(std::make_shared<TryToBuildEvent>(std::make_shared<WarehouseStone>(0, 0, 0, this->map->getUnits())));
	buildMenuSectionWarehousesComponents.emplace_back(WarehouseStone().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<WarehouseStone>()), true, createBuildingModeEvent);
	createBuildingModeEvent = clickSoundEvent;
	createBuildingModeEvent.add(std::make_shared<TryToBuildEvent>(std::make_shared<WarehouseIron>(0, 0, 0, this->map->getUnits())));
	buildMenuSectionWarehousesComponents.emplace_back(WarehouseIron().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<WarehouseIron>()), true, createBuildingModeEvent);
	createBuildingModeEvent = clickSoundEvent;
	createBuildingModeEvent.add(std::make_shared<TryToBuildEvent>(std::make_shared<WarehouseGold>(0, 0, 0, this->map->getUnits(), this->map->getTreasures())));
	buildMenuSectionWarehousesComponents.emplace_back(WarehouseGold().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<WarehouseGold>()), true, createBuildingModeEvent);
	
	std::shared_ptr<HorizontalSelectionWindow> buildWindowSectionWarehouses = std::make_shared<HorizontalSelectionWindow>(buildMenuSectionWarehousesComponents);
	Events createBuildWindowSectionWarehousesEvent = clickSoundEvent;
	createBuildWindowSectionWarehousesEvent.add(std::make_shared<CreateEEvent>(buildWindowSectionWarehouses));



	std::vector<HorizontalSelectionWindowComponent> buildMenuSectionTroopsComponents;
    buildMenuSectionTroopsComponents.emplace_back("hammer_icon", *Texts::get()->get("leave"), true, clickSoundEvent);
    createBuildingModeEvent = clickSoundEvent;
    createBuildingModeEvent.add((std::make_shared<TryToBuildEvent>(std::make_shared<Barracks>(0, 0, 0, this->map->getUnits(), this->map->getGO(), this->map->getW(), this->map->getH()))));
    buildMenuSectionTroopsComponents.emplace_back(Barracks().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<Barracks>()), true, createBuildingModeEvent);

    std::shared_ptr<HorizontalSelectionWindow> buildWindowSectionTroops = std::make_shared<HorizontalSelectionWindow>( buildMenuSectionTroopsComponents);
    Events createBuildWindowSectionTroopsEvent = clickSoundEvent;
    createBuildWindowSectionTroopsEvent.add(std::make_shared<CreateEEvent>(buildWindowSectionTroops));



    std::vector<HorizontalSelectionWindowComponent> buildMenuSectionDefenceComponents;
    buildMenuSectionDefenceComponents.emplace_back("hammer_icon", *Texts::get()->get("leave"), true, clickSoundEvent);
    createBuildingModeEvent = clickSoundEvent;
    createBuildingModeEvent.add((std::make_shared<TryToBuildEvent>(std::make_shared<Wall1>(0, 0, 0, this->map->getUnits()))));
    buildMenuSectionDefenceComponents.emplace_back(Wall1().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<Wall1>()), true, createBuildingModeEvent);
    createBuildingModeEvent = clickSoundEvent;
    createBuildingModeEvent.add((std::make_shared<TryToBuildEvent>(std::make_shared<Gates1>(0, 0, 0, this->map->getUnits(), this->map->getGO(), this->map->getW(), this->map->getH(), this->map->getTobs()))));
    buildMenuSectionDefenceComponents.emplace_back(Gates1().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<Gates1>()), true, createBuildingModeEvent);
    createBuildingModeEvent = clickSoundEvent;
    createBuildingModeEvent.add((std::make_shared<TryToBuildEvent>(std::make_shared<Wall2>(0, 0, 0, this->map->getUnits()))));
    buildMenuSectionDefenceComponents.emplace_back(Wall2().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<Wall2>()), true, createBuildingModeEvent);
    createBuildingModeEvent = clickSoundEvent;
    createBuildingModeEvent.add((std::make_shared<TryToBuildEvent>(std::make_shared<Gates2>(0, 0, 0, this->map->getUnits(), this->map->getGO(), this->map->getW(), this->map->getH(), this->map->getTobs()))));
    buildMenuSectionDefenceComponents.emplace_back(Gates2().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<Gates2>()), true, createBuildingModeEvent);

    std::shared_ptr<HorizontalSelectionWindow> buildWindowSectionDefence = std::make_shared<HorizontalSelectionWindow>(buildMenuSectionDefenceComponents);
    Events createBuildWindowSectionDefenceEvent = clickSoundEvent;
    createBuildWindowSectionDefenceEvent.add(std::make_shared<CreateEEvent>(buildWindowSectionDefence));



    std::vector<HorizontalSelectionWindowComponent> buildMenuSectionOtherComponents;
    buildMenuSectionOtherComponents.emplace_back("hammer_icon", *Texts::get()->get("leave"), true, Events());
    createBuildingModeEvent = clickSoundEvent;
    createBuildingModeEvent.add((std::make_shared<TryToBuildEvent>(std::make_shared<Market>(0, 0, 0, this->map->getUnits()))));
    buildMenuSectionOtherComponents.emplace_back(Market().getTextureName(), GET_BUILD_DESCRIPTION(std::make_unique<Market>()), true, createBuildingModeEvent);

    std::shared_ptr<HorizontalSelectionWindow> buildWindowSectionOther = std::make_shared<HorizontalSelectionWindow>(buildMenuSectionOtherComponents);
    Events createBuildWindowSectionOtherEvent = clickSoundEvent;
    createBuildWindowSectionOtherEvent.add(std::make_shared<CreateEEvent>(buildWindowSectionOther));



    std::vector<HorizontalSelectionWindowComponent> buildMenuComponents;
    buildMenuComponents.emplace_back("hammer_icon", *Texts::get()->get("leave"), true, clickSoundEvent);
    buildMenuComponents.emplace_back("hammer_icon", *Texts::get()->get("main"), true, createBuildWindowSectionMainEvent);
    buildMenuComponents.emplace_back("hammer_icon", *Texts::get()->get("resource_collectors"), true, createBuildWindowSectionResourceCollectorsEvent);
    buildMenuComponents.emplace_back("hammer_icon", *Texts::get()->get("warehouses"), true, createBuildWindowSectionWarehousesEvent);
    buildMenuComponents.emplace_back("hammer_icon", *Texts::get()->get("troops"), true, createBuildWindowSectionTroopsEvent);
    buildMenuComponents.emplace_back("hammer_icon", *Texts::get()->get("defence"), true, createBuildWindowSectionDefenceEvent);
    buildMenuComponents.emplace_back("hammer_icon", *Texts::get()->get("other"), true, createBuildWindowSectionOtherEvent);

    std::shared_ptr<HorizontalSelectionWindow> buildWindow = std::make_shared<HorizontalSelectionWindow>(buildMenuComponents);
    Events buildEvent = clickSoundEvent;
    buildEvent.add(std::make_shared<CreateEEvent>(buildWindow));



	this->windowW = window.getSize().x;
	this->windowH = window.getSize().y;
    this->returnToMenu = false;
	this->curcorVisibility = true;
    this->element = nullptr;
    this->animation = std::nullopt;
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
		window.draw(*this->selected->getSelectablePointer(this->getMousePositionBasedOnView()));
	}
	if (this->element != nullptr) {
		if (!this->element->isCameraDependent()) {
			window.setView(window.getDefaultView());
		}
		window.draw(*this->element);
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










void MainScreen::removeFinishedElement() {
    if (this->element != nullptr and this->element->finished()) {
        this->element->restart();
        this->element = nullptr;
    }
}
void MainScreen::addNewMoveEvent() {
	while (this->currentGOIndexNewMoveEvent != this->totalGONewMoveEvents) {
		if (this->element != nullptr or !this->baseEvents.empty()) {
			break;
		}
		Events newMoveEvent = this->map->getGO()->at(this->currentGOIndexNewMoveEvent)->newMove(this->getCurrentPlayer()->getId());
		this->addEvents(newMoveEvent);
		this->currentGOIndexNewMoveEvent = this->currentGOIndexNewMoveEvent + 1;
	}
}
bool MainScreen::allNewMoveEventsAdded() const {
	return (this->currentGOIndexNewMoveEvent == this->totalGONewMoveEvents);
}
void MainScreen::changeMove() {
	this->move = this->move + 1;
	this->currentGOIndexNewMoveEvent = 0;
	this->totalGONewMoveEvents = this->map->getGO()->size();
	do {
		this->currentPlayerIndex = (this->currentPlayerIndex + 1) % this->map->getPlayersNumber();
	}
	while (!this->playerIsActive.at(this->currentPlayerIndex));
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
void MainScreen::addButtonClickEventToQueue() {
	for (uint32_t i = 0; i < this->buttons.size(); i = i + 1) {
		Events buttonClickEvent = this->buttons.at(i).click();
		if (!buttonClickEvent.empty()) {
			this->addEvents(buttonClickEvent);
			break;
		}
	}
}
void MainScreen::addGameObjectClickEventToQueue() {
	uint32_t mouseX, mouseY;
	std::tie(mouseX, mouseY) = this->getMousePositionBasedOnView();

	for (int8_t c = 1; c >= 0; c = c - 1) {
		for (uint32_t i = 0; i < this->map->getGO()->size(); i = i + 1) {
			GO* go = this->map->getGO()->at(i);
			if (go->highClickPriority() == c) {
				Events gor = go->click(this->getCurrentPlayer()->getId(), mouseX, mouseY);
				if (!gor.empty()) {
					this->addEvents(gor);
					return;
				}
			}
		}
	}
}
void MainScreen::processBaseEvents() {
    while (!this->baseEvents.empty()) {
        if (this->element != nullptr or this->animation.has_value() or !this->viewMovingQueue.empty()) {
            break;
        }
        this->handleBaseEvent(this->baseEvents.front());
        this->baseEvents.pop();
    }
}
void MainScreen::addEvents(Events &e) {
	for (uint32_t i = 0; i < e.size(); i = i + 1) {
		if (std::shared_ptr<CloseAnimationEvent> closeAnimationEvent = std::dynamic_pointer_cast<CloseAnimationEvent>(e.at(i))) { // handling close animation event out of queue cuz active animation stops processing events in queue
			this->handleCloseAnimationEvent(closeAnimationEvent);
		}
		else {
			this->baseEvents.push(e.at(i));
		}
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
	if (this->viewMovingQueue.empty()) {
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
	else {
		uint32_t currentX, currentY;
		std::tie(currentX, currentY) = this->viewMovingQueue.front();

		uint32_t viewX = this->view->getCenter().x;
		uint32_t viewY = this->view->getCenter().y;

		bool horizontalOk = false;
		bool verticalOk = false;

		if (currentX < viewX) {
			horizontalOk = horizontalOk or !this->moveViewToWest(currentX);
		}
		else if (currentX > viewX) {
			horizontalOk = horizontalOk or !this->moveViewToEast(currentX);
		}
		else {
			horizontalOk = true;
		}

		if (currentY < viewY) {
			verticalOk = verticalOk or !this->moveViewToNorth(currentY);
		}
		else if (currentY > viewY) {
			verticalOk = verticalOk or !this->moveViewToSouth(currentY);
		}
		else {
			verticalOk = true;
		}

		if (horizontalOk and verticalOk) {
			this->viewMovingQueue.pop();
		}
	}
}








static float VIEW_MOVING_DELTA = 10;
static float VIEW_MOVING_BIG_DELTA = 1.25f * VIEW_MOVING_DELTA;


bool MainScreen::moveViewToNorth(uint32_t border) {
	this->view->setCenter(this->view->getCenter() - sf::Vector2f(0, VIEW_MOVING_BIG_DELTA));
	return this->verifyViewNorth() and this->verifyViewNorth(border);
}
bool MainScreen::moveViewToNorth() {
	this->view->setCenter(this->view->getCenter() - sf::Vector2f(0, VIEW_MOVING_DELTA));
	return this->verifyViewNorth();
}
bool MainScreen::moveViewToSouth(uint32_t border) {
	this->view->setCenter(this->view->getCenter() + sf::Vector2f(0, VIEW_MOVING_BIG_DELTA));
	return this->verifyViewSouth() and this->verifyViewSouth(border);
}
bool MainScreen::moveViewToSouth() {
	this->view->setCenter(this->view->getCenter() + sf::Vector2f(0, VIEW_MOVING_DELTA));
	return this->verifyViewSouth();
}
bool MainScreen::moveViewToWest(uint32_t border) {
	this->view->setCenter(this->view->getCenter() - sf::Vector2f(VIEW_MOVING_BIG_DELTA, 0));
	return this->verifyViewWest() and this->verifyViewWest(border);
}
bool MainScreen::moveViewToWest() {
	this->view->setCenter(this->view->getCenter() - sf::Vector2f(VIEW_MOVING_DELTA, 0));
	return this->verifyViewWest();
}
bool MainScreen::moveViewToEast(uint32_t border) {
	this->view->setCenter(this->view->getCenter() + sf::Vector2f(VIEW_MOVING_BIG_DELTA, 0));
	return this->verifyViewEast() and this->verifyViewEast(border);
}
bool MainScreen::moveViewToEast() {
	this->view->setCenter(this->view->getCenter() + sf::Vector2f(VIEW_MOVING_DELTA, 0));
	return this->verifyViewEast();
}
bool MainScreen::verifyViewNorth(uint32_t border) {
	if (this->view->getCenter().y < border) {
		this->view->setCenter(sf::Vector2f(this->view->getCenter().x, border));
		return false;
	}
	return true;
}
bool MainScreen::verifyViewNorth() {
	return this->verifyViewNorth(this->windowH / 2);
}
bool MainScreen::verifyViewSouth(uint32_t border) {
	if (this->view->getCenter().y > border) {
		this->view->setCenter(sf::Vector2f(this->view->getCenter().x, border));
		return false;
	}
	return true;
}
bool MainScreen::verifyViewSouth() {
	return this->verifyViewSouth(64 * this->plains.getH() - this->windowH / 2);
}
bool MainScreen::verifyViewWest(uint32_t border) {
	if (this->view->getCenter().x < border) {
		this->view->setCenter(sf::Vector2f(border, this->view->getCenter().y));
		return false;
	}
	return true;
}
bool MainScreen::verifyViewWest() {
	return this->verifyViewWest(this->windowW / 2);
}
bool MainScreen::verifyViewEast(uint32_t border) {
	if (this->view->getCenter().x > border) {
		this->view->setCenter(sf::Vector2f(border, this->view->getCenter().y));
		return false;
	}
	return true;
}
bool MainScreen::verifyViewEast() {
	return this->verifyViewEast(64 * this->plains.getW() - this->windowW / 2);
}










void MainScreen::handleBaseEvent(std::shared_ptr<Event> e) {
    if (std::shared_ptr<TryToTradeEvent> tryToTradeEvent = std::dynamic_pointer_cast<TryToTradeEvent>(e)) {
        this->handleTryToTradeEvent(tryToTradeEvent);
    }
    else if (std::shared_ptr<AddResourceEvent> addResourceEvent = std::dynamic_pointer_cast<AddResourceEvent>(e)) {
        this->handleAddResourceEvent(addResourceEvent);
    }
    else if (std::shared_ptr<SubResourceEvent> subResourceEvent = std::dynamic_pointer_cast<SubResourceEvent>(e)) {
        this->handleSubResourceEvent(subResourceEvent);
    }
    else if (std::shared_ptr<AddResourcesEvent> addResourcesEvent = std::dynamic_pointer_cast<AddResourcesEvent>(e)) {
        this->handleAddResourcesEvent(addResourcesEvent);
    }
    else if (std::shared_ptr<SubResourcesEvent> subResourcesEvent = std::dynamic_pointer_cast<SubResourcesEvent>(e)) {
        this->handleSubResourcesEvent(subResourcesEvent);
    }
    else if (std::shared_ptr<ChangeHighlightEvent> changeHighlightEvent = std::dynamic_pointer_cast<ChangeHighlightEvent>(e)) {
        this->handleChangeHighlightEvent(changeHighlightEvent);
    }
    else if (std::shared_ptr<CollectEvent> collectEvent = std::dynamic_pointer_cast<CollectEvent>(e)) {
        this->handleCollectEvent(collectEvent);
    }
    else if (std::shared_ptr<AddHpEvent> addHpEvent = std::dynamic_pointer_cast<AddHpEvent>(e)) {
        this->handleAddHpEvent(addHpEvent);
    }
    else if (std::shared_ptr<DecreaseCurrentTradeMovesLeftEvent> decreaseCurrentTradeMovesLeftEvent = std::dynamic_pointer_cast<DecreaseCurrentTradeMovesLeftEvent>(e)) {
        this->handleDecreaseCurrentTradeMovesLeft(decreaseCurrentTradeMovesLeftEvent);
    }
    else if (std::shared_ptr<TryToBuildEvent> tryToBuildEvent = std::dynamic_pointer_cast<TryToBuildEvent>(e)) {
        this->handleTryToBuild(tryToBuildEvent);
    }
    else if (std::shared_ptr<BuildEvent> buildEvent = std::dynamic_pointer_cast<BuildEvent>(e)) {
        this->handleBuild(buildEvent);
    }
    else if (std::shared_ptr<PlaySoundEvent> playSoundEvent = std::dynamic_pointer_cast<PlaySoundEvent>(e)) {
        this->handlePlaySoundEvent(playSoundEvent);
    }
    else if (std::shared_ptr<CreateEEvent> createEEvent = std::dynamic_pointer_cast<CreateEEvent>(e)) {
        this->handleCreatePopUpElementEvent(createEEvent);
    }
    else if (std::shared_ptr<ChangeMoveEvent> changeMoveEvent = std::dynamic_pointer_cast<ChangeMoveEvent>(e)) {
        this->handleChangeMoveEvent(changeMoveEvent);
    }
    else if (std::shared_ptr<ReturnToMenuEvent> returnToMenuEvent = std::dynamic_pointer_cast<ReturnToMenuEvent>(e)) {
        this->handleReturnToMenuEvent(returnToMenuEvent);
    }
    else if (std::shared_ptr<DestroyEvent> destroyEvent = std::dynamic_pointer_cast<DestroyEvent>(e)) {
        this->handleDestroyEvent(destroyEvent);
    }
    else if (std::shared_ptr<ResourceStorageBDestroyedEvent> resourceStorageBDestroyedEvent = std::dynamic_pointer_cast<ResourceStorageBDestroyedEvent>(e)) {
        this->handleResourceStorageBDestroyedEvent(resourceStorageBDestroyedEvent);
    }
    else if (std::shared_ptr<VictoryConditionBDestroyedEvent> victoryConditionBDestroyedEvent = std::dynamic_pointer_cast<VictoryConditionBDestroyedEvent>(e)) {
        this->handleVictoryConditionBDestroyedEvent(victoryConditionBDestroyedEvent);
    }
    else if (std::shared_ptr<DecreaseCurrentProducingMovesLeftEvent> decreaseCurrentProducingMovesLeftEvent = std::dynamic_pointer_cast<DecreaseCurrentProducingMovesLeftEvent>(e)) {
        this->handleDecreaseCurrentProdusingMovesLeftEvent(decreaseCurrentProducingMovesLeftEvent);
    }
    else if (std::shared_ptr<TryToProduceEvent> tryToProduceEvent = std::dynamic_pointer_cast<TryToProduceEvent>(e)) {
        this->handleTryToProduceEvent(tryToProduceEvent);
    }
    else if (std::shared_ptr<WarriorProducingFinishedEvent> warriorProducingFinishedEvent = std::dynamic_pointer_cast<WarriorProducingFinishedEvent>(e)) {
        this->handleWarriorProducingFinishedEvent(warriorProducingFinishedEvent);
    }
    else if (std::shared_ptr<SelectEvent> selectEvent = std::dynamic_pointer_cast<SelectEvent>(e)) {
        this->handleSelectEvent(selectEvent);
    }
    else if (std::shared_ptr<UnselectEvent> unselectEvent = std::dynamic_pointer_cast<UnselectEvent>(e)) {
        this->handleUnselectEvent(unselectEvent);
    }
    else if (std::shared_ptr<StartWarriorAnimationEvent> startWarriorAnimationEvent = std::dynamic_pointer_cast<StartWarriorAnimationEvent>(e)) {
        this->handleStartWarriorAnimationEvent(startWarriorAnimationEvent);
    }
    else if (std::shared_ptr<TryToCollectEvent> tryToCollectEvent = std::dynamic_pointer_cast<TryToCollectEvent>(e)) {
        this->handleTryToCollectEvent(tryToCollectEvent);
    }
    else if (std::shared_ptr<RefreshMovementPointsEvent> refreshMovementPointsEvent = std::dynamic_pointer_cast<RefreshMovementPointsEvent>(e)) {
        this->handleRefreshMovementPointsEvent(refreshMovementPointsEvent);
    }
    else if (std::shared_ptr<EnableCursorEvent> enableCursorEvent = std::dynamic_pointer_cast<EnableCursorEvent>(e)) {
        this->handleEnableCursorEvent(enableCursorEvent);
    }
    else if (std::shared_ptr<DisableCursorEvent> disableCursorEvent = std::dynamic_pointer_cast<DisableCursorEvent>(e)) {
        this->handleDisableCursorEvent(disableCursorEvent);
    }
    else if (std::shared_ptr<CreateAnimationEvent> createAnimationEvent = std::dynamic_pointer_cast<CreateAnimationEvent>(e)) {
        this->handleCreateAnimationEvent(createAnimationEvent);
    }
	else if (std::shared_ptr<DecreaseBurningMovesLeftEvent> decreaseBurningMovesLeftEvent = std::dynamic_pointer_cast<DecreaseBurningMovesLeftEvent>(e)) {
		this->handleDecreaseBurningMovesLeftEvent(decreaseBurningMovesLeftEvent);
	}
	else if (std::shared_ptr<SubHpEvent> subHpEvent = std::dynamic_pointer_cast<SubHpEvent>(e)) {
		this->handleSubHpEvent(subHpEvent);
	}
	else if (std::shared_ptr<SetFireEvent> setFireEvent = std::dynamic_pointer_cast<SetFireEvent>(e)) {
		this->handleSetFireEvent(setFireEvent);
	}
	else if (std::shared_ptr<ChangeWarriorDirectionEvent> changeWarriorDirectionEvent = std::dynamic_pointer_cast<ChangeWarriorDirectionEvent>(e)) {
		this->handleChangeWarriorDirectionEvent(changeWarriorDirectionEvent);
	}
	else if (std::shared_ptr<TryToRaiseDragonEvent> tryToRaiseDragonEvent = std::dynamic_pointer_cast<TryToRaiseDragonEvent>(e)) {
		this->handleTryToRaiseDragonEvent(tryToRaiseDragonEvent);
	}
	else if (std::shared_ptr<DecreaseDragonRecoverMovesLeftEvent> decreaseDragonRecoverMovesLeftEvent = std::dynamic_pointer_cast<DecreaseDragonRecoverMovesLeftEvent>(e)) {
		this->handleDecreaseDragonRecoverMovesLeftEvent(decreaseDragonRecoverMovesLeftEvent);
	}
	else if (std::shared_ptr<ResetDragonRecoverMovesLeftEvent> resetDragonRecoverMovesLeftEvent = std::dynamic_pointer_cast<ResetDragonRecoverMovesLeftEvent>(e)) {
		this->handleResetDragonRecoverMovesLeftEvent(resetDragonRecoverMovesLeftEvent);
	}
	else if (std::shared_ptr<FocusOnEvent> focusOnEvent = std::dynamic_pointer_cast<FocusOnEvent>(e)) {
		this->handleFocusOnEvent(focusOnEvent);
	}
}
void MainScreen::handleTryToTradeEvent(std::shared_ptr<TryToTradeEvent> e) {
	TradingB* b = e->getBuilding();
	Trade t = e->getTrade();
	if (this->getCurrentPlayer()->getResource(t.sell.type) >= t.sell.n) {
		Events tradeEvent = b->doTrade(t);
		this->addEvents(tradeEvent);
	}
	else {
        Events clickEvent;
        clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

        Events unableToTradeEvent = clickEvent;
		std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(*Texts::get()->get("no_resources_for_trade"), *Texts::get()->get("OK"), clickEvent);
        unableToTradeEvent.add(std::make_shared<CreateEEvent>(w));
        this->addEvents(unableToTradeEvent);
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
		this->bm = BuildingMode(e->getBuilding(), this->map->getGO(), this->map->getTbs(), this->getCurrentPlayer()->getId(), this->map->getW(), this->map->getH());
		Events bmStartEvent = bm.start();
		this->addEvents(bmStartEvent);
	}
	else {
        Events clickEvent;
        clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

		std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(*Texts::get()->get("no_resources_for_building"), *Texts::get()->get("OK"), clickEvent);
        Events unableToBuildEvent;
        unableToBuildEvent.add(std::make_shared<CreateEEvent>(w));
		this->addEvents(unableToBuildEvent);
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
    this->element = e->getElement();
    this->element->run(this->windowW, this->windowH);
}
void MainScreen::handleChangeMoveEvent(std::shared_ptr<ChangeMoveEvent> e) {
	this->changeMove();
}
void MainScreen::handleReturnToMenuEvent(std::shared_ptr<ReturnToMenuEvent> e) {
	this->returnToMenu = true;
}
void MainScreen::handleDestroyEvent(std::shared_ptr<DestroyEvent> e) {
	Events destroyBuildingEvent = e->getBuilding()->destroy();
	this->addEvents(destroyBuildingEvent);
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
		Events returnToMenuEvent;
        returnToMenuEvent.add(std::make_shared<PlaySoundEvent>("click"));
		returnToMenuEvent.add(std::make_shared<ReturnToMenuEvent>());
		w = std::make_shared<WindowButton>(*Texts::get()->get("game_finished"), *Texts::get()->get("OK"), returnToMenuEvent);
	}
	else {
        Events clickEvent;
        clickEvent.add(std::make_shared<PlaySoundEvent>("click"));
		w = std::make_shared<WindowButton>(*Texts::get()->get("player_is_out"), *Texts::get()->get("OK"), clickEvent);
	}
	std::shared_ptr<CreateEEvent> createW = std::make_shared<CreateEEvent>(w);
	this->handleCreatePopUpElementEvent(createW);
}
void MainScreen::handleDecreaseCurrentProdusingMovesLeftEvent(std::shared_ptr<DecreaseCurrentProducingMovesLeftEvent> e) {
	e->getBuilding()->decreaseCurrentProducingMovesLeft();
}
void MainScreen::handleTryToProduceEvent(std::shared_ptr<TryToProduceEvent> e) {
	if (this->getCurrentPlayer()->getResources() >= e->getWarrior()->getCost()) {
		Events subResourcesEvent;
		subResourcesEvent.add(std::make_shared<SubResourcesEvent>(e->getWarrior()->getCost()));
        subResourcesEvent = subResourcesEvent + e->getProducer()->startProducing(e->getWarrior());
		this->addEvents(subResourcesEvent);
	}
	else {
        Events clickEvent;
        clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

		std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(*Texts::get()->get("no_resources_for_producing"), *Texts::get()->get("OK"), clickEvent);
        Events unableToProduceEvent = clickEvent;
        unableToProduceEvent.add(std::make_shared<CreateEEvent>(w));
		this->addEvents(unableToProduceEvent);
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
void MainScreen::handleStartWarriorAnimationEvent(std::shared_ptr<StartWarriorAnimationEvent> e) {
	e->getWarrior()->startAnimation(e->getAnimation());
}
void MainScreen::handleTryToCollectEvent(std::shared_ptr<TryToCollectEvent> e) {
	uint32_t id = e->getPlayerId();
	ResourcePoint* rp = e->getResourcePoint();
	uint32_t value = e->getValue();

	uint32_t limit = this->getResourcesLimit().get(rp->getResourceType()) - this->getCurrentPlayer()->getResource(rp->getResourceType());
	if (value > limit) {
		value = limit;
	}

	Events tryToCollectEvent = rp->tryToCollect(id, value);
	this->addEvents(tryToCollectEvent);
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
void MainScreen::handleCreateAnimationEvent(std::shared_ptr<CreateAnimationEvent> e) {
    this->animation = e->getAnimation();
}
void MainScreen::handleCloseAnimationEvent(std::shared_ptr<CloseAnimationEvent> e) {
    this->animation = std::nullopt;
}
void MainScreen::handleDecreaseBurningMovesLeftEvent(std::shared_ptr<DecreaseBurningMovesLeftEvent> e) {
	e->getBuilding()->decreaseBurningMovesLeft();
}
void MainScreen::handleSubHpEvent(std::shared_ptr<SubHpEvent> e) {
	e->getHPGO()->subHp(e->getValue());
}
void MainScreen::handleSetFireEvent(std::shared_ptr<SetFireEvent> e) {
	e->getBuilding()->setFire();
}
void MainScreen::handleChangeWarriorDirectionEvent(std::shared_ptr<ChangeWarriorDirectionEvent> e) {
	e->getWarrior()->changeDirection(e->getDirection());
}
void MainScreen::handleTryToRaiseDragonEvent(std::shared_ptr<TryToRaiseDragonEvent> e) {
	if (this->getCurrentPlayer()->getResources() >= e->getDragon()->getCost()) {
		Events subResourcesEvent;
		subResourcesEvent.add(std::make_shared<SubResourcesEvent>(e->getDragon()->getCost()));
		subResourcesEvent = subResourcesEvent + e->getCave()->setDragon(e->getDragon()->clone());
		this->addEvents(subResourcesEvent);
	}
	else {
		Events clickEvent;
		clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

		std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(*Texts::get()->get("no_resource_for_dragon"), *Texts::get()->get("OK"), clickEvent);
		Events unableToRaiseDragonEvent = clickEvent;
		unableToRaiseDragonEvent.add(std::make_shared<CreateEEvent>(w));
		this->addEvents(unableToRaiseDragonEvent);
	}
}
void MainScreen::handleDecreaseDragonRecoverMovesLeftEvent(std::shared_ptr<DecreaseDragonRecoverMovesLeftEvent> e) {
	e->getDragon()->decreaseRecoverMovesLeft();
}
void MainScreen::handleResetDragonRecoverMovesLeftEvent(std::shared_ptr<ResetDragonRecoverMovesLeftEvent> e) {
	e->getDragon()->resetRecoverMoves();
}
void MainScreen::handleFocusOnEvent(std::shared_ptr<FocusOnEvent> e) {
	this->viewMovingQueue.emplace(
		64 * e->getX() + 64 * e->getSX() / 2,
		64 * e->getY() + 64 * e->getSY() / 2);
}