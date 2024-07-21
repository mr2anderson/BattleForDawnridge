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
#include "TradingSpec.hpp"
#include "ResourcePoint.hpp"
#include "WarriorProducerSpec.hpp"
#include "Warrior.hpp"
#include "House.hpp"
#include "Resin.hpp"
#include "Spell.hpp"
#include "SpellProducerSpec.hpp"
#include "SpellFactory.hpp"
#include "Effect.hpp"
#include "Infirmary.hpp"







MainScreen* MainScreen::singletone = nullptr;






bool MainScreen::run(std::shared_ptr<Map> mapPtr, sf::RenderWindow& window) {
    this->init(mapPtr, window);
	sf::Event event{};
	for (; ;) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::MouseButtonPressed) {
                if (!this->animation.has_value() and this->viewMovingQueue.empty()) {
                    if (this->element == nullptr) {
                        if (event.mouseButton.button == sf::Mouse::Button::Left or event.mouseButton.button == sf::Mouse::Button::Right) {
                            if (this->events.empty() and this->allNewMoveEventsAdded()) {
                                if (this->selected == nullptr) {
                                    if (event.mouseButton.button == sf::Mouse::Button::Left) {
                                        this->addButtonClickEventToQueue();
                                    }
                                    if (this->events.empty()) {
                                        this->addGameObjectClickEventToQueue(event.mouseButton.button);
                                    }
                                }
                                else {
                                    std::tuple<uint32_t, uint32_t> pos = this->getMousePositionBasedOnView();
                                    Events unselectEvent = this->selected->unselect(this->map->getStatePtr(), std::get<0>(pos) / 64, std::get<1>(pos) / 64, event.mouseButton.button);
                                    this->addEvents(unselectEvent);
                                }
                            }
                        }
                    }
                    else  {
                        if (event.mouseButton.button == sf::Mouse::Button::Left) {
                            Events elementClickEvent = this->element->click();
                            this->addEvents(elementClickEvent);
                        }
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
    this->initGraphics(window);
	this->changeMove();
}
void MainScreen::initMap(std::shared_ptr<Map> mapPtr) {
    this->map = mapPtr;
}
void MainScreen::initPlayerIsActiveTable() {
	this->playerIsActive.resize(this->map->getStatePtr()->getPlayersPtr()->total(), true);
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
template<typename T> std::wstring GET_BUILD_DESCRIPTION() {
	std::unique_ptr<T> t = std::make_unique<T>();
	std::wstring description = t->getDescription() + L'\n' +
		*Texts::get()->get("cost") + t->getCost().getReadableInfo();
	return description;
}
template<typename T> HorizontalSelectionWindowComponent GET_BUILD_COMPONENT() {
	Events clickSoundEvent;
	clickSoundEvent.add(std::make_shared<PlaySoundEvent>("click"));

	Events createBuildingModeEvent = clickSoundEvent;
	createBuildingModeEvent.add(std::make_shared<TryToBuildEvent>(std::make_shared<T>(0, 0, 0)));
	HorizontalSelectionWindowComponent component = { T().getTextureName(), GET_BUILD_DESCRIPTION<T>(), true, createBuildingModeEvent };

	return component;
}
void MainScreen::initGraphics(sf::RenderWindow &window) {
	window.setMouseCursorVisible(true);



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




	std::vector<HorizontalSelectionWindowComponent> buildMenuSectionMainComponents;
    buildMenuSectionMainComponents.emplace_back("hammer_icon", *Texts::get()->get("leave"), true, clickSoundEvent);
	buildMenuSectionMainComponents.emplace_back(GET_BUILD_COMPONENT<Road>());
	buildMenuSectionMainComponents.emplace_back(GET_BUILD_COMPONENT<House>());

	std::shared_ptr<HorizontalSelectionWindow> buildWindowSectionMain = std::make_shared<HorizontalSelectionWindow>(buildMenuSectionMainComponents);
	Events createBuildWindowSectionMainEvent = clickSoundEvent;
	createBuildWindowSectionMainEvent.add(std::make_shared<CreateEEvent>(buildWindowSectionMain));



	std::vector<HorizontalSelectionWindowComponent> buildMenuSectionResourceCollectorsComponents;
    buildMenuSectionResourceCollectorsComponents.emplace_back("hammer_icon", *Texts::get()->get("leave"), true, clickSoundEvent);
	buildMenuSectionResourceCollectorsComponents.emplace_back(GET_BUILD_COMPONENT<Arable>());
	buildMenuSectionResourceCollectorsComponents.emplace_back(GET_BUILD_COMPONENT<Sawmill>());
	buildMenuSectionResourceCollectorsComponents.emplace_back(GET_BUILD_COMPONENT<Quarry>());
	buildMenuSectionResourceCollectorsComponents.emplace_back(GET_BUILD_COMPONENT<Mine>());

	std::shared_ptr<HorizontalSelectionWindow> buildWindowSectionResourceCollectors = std::make_shared<HorizontalSelectionWindow>(buildMenuSectionResourceCollectorsComponents);
	Events createBuildWindowSectionResourceCollectorsEvent = clickSoundEvent;
	createBuildWindowSectionResourceCollectorsEvent.add(std::make_shared<CreateEEvent>(buildWindowSectionResourceCollectors));



	std::vector<HorizontalSelectionWindowComponent> buildMenuSectionWarehousesComponents;
    buildMenuSectionWarehousesComponents.emplace_back("hammer_icon", *Texts::get()->get("leave"), true, clickSoundEvent);
	buildMenuSectionWarehousesComponents.emplace_back(GET_BUILD_COMPONENT<WarehouseFood>());
	buildMenuSectionWarehousesComponents.emplace_back(GET_BUILD_COMPONENT<WarehouseWood>());
	buildMenuSectionWarehousesComponents.emplace_back(GET_BUILD_COMPONENT<WarehouseStone>());
	buildMenuSectionWarehousesComponents.emplace_back(GET_BUILD_COMPONENT<WarehouseIron>());
	buildMenuSectionWarehousesComponents.emplace_back(GET_BUILD_COMPONENT<WarehouseGold>());
	
	std::shared_ptr<HorizontalSelectionWindow> buildWindowSectionWarehouses = std::make_shared<HorizontalSelectionWindow>(buildMenuSectionWarehousesComponents);
	Events createBuildWindowSectionWarehousesEvent = clickSoundEvent;
	createBuildWindowSectionWarehousesEvent.add(std::make_shared<CreateEEvent>(buildWindowSectionWarehouses));



	std::vector<HorizontalSelectionWindowComponent> buildMenuSectionTroopsComponents;
    buildMenuSectionTroopsComponents.emplace_back("hammer_icon", *Texts::get()->get("leave"), true, clickSoundEvent);
	buildMenuSectionTroopsComponents.emplace_back(GET_BUILD_COMPONENT<Barracks>());
	buildMenuSectionTroopsComponents.emplace_back(GET_BUILD_COMPONENT<Infirmary>());

    std::shared_ptr<HorizontalSelectionWindow> buildWindowSectionTroops = std::make_shared<HorizontalSelectionWindow>( buildMenuSectionTroopsComponents);
    Events createBuildWindowSectionTroopsEvent = clickSoundEvent;
    createBuildWindowSectionTroopsEvent.add(std::make_shared<CreateEEvent>(buildWindowSectionTroops));



    std::vector<HorizontalSelectionWindowComponent> buildMenuSectionDefenceComponents;
    buildMenuSectionDefenceComponents.emplace_back("hammer_icon", *Texts::get()->get("leave"), true, clickSoundEvent);
	buildMenuSectionDefenceComponents.emplace_back(GET_BUILD_COMPONENT<Wall1>());
	buildMenuSectionDefenceComponents.emplace_back(GET_BUILD_COMPONENT<Gates1>());
	buildMenuSectionDefenceComponents.emplace_back(GET_BUILD_COMPONENT<Wall2>());
	buildMenuSectionDefenceComponents.emplace_back(GET_BUILD_COMPONENT<Gates2>());
	buildMenuSectionDefenceComponents.emplace_back(GET_BUILD_COMPONENT<Resin>());

    std::shared_ptr<HorizontalSelectionWindow> buildWindowSectionDefence = std::make_shared<HorizontalSelectionWindow>(buildMenuSectionDefenceComponents);
    Events createBuildWindowSectionDefenceEvent = clickSoundEvent;
    createBuildWindowSectionDefenceEvent.add(std::make_shared<CreateEEvent>(buildWindowSectionDefence));



    std::vector<HorizontalSelectionWindowComponent> buildMenuSectionOtherComponents;
    buildMenuSectionOtherComponents.emplace_back("hammer_icon", *Texts::get()->get("leave"), true, clickSoundEvent);
	buildMenuSectionOtherComponents.emplace_back(GET_BUILD_COMPONENT<Market>());
	buildMenuSectionOtherComponents.emplace_back(GET_BUILD_COMPONENT<SpellFactory>());

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














void MainScreen::drawEverything(sf::RenderWindow& window) {
	window.clear();
	window.setView(*this->view);
	this->drawCells(window);
	window.draw(*this->map);
	this->drawHighlightion(window);
	if (this->selected != nullptr) {
		window.draw(*this->selected->getSelectablePointer(std::get<0>(this->getMousePositionBasedOnView()), std::get<1>(this->getMousePositionBasedOnView())));
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

	Resources limit;
	for (uint32_t i = 0; i < this->map->getStatePtr()->getCollectionsPtr()->totalBuildings(); i = i + 1) {
		Building* b = this->map->getStatePtr()->getCollectionsPtr()->getBuilding(i);
		if (b->exist() and b->getPlayerId() == this->getCurrentPlayer()->getId()) {
			limit.plus(b->getLimit());
		}
	}
	bar.setLimit(limit);

	uint32_t population = 0;
	for (uint32_t i = 0; i < this->map->getStatePtr()->getCollectionsPtr()->totalWarriors(); i = i + 1) {
		Warrior* w = this->map->getStatePtr()->getCollectionsPtr()->getWarrior(i);
		if (w->exist() and w->getPlayerId() == this->getCurrentPlayer()->getId()) {
			population = population + w->getPopulation();
		}
	}
	bar.setPopulation(population);

	uint32_t populationLimit = 0;
	for (uint32_t i = 0; i < this->map->getStatePtr()->getCollectionsPtr()->totalBuildings(); i = i + 1) {
		Building* b = this->map->getStatePtr()->getCollectionsPtr()->getBuilding(i);
		if (b->exist() and b->getPlayerId() == this->getCurrentPlayer()->getId()) {
			populationLimit = populationLimit + b->getPopulationLimit();
		}
	}
	bar.setPopulationLimit(populationLimit);

	window.draw(bar);
}
void MainScreen::drawCells(sf::RenderWindow& window) {
	for (uint32_t i = 0; i < this->map->getStatePtr()->getMapSizePtr()->getWidth(); i = i + 1) {
		for (uint32_t j = 0; j < this->map->getStatePtr()->getMapSizePtr()->getHeight(); j = j + 1) {
			sf::Sprite s;
			s.setTexture(*Textures::get()->get("plain" + std::to_string(this->plains.getType(i, j))));
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
		if (this->element != nullptr or !this->events.empty()) {
			break;
		}
		Events newMoveEvent = this->map->getStatePtr()->getCollectionsPtr()->getGO(this->currentGOIndexNewMoveEvent)->newMove(this->map->getStatePtr(), this->getCurrentPlayer()->getId());
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
	this->totalGONewMoveEvents = this->map->getStatePtr()->getCollectionsPtr()->totalGOs();
	do {
		this->currentPlayerIndex = (this->currentPlayerIndex + 1) % this->map->getStatePtr()->getPlayersPtr()->total();
	}
	while (!this->playerIsActive.at(this->currentPlayerIndex));
}
Player* MainScreen::getCurrentPlayer() {
	return this->map->getStatePtr()->getPlayersPtr()->getPlayerPtr((this->move + 1) % this->map->getStatePtr()->getPlayersPtr()->total() + 1);
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
void MainScreen::addGameObjectClickEventToQueue(uint8_t button) {
	uint32_t mouseX, mouseY;
	std::tie(mouseX, mouseY) = this->getMousePositionBasedOnView();

	for (uint8_t priority : {GO::PRIORITY::HIGHEST, GO::PRIORITY::HIGH, GO::PRIORITY::DEFAULT, GO::PRIORITY::LOW}) {
		for (uint32_t i = 0; i < this->map->getStatePtr()->getCollectionsPtr()->totalGOs(); i = i + 1) {
			GO* go = this->map->getStatePtr()->getCollectionsPtr()->getGO(i);
			if (go->getClickPriority() == priority) {
				Events gor = go->click(this->map->getStatePtr(), this->getCurrentPlayer()->getId(), button, mouseX, mouseY);
				if (!gor.empty()) {
					this->addEvents(gor);
					return;
				}
			}
		}
	}
}
void MainScreen::processBaseEvents() {
    while (!this->events.empty()) {
        if (this->element != nullptr or this->animation.has_value() or !this->viewMovingQueue.empty()) {
            break;
        }
        this->handleEvent(this->events.front());
        this->events.pop();
    }
}
void MainScreen::addEvents(Events &e) {
	for (uint32_t i = 0; i < e.size(); i = i + 1) {
		std::shared_ptr<Event> event = e.at(i);
		if (event->isUrgent()) {
			this->handleEvent(event);
		}
		else {
			this->events.push(event);
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
	return this->verifyViewSouth(64 * this->map->getStatePtr()->getMapSizePtr()->getHeight() - this->windowH / 2);
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
	return this->verifyViewEast(64 * this->map->getStatePtr()->getMapSizePtr()->getWidth() - this->windowW / 2);
}










void MainScreen::handleEvent(std::shared_ptr<Event> e) {
    if (std::shared_ptr<AddResourceEvent> addResourceEvent = std::dynamic_pointer_cast<AddResourceEvent>(e)) {
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
    else if (std::shared_ptr<SetHighlightEvent> changeHighlightEvent = std::dynamic_pointer_cast<SetHighlightEvent>(e)) {
        this->handleSetHighlightEvent(changeHighlightEvent);
    }
    else if (std::shared_ptr<AddHpEvent> addHpEvent = std::dynamic_pointer_cast<AddHpEvent>(e)) {
        this->handleAddHpEvent(addHpEvent);
    }
    else if (std::shared_ptr<DecreaseCurrentTradeMovesLeftEvent> decreaseCurrentTradeMovesLeftEvent = std::dynamic_pointer_cast<DecreaseCurrentTradeMovesLeftEvent>(e)) {
        this->handleDecreaseCurrentTradeMovesLeft(decreaseCurrentTradeMovesLeftEvent);
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
    else if (std::shared_ptr<VictoryConditionBDestroyedEvent> victoryConditionBDestroyedEvent = std::dynamic_pointer_cast<VictoryConditionBDestroyedEvent>(e)) {
        this->handleVictoryConditionBDestroyedEvent(victoryConditionBDestroyedEvent);
    }
    else if (std::shared_ptr<DecreaseCurrentProducingMovesLeftEvent> decreaseCurrentProducingMovesLeftEvent = std::dynamic_pointer_cast<DecreaseCurrentProducingMovesLeftEvent>(e)) {
        this->handleDecreaseCurrentProdusingMovesLeftEvent(decreaseCurrentProducingMovesLeftEvent);
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
	else if (std::shared_ptr<FocusOnEvent> focusOnEvent = std::dynamic_pointer_cast<FocusOnEvent>(e)) {
		this->handleFocusOnEvent(focusOnEvent);
	}
	else if (std::shared_ptr<ResetHighlightEvent> resetHighlightEvent = std::dynamic_pointer_cast<ResetHighlightEvent>(e)) {
		this->handleResetHighlightEvent(resetHighlightEvent);
	}
	else if (std::shared_ptr<DoTradeEvent> doTradeEvent = std::dynamic_pointer_cast<DoTradeEvent>(e)) {
		this->handleDoTradeEvent(doTradeEvent);
	}
	else if (std::shared_ptr<StartWarriorProducingEvent> startWarriorProducingEvent = std::dynamic_pointer_cast<StartWarriorProducingEvent>(e)) {
		this->handleStartWarriorProducingEvent(startWarriorProducingEvent);
	}
	else if (std::shared_ptr<TryToBuildEvent> tryToBuildEvent = std::dynamic_pointer_cast<TryToBuildEvent>(e)) {
		this->handleTryToBuildEvent(tryToBuildEvent);
	}
	else if (std::shared_ptr<KillNextTurnEvent> killNextTurnEvent = std::dynamic_pointer_cast<KillNextTurnEvent>(e)) {
		this->handleKillNextTurnEvent(killNextTurnEvent);
	}
	else if (std::shared_ptr<RevertKillNextTurnEvent> revertKillNextTurnEvent = std::dynamic_pointer_cast<RevertKillNextTurnEvent>(e)) {
		this->handleRevertKillNextTurnEvent(revertKillNextTurnEvent);
	}
	else if (std::shared_ptr<CloseAnimationEvent> closeAnimationEvent = std::dynamic_pointer_cast<CloseAnimationEvent>(e)) {
		this->handleCloseAnimationEvent(closeAnimationEvent);
	}
	else if (std::shared_ptr<DecreaseSpellCreationMovesLeftEvent> decreaseSpellCreationMovesLeftEvent = std::dynamic_pointer_cast<DecreaseSpellCreationMovesLeftEvent>(e)) {
		this->handleDecreaseSpellCreationMovesLeftEvent(decreaseSpellCreationMovesLeftEvent);
	}
	else if (std::shared_ptr<SetSpellEvent> setSpellEvent = std::dynamic_pointer_cast<SetSpellEvent>(e)) {
		this->handleSetSpellEvent(setSpellEvent);
	}
	else if (std::shared_ptr<UseSpellEvent> useSpellEvent = std::dynamic_pointer_cast<UseSpellEvent>(e)) {
		this->handleUseSpellEvent(useSpellEvent);
	}
	else if (std::shared_ptr<MarkSpellAsUsedEvent> markSpellAsUsedEvent = std::dynamic_pointer_cast<MarkSpellAsUsedEvent>(e)) {
		this->handleMarkSpellAsUsedEvent(markSpellAsUsedEvent);
	}
	else if (std::shared_ptr<EnableWarriorRageModeEvent> enableWarriorRageModeEvent = std::dynamic_pointer_cast<EnableWarriorRageModeEvent>(e)) {
		this->handleEnableWarriorRageModeEvent(enableWarriorRageModeEvent);
	}
	else if (std::shared_ptr<DecreaseRageModeMovesLeftEvent> decreaseRageModeMovesLeftEvent = std::dynamic_pointer_cast<DecreaseRageModeMovesLeftEvent>(e)) {
		this->handleDecreaseRageModeMovesLeftEvent(decreaseRageModeMovesLeftEvent);
	}
	else if (std::shared_ptr<CreateEffectEvent> createEffectEvent = std::dynamic_pointer_cast<CreateEffectEvent>(e)) {
		this->handleCreateEffectEvent(createEffectEvent);
	}
}
void MainScreen::handleAddResourceEvent(std::shared_ptr<AddResourceEvent> e) {
	this->getCurrentPlayer()->addResource(e->getResource(), e->getLimit().get(e->getResource().type));
}
void MainScreen::handleSubResourceEvent(std::shared_ptr<SubResourceEvent> e) {
	this->getCurrentPlayer()->subResource(e->getResource());
}
void MainScreen::handleAddResourcesEvent(std::shared_ptr<AddResourcesEvent> e) {
	this->getCurrentPlayer()->addResources(e->getResources(), e->getLimit());
}
void MainScreen::handleSubResourcesEvent(std::shared_ptr<SubResourcesEvent> e) {
	this->getCurrentPlayer()->subResources(e->getResources());
}
void MainScreen::handleSetHighlightEvent(std::shared_ptr<SetHighlightEvent> e) {
	this->highlightTable.mark(*e);
}
void MainScreen::handleAddHpEvent(std::shared_ptr<AddHpEvent> e) {
	HPGO* go = e->getHPGO();
	uint32_t n = e->getN();
	go->addHp(n);
}
void MainScreen::handleDecreaseCurrentTradeMovesLeft(std::shared_ptr<DecreaseCurrentTradeMovesLeftEvent> e) {
	e->getSpec()->decreaseCurrentTradeMovesLeft();
}
void MainScreen::handleBuild(std::shared_ptr<BuildEvent> e) {
	Building* b = e->getBuilding();
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
	Events destroyBuildingEvent = e->getBuilding()->destroy(this->map->getStatePtr());
	this->addEvents(destroyBuildingEvent);
}
void MainScreen::handleVictoryConditionBDestroyedEvent(std::shared_ptr<VictoryConditionBDestroyedEvent> e) {
	for (uint32_t i = 0; i < this->map->getStatePtr()->getCollectionsPtr()->totalBuildings(); i = i + 1) {
		Building* b = this->map->getStatePtr()->getCollectionsPtr()->getBuilding(i);
		if (b->getPlayerId() == e->getPlayerId() and b->exist() and b->isVictoryCondition()) {
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
	e->getSpec()->decreaseCurrentProducingMovesLeft();
}
void MainScreen::handleWarriorProducingFinishedEvent(std::shared_ptr<WarriorProducingFinishedEvent> e) {
	e->getSpec()->stopProducing();
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
void MainScreen::handleFocusOnEvent(std::shared_ptr<FocusOnEvent> e) {
	this->viewMovingQueue.emplace(
		64 * e->getX() + 64 * e->getSX() / 2,
		64 * e->getY() + 64 * e->getSY() / 2);
}
void MainScreen::handleResetHighlightEvent(std::shared_ptr<ResetHighlightEvent> e) {
	this->highlightTable.clear();
}
void MainScreen::handleDoTradeEvent(std::shared_ptr<DoTradeEvent> e) {
	Events events = e->getSpec()->doTrade(e->getBuilding(), e->getTrade());
	this->addEvents(events);
}
void MainScreen::handleStartWarriorProducingEvent(std::shared_ptr<StartWarriorProducingEvent> e) {
	Events events = e->getSpec()->startProducing(e->getWarrior());
	this->addEvents(events);
}
void MainScreen::handleTryToBuildEvent(std::shared_ptr<TryToBuildEvent> e) {
	if (this->getCurrentPlayer()->getResources() >= e->getBuilding()->getCost()) {
		this->bm = BuildingMode(e->getBuilding(), this->getCurrentPlayer()->getId());
		Events bmStartEvent = bm.start(this->map->getStatePtr());
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
void MainScreen::handleKillNextTurnEvent(std::shared_ptr<KillNextTurnEvent> e) {
	Events events = e->getWarrior()->killNextTurn();
	this->addEvents(events);
}
void MainScreen::handleRevertKillNextTurnEvent(std::shared_ptr<RevertKillNextTurnEvent> e) {
	Events events = e->getWarrior()->revertKillNextTurn();
	this->addEvents(events);
}
void MainScreen::handleCloseAnimationEvent(std::shared_ptr<CloseAnimationEvent> e) {
	this->animation = std::nullopt;
}
void MainScreen::handleDecreaseSpellCreationMovesLeftEvent(std::shared_ptr<DecreaseSpellCreationMovesLeftEvent> e) {
	e->getSpell()->decreaseCreationMovesLeft();
}
void MainScreen::handleSetSpellEvent(std::shared_ptr<SetSpellEvent> e) {
	e->getSpec()->setSpell(e->getSpell());
}
void MainScreen::handleUseSpellEvent(std::shared_ptr<UseSpellEvent> e) {
	Events events = e->getSpell()->use();
	this->addEvents(events);
}
void MainScreen::handleMarkSpellAsUsedEvent(std::shared_ptr<MarkSpellAsUsedEvent> e) {
	e->getSpell()->markAsUsed();
}
void MainScreen::handleEnableWarriorRageModeEvent(std::shared_ptr<EnableWarriorRageModeEvent> e) {
	e->getWarrior()->enableRageMode();
}
void MainScreen::handleDecreaseRageModeMovesLeftEvent(std::shared_ptr<DecreaseRageModeMovesLeftEvent> e) {
	e->getWarrior()->decreaseRageModeMovesLeft();
}
void MainScreen::handleCreateEffectEvent(std::shared_ptr<CreateEffectEvent> e) {
	this->map->add(e->getEffect());
}