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







#include <fstream>
#include <filesystem>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <boost/iostreams/filter/bzip2.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
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
#include "Locales.hpp"
#include "TradingSpec.hpp"
#include "ResourcePoint.hpp"
#include "WarriorProducerSpec.hpp"
#include "Warrior.hpp"
#include "House.hpp"
#include "Spell.hpp"
#include "SpellProducerSpec.hpp"
#include "SpellFactory.hpp"
#include "Infirmary.hpp"
#include "Tower1.hpp"
#include "Tower2.hpp"
#include "Well.hpp"
#include "WarehouseCrystal.hpp"
#include "ISingleAttacker.hpp"
#include "WarriorNearMultyAttacker.hpp"
#include "WarriorHealer.hpp"
#include "Workshop.hpp"
#include "VictoryConditionSpec.hpp"
#include "Root.hpp"
#include "Maps.hpp"
#include "StaticString.hpp"
#include "TextureNameStringSmart.hpp"
#include "ScreenAlreadyFinished.hpp"







template<typename T> StringLcl GET_BUILD_DESCRIPTION() {
	std::unique_ptr<T> t = std::make_unique<T>();
	StringLcl description = t->getDescription() + L'\n' +
		StringLcl("{cost}") + t->getCost().getReadableInfo();
	return description;
}
template<typename T> HorizontalSelectionWindowComponent GET_BUILD_COMPONENT() {
	Events clickSoundEvent;
	clickSoundEvent.add(std::make_shared<PlaySoundEvent>("click"));

	Events createBuildingModeEvent = clickSoundEvent;
	createBuildingModeEvent.add(std::make_shared<TryToBuildEvent>(std::make_shared<T>(0, 0, 0)));
	HorizontalSelectionWindowComponent component = {
		std::make_shared<TextureNameStringSmart>(std::make_shared<T>()),
		GET_BUILD_DESCRIPTION<T>(),
		true,
		createBuildingModeEvent
	};
	return component;
}
MainScreen::MainScreen(sf::RenderWindow& window, const MenuResponse& response) {
	this->alreadyFinished = false;




	switch (response.getType()) {
	case MenuResponse::TYPE::START_LOCAL_GAME: {
		this->map = Maps::get()->load(response.getData());
		this->playerIsActive.resize(this->map->getStatePtr()->getPlayersPtr()->total(), true);
		this->currentPlayerId = 1;
		this->move = 1;
		break;
	}
	case MenuResponse::TYPE::LOAD_LOCAL_GAME: {
		std::ifstream ifs(USERDATA_ROOT + "/saves/" + response.getData(), std::ios::binary);
		boost::iostreams::filtering_istreambuf fis;
		fis.push(boost::iostreams::bzip2_decompressor());
		fis.push(ifs);
		iarchive ia(fis);
		ia >> *this;
		break;
	}
	}



	Events clickSoundEvent;
	clickSoundEvent.add(std::make_shared<PlaySoundEvent>("click"));



	Events endMoveEvent = clickSoundEvent;
	endMoveEvent.add(std::make_shared<ChangeMoveEvent>());

	std::shared_ptr<WindowTwoButtons> confirmEndMoveWindow = std::make_shared<WindowTwoButtons>(StringLcl("{confirm_end_move}"), StringLcl("{yes}"), StringLcl("{no}"), endMoveEvent, clickSoundEvent);
	Events createConfirmEndMoveWindowEvent = clickSoundEvent;
	createConfirmEndMoveWindowEvent.add(std::make_shared<CreateEEvent>(confirmEndMoveWindow));



	Events returnToMenuEvent = clickSoundEvent;
	returnToMenuEvent.add(std::make_shared<ReturnToMenuEvent>());

	std::shared_ptr<WindowTwoButtons> confirmReturnToMenuWindow = std::make_shared<WindowTwoButtons>(StringLcl("{confirm_return_to_menu}"), StringLcl("{yes}"), StringLcl("{no}"), returnToMenuEvent, clickSoundEvent);
	Events createConfirmReturnToMenuWindowEvent = clickSoundEvent;
	createConfirmReturnToMenuWindowEvent.add(std::make_shared<CreateEEvent>(confirmReturnToMenuWindow));




	Events saveGameEvent = clickSoundEvent;
	saveGameEvent.add(std::make_shared<SaveGameEvent>());
	saveGameEvent.add(std::make_shared<CreateEEvent>(std::make_shared<WindowButton>(StringLcl("{game_saved}"), StringLcl("{OK}"), clickSoundEvent)));




	std::vector<HorizontalSelectionWindowComponent> buildMenuSectionMainComponents;
	buildMenuSectionMainComponents.emplace_back(std::make_shared<StaticString>("hammer_icon"), StringLcl("{leave}"), true, clickSoundEvent);
	buildMenuSectionMainComponents.emplace_back(GET_BUILD_COMPONENT<Road>());
	buildMenuSectionMainComponents.emplace_back(GET_BUILD_COMPONENT<House>());

	std::shared_ptr<HorizontalSelectionWindow> buildWindowSectionMain = std::make_shared<HorizontalSelectionWindow>(buildMenuSectionMainComponents);
	Events createBuildWindowSectionMainEvent = clickSoundEvent;
	createBuildWindowSectionMainEvent.add(std::make_shared<CreateEEvent>(buildWindowSectionMain));



	std::vector<HorizontalSelectionWindowComponent> buildMenuSectionResourceCollectorsComponents;
	buildMenuSectionResourceCollectorsComponents.emplace_back(std::make_shared<StaticString>("hammer_icon"), StringLcl("{leave}"), true, clickSoundEvent);
	buildMenuSectionResourceCollectorsComponents.emplace_back(GET_BUILD_COMPONENT<Arable>());
	buildMenuSectionResourceCollectorsComponents.emplace_back(GET_BUILD_COMPONENT<Sawmill>());
	buildMenuSectionResourceCollectorsComponents.emplace_back(GET_BUILD_COMPONENT<Quarry>());
	buildMenuSectionResourceCollectorsComponents.emplace_back(GET_BUILD_COMPONENT<Mine>());
	buildMenuSectionResourceCollectorsComponents.emplace_back(GET_BUILD_COMPONENT<Well>());

	std::shared_ptr<HorizontalSelectionWindow> buildWindowSectionResourceCollectors = std::make_shared<HorizontalSelectionWindow>(buildMenuSectionResourceCollectorsComponents);
	Events createBuildWindowSectionResourceCollectorsEvent = clickSoundEvent;
	createBuildWindowSectionResourceCollectorsEvent.add(std::make_shared<CreateEEvent>(buildWindowSectionResourceCollectors));



	std::vector<HorizontalSelectionWindowComponent> buildMenuSectionWarehousesComponents;
	buildMenuSectionWarehousesComponents.emplace_back(std::make_shared<StaticString>("hammer_icon"), StringLcl("{leave}"), true, clickSoundEvent);
	buildMenuSectionWarehousesComponents.emplace_back(GET_BUILD_COMPONENT<WarehouseFood>());
	buildMenuSectionWarehousesComponents.emplace_back(GET_BUILD_COMPONENT<WarehouseWood>());
	buildMenuSectionWarehousesComponents.emplace_back(GET_BUILD_COMPONENT<WarehouseStone>());
	buildMenuSectionWarehousesComponents.emplace_back(GET_BUILD_COMPONENT<WarehouseIron>());
	buildMenuSectionWarehousesComponents.emplace_back(GET_BUILD_COMPONENT<WarehouseCrystal>());
	buildMenuSectionWarehousesComponents.emplace_back(GET_BUILD_COMPONENT<WarehouseGold>());

	std::shared_ptr<HorizontalSelectionWindow> buildWindowSectionWarehouses = std::make_shared<HorizontalSelectionWindow>(buildMenuSectionWarehousesComponents);
	Events createBuildWindowSectionWarehousesEvent = clickSoundEvent;
	createBuildWindowSectionWarehousesEvent.add(std::make_shared<CreateEEvent>(buildWindowSectionWarehouses));



	std::vector<HorizontalSelectionWindowComponent> buildMenuSectionTroopsComponents;
	buildMenuSectionTroopsComponents.emplace_back(std::make_shared<StaticString>("hammer_icon"), StringLcl("{leave}"), true, clickSoundEvent);
	buildMenuSectionTroopsComponents.emplace_back(GET_BUILD_COMPONENT<Barracks>());
	buildMenuSectionTroopsComponents.emplace_back(GET_BUILD_COMPONENT<Infirmary>());
	buildMenuSectionTroopsComponents.emplace_back(GET_BUILD_COMPONENT<Workshop>());

	std::shared_ptr<HorizontalSelectionWindow> buildWindowSectionTroops = std::make_shared<HorizontalSelectionWindow>(buildMenuSectionTroopsComponents);
	Events createBuildWindowSectionTroopsEvent = clickSoundEvent;
	createBuildWindowSectionTroopsEvent.add(std::make_shared<CreateEEvent>(buildWindowSectionTroops));



	std::vector<HorizontalSelectionWindowComponent> buildMenuSectionDefenceComponents;
	buildMenuSectionDefenceComponents.emplace_back(std::make_shared<StaticString>("hammer_icon"), StringLcl("{leave}"), true, clickSoundEvent);
	buildMenuSectionDefenceComponents.emplace_back(GET_BUILD_COMPONENT<Wall1>());
	buildMenuSectionDefenceComponents.emplace_back(GET_BUILD_COMPONENT<Gates1>());
	buildMenuSectionDefenceComponents.emplace_back(GET_BUILD_COMPONENT<Tower1>());
	buildMenuSectionDefenceComponents.emplace_back(GET_BUILD_COMPONENT<Wall2>());
	buildMenuSectionDefenceComponents.emplace_back(GET_BUILD_COMPONENT<Gates2>());
	buildMenuSectionDefenceComponents.emplace_back(GET_BUILD_COMPONENT<Tower2>());

	std::shared_ptr<HorizontalSelectionWindow> buildWindowSectionDefence = std::make_shared<HorizontalSelectionWindow>(buildMenuSectionDefenceComponents);
	Events createBuildWindowSectionDefenceEvent = clickSoundEvent;
	createBuildWindowSectionDefenceEvent.add(std::make_shared<CreateEEvent>(buildWindowSectionDefence));



	std::vector<HorizontalSelectionWindowComponent> buildMenuSectionOtherComponents;
	buildMenuSectionOtherComponents.emplace_back(std::make_shared<StaticString>("hammer_icon"), StringLcl("{leave}"), true, clickSoundEvent);
	buildMenuSectionOtherComponents.emplace_back(GET_BUILD_COMPONENT<Market>());
	buildMenuSectionOtherComponents.emplace_back(GET_BUILD_COMPONENT<SpellFactory>());

	std::shared_ptr<HorizontalSelectionWindow> buildWindowSectionOther = std::make_shared<HorizontalSelectionWindow>(buildMenuSectionOtherComponents);
	Events createBuildWindowSectionOtherEvent = clickSoundEvent;
	createBuildWindowSectionOtherEvent.add(std::make_shared<CreateEEvent>(buildWindowSectionOther));



	std::vector<HorizontalSelectionWindowComponent> buildMenuComponents;
	buildMenuComponents.emplace_back(std::make_shared<StaticString>("hammer_icon"), StringLcl("{leave}"), true, clickSoundEvent);
	buildMenuComponents.emplace_back(std::make_shared<StaticString>("hammer_icon"), StringLcl("{main}"), true, createBuildWindowSectionMainEvent);
	buildMenuComponents.emplace_back(std::make_shared<StaticString>("hammer_icon"), StringLcl("{resource_collectors}"), true, createBuildWindowSectionResourceCollectorsEvent);
	buildMenuComponents.emplace_back(std::make_shared<StaticString>("hammer_icon"), StringLcl("{warehouses}"), true, createBuildWindowSectionWarehousesEvent);
	buildMenuComponents.emplace_back(std::make_shared<StaticString>("hammer_icon"), StringLcl("{troops}"), true, createBuildWindowSectionTroopsEvent);
	buildMenuComponents.emplace_back(std::make_shared<StaticString>("hammer_icon"), StringLcl("{defence}"), true, createBuildWindowSectionDefenceEvent);
	buildMenuComponents.emplace_back(std::make_shared<StaticString>("hammer_icon"), StringLcl("{other}"), true, createBuildWindowSectionOtherEvent);

	std::shared_ptr<HorizontalSelectionWindow> buildWindow = std::make_shared<HorizontalSelectionWindow>(buildMenuComponents);
	Events buildEvent = clickSoundEvent;
	buildEvent.add(std::make_shared<CreateEEvent>(buildWindow));



	this->returnToMenu = false;
	this->curcorVisibility = true;
	this->element = nullptr;
	this->illiminanceTable = std::make_shared<IlluminanceTable>(window.getSize().x, window.getSize().y, window.getSettings());
	this->selected = nullptr;
	this->animation = std::nullopt;
    this->currentGOIndexNewMoveEvent = this->map->getStatePtr()->getCollectionsPtr()->totalGOs();
    this->totalGONewMoveEvents = this->map->getStatePtr()->getCollectionsPtr()->totalGOs();
	this->view = sf::View(window.getDefaultView());

	this->buttons.emplace_back(std::make_shared<Label>(5, 40, 200, 60, StringLcl("{to_menu}")), createConfirmReturnToMenuWindowEvent);
	this->buttons.emplace_back(std::make_shared<Label>(5, 110, 200, 60, StringLcl("{save_game}")), saveGameEvent);
	this->buttons.emplace_back(std::make_shared<Label>(5, 180, 200, 60, StringLcl("{new_move}")), createConfirmEndMoveWindowEvent);
	this->buttons.emplace_back(std::make_shared<Image>(5, 250, std::make_shared<StaticString>("hammer_icon")), buildEvent);
}








MainScreenResponse MainScreen::run(sf::RenderWindow &window) {
	if (this->alreadyFinished) {
		throw ScreenAlreadyFinished();
	}
	this->alreadyFinished = true;

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
                                        this->addButtonClickEventToQueue(window);
                                    }
                                    if (this->events.empty()) {
                                        this->addGameObjectClickEventToQueue(event.mouseButton.button, window);
                                    }
                                }
                                else {
                                    std::tuple<uint32_t, uint32_t> pos = this->getMousePositionBasedOnView(window);
                                    Events unselectEvent = this->selected->unselect(this->map->getStatePtr(), std::get<0>(pos) / 64, std::get<1>(pos) / 64, event.mouseButton.button);
                                    this->addEvents(unselectEvent);
                                }
                            }
                        }
                    }
                    else  {
                        if (event.mouseButton.button == sf::Mouse::Button::Left) {
                            Events elementClickEvent = this->element->click(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y, window.getSize().x, window.getSize().y);
                            this->addEvents(elementClickEvent);
                        }
                    }
                }
            }
        }
        this->drawEverything(window);
        for (uint32_t i = 0; i < this->map->getStatePtr()->getCollectionsPtr()->totalGOs(); i = i + 1) {
            this->map->getStatePtr()->getCollectionsPtr()->getGO(i, FILTER::DEFAULT_PRIORITY)->newFrame(this->map->getStatePtr(), this->getCurrentPlayer()->getId());
        }
        Playlist::get()->update();
        this->processNewMoveEvents(window);
        this->processBaseEvents();
        if (this->element != nullptr) {
            this->element->update();
			if (this->element->finished()) {
				this->element = nullptr;
			}
        }
        if (this->animation.has_value()) {
            Events animationEvent;
            animationEvent = this->animation.value().process(this->map->getStatePtr());
            this->addEvents(animationEvent);
        }
        this->moveView(window);
        if (this->returnToMenu) {
			Playlist::get()->stop();
			return MainScreenResponse(MainScreenResponse::TYPE::RETURN_TO_MENU);
        }
        window.setMouseCursorVisible(this->curcorVisibility);
    }
}










void MainScreen::save() const {
    if (!std::filesystem::is_directory(USERDATA_ROOT + "/saves")) {
        std::filesystem::create_directories(USERDATA_ROOT + "/saves");
    }
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);
	std::stringstream ss;
	ss << std::put_time(&tm, "%Y-%m-%d %H-%M-%S");
    std::ofstream ofs(USERDATA_ROOT + "/saves/" + ss.str() + ".save", std::ios::binary);
    boost::iostreams::filtering_ostreambuf fos;
    fos.push(boost::iostreams::bzip2_compressor());
    fos.push(ofs);
    oarchive oa(fos);
    oa << *this;
}














void MainScreen::drawEverything(sf::RenderWindow& window) {
	window.clear();
	window.setView(this->view);
	this->drawCells(window);
	this->drawMap(window);
	this->drawHighlightion(window);
    this->drawDarkness(window);
    if (this->selected != nullptr) {
        window.draw(*this->selected->getSelectablePointer(std::get<0>(this->getMousePositionBasedOnView(window)), std::get<1>(this->getMousePositionBasedOnView(window))));
    }
    if (this->element != nullptr and this->element->isCameraDependent()) {
        window.draw(*this->element);
    }
	window.setView(window.getDefaultView());
    if (this->element != nullptr and !this->element->isCameraDependent()) {
        window.draw(*this->element);
    }
	this->drawResourceBar(window);
	for (const auto& b : this->buttons) {
		window.draw(b);
	}
	window.display();
}
void MainScreen::drawMap(sf::RenderWindow& window) {
	for (uint32_t i = 0; i < this->map->getStatePtr()->getCollectionsPtr()->totalGOs(); i = i + 1) {
		GO* go = this->map->getStatePtr()->getCollectionsPtr()->getGO(i, FILTER::DRAW_PRIORITY);
		if (go->exist() and go->inView(this->view)) {
			window.draw(*go);
		}
	}
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
    uint32_t sx = Textures::get()->get("plain")->getSize().x / 64;
    uint32_t sy = Textures::get()->get("plain")->getSize().y / 64;
	for (uint32_t i = 0; i < this->map->getStatePtr()->getMapSizePtr()->getWidth(); i = i + sx) {
		for (uint32_t j = 0; j < this->map->getStatePtr()->getMapSizePtr()->getHeight(); j = j + sy) {
			sf::Sprite s;
			s.setTexture(*Textures::get()->get("plain"));
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
void MainScreen::drawDarkness(sf::RenderWindow &window) {
	this->illiminanceTable->newFrame(this->view);

	for (uint32_t i = 0; i < this->map->getStatePtr()->getCollectionsPtr()->totalGOs(); i = i + 1) {
		const GO* go = this->map->getStatePtr()->getCollectionsPtr()->getGO(i, FILTER::DEFAULT_PRIORITY);
		if (go->exist()) {
			this->illiminanceTable->add(go);
		}
	}

	window.draw(*this->illiminanceTable);
}










void MainScreen::processNewMoveEvents(sf::RenderWindow& window) {
	while (this->currentGOIndexNewMoveEvent != this->totalGONewMoveEvents) {
		if (this->element != nullptr or !this->events.empty()) {
			break;
		}
		Events newMoveEvent = this->map->getStatePtr()->getCollectionsPtr()->getGO(this->currentGOIndexNewMoveEvent, FILTER::NEW_MOVE_PRIORITY)->newMove(this->map->getStatePtr(), this->getCurrentPlayer()->getId());
		this->addEvents(newMoveEvent);
		this->currentGOIndexNewMoveEvent = this->currentGOIndexNewMoveEvent + 1;
        this->processBaseEvents();
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
		this->currentPlayerId = this->currentPlayerId + 1;
        if (this->currentPlayerId > this->map->getStatePtr()->getPlayersPtr()->total()) {
            this->currentPlayerId = 1;
        }
	}
	while (!this->playerIsActive.at(this->currentPlayerId - 1));
}
Player* MainScreen::getCurrentPlayer() {
	return this->map->getStatePtr()->getPlayersPtr()->getPlayerPtr(this->currentPlayerId);
}
void MainScreen::addButtonClickEventToQueue(sf::RenderWindow& window) {
	for (uint32_t i = 0; i < this->buttons.size(); i = i + 1) {
		Events buttonClickEvent = this->buttons.at(i).click(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
		if (!buttonClickEvent.empty()) {
			this->addEvents(buttonClickEvent);
			break;
		}
	}
}
void MainScreen::addGameObjectClickEventToQueue(uint8_t button, sf::RenderWindow& window) {
	uint32_t mouseX, mouseY;
	std::tie(mouseX, mouseY) = this->getMousePositionBasedOnView(window);

    for (uint32_t i = 0; i < this->map->getStatePtr()->getCollectionsPtr()->totalGOs(); i = i + 1) {
        GO* go = this->map->getStatePtr()->getCollectionsPtr()->getGO(i, FILTER::CLICK_PRIORITY);
        Events gor = go->click(this->map->getStatePtr(), this->getCurrentPlayer()->getId(), button, mouseX, mouseY);
        if (!gor.empty()) {
            this->addEvents(gor);
            return;
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
std::tuple<uint32_t, uint32_t> MainScreen::getMousePositionBasedOnView(sf::RenderWindow &window) const {
	uint32_t mouseX = sf::Mouse::getPosition().x + this->view.getCenter().x - window.getSize().x / 2;
	uint32_t mouseY = sf::Mouse::getPosition().y + this->view.getCenter().y - window.getSize().y / 2;
	return std::make_tuple(mouseX, mouseY);
}
void MainScreen::moveView(sf::RenderWindow &window) {
	if (this->viewMovingQueue.empty()) {
		auto p = sf::Mouse::getPosition();
		if (p.x < 10 or sf::Keyboard::isKeyPressed(sf::Keyboard::A) or sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			this->moveViewToWest(window);
		}
		else if (p.x > window.getSize().x - 10 or sf::Keyboard::isKeyPressed(sf::Keyboard::D) or sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			this->moveViewToEast(window);
		}
		if (p.y < 10 or sf::Keyboard::isKeyPressed(sf::Keyboard::W) or sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			this->moveViewToNorth(window);
		}
		else if (p.y > window.getSize().y - 10 or sf::Keyboard::isKeyPressed(sf::Keyboard::S) or sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			this->moveViewToSouth(window);
		}
	}
	else {
		uint32_t currentX, currentY;
		std::tie(currentX, currentY) = this->viewMovingQueue.front();

		uint32_t viewX = this->view.getCenter().x;
		uint32_t viewY = this->view.getCenter().y;

		bool horizontalOk = false;
		bool verticalOk = false;

		if (currentX < viewX) {
			horizontalOk = horizontalOk or !this->moveViewToWest(currentX, window);
		}
		else if (currentX > viewX) {
			horizontalOk = horizontalOk or !this->moveViewToEast(currentX, window);
		}
		else {
			horizontalOk = true;
		}

		if (currentY < viewY) {
			verticalOk = verticalOk or !this->moveViewToNorth(currentY, window);
		}
		else if (currentY > viewY) {
			verticalOk = verticalOk or !this->moveViewToSouth(currentY, window);
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
bool MainScreen::moveViewToNorth(uint32_t border, sf::RenderWindow& window) {
	this->view.setCenter(this->view.getCenter() - sf::Vector2f(0, VIEW_MOVING_BIG_DELTA));
	return this->verifyViewNorth(window) and this->verifyViewNorth(border);
}
bool MainScreen::moveViewToNorth(sf::RenderWindow& window) {
	this->view.setCenter(this->view.getCenter() - sf::Vector2f(0, VIEW_MOVING_DELTA));
	return this->verifyViewNorth(window);
}
bool MainScreen::moveViewToSouth(uint32_t border, sf::RenderWindow& window) {
	this->view.setCenter(this->view.getCenter() + sf::Vector2f(0, VIEW_MOVING_BIG_DELTA));
	return this->verifyViewSouth(window) and this->verifyViewSouth(border);
}
bool MainScreen::moveViewToSouth(sf::RenderWindow& window) {
	this->view.setCenter(this->view.getCenter() + sf::Vector2f(0, VIEW_MOVING_DELTA));
	return this->verifyViewSouth(window);
}
bool MainScreen::moveViewToWest(uint32_t border, sf::RenderWindow& window) {
	this->view.setCenter(this->view.getCenter() - sf::Vector2f(VIEW_MOVING_BIG_DELTA, 0));
	return this->verifyViewWest(window) and this->verifyViewWest(border);
}
bool MainScreen::moveViewToWest(sf::RenderWindow& window) {
	this->view.setCenter(this->view.getCenter() - sf::Vector2f(VIEW_MOVING_DELTA, 0));
	return this->verifyViewWest(window);
}
bool MainScreen::moveViewToEast(uint32_t border, sf::RenderWindow& window) {
	this->view.setCenter(this->view.getCenter() + sf::Vector2f(VIEW_MOVING_BIG_DELTA, 0));
	return this->verifyViewEast(window) and this->verifyViewEast(border);
}
bool MainScreen::moveViewToEast(sf::RenderWindow& window) {
	this->view.setCenter(this->view.getCenter() + sf::Vector2f(VIEW_MOVING_DELTA, 0));
	return this->verifyViewEast(window);
}
bool MainScreen::verifyViewNorth(uint32_t border) {
	if (this->view.getCenter().y < border) {
		this->view.setCenter(sf::Vector2f(this->view.getCenter().x, border));
		return false;
	}
	return true;
}
bool MainScreen::verifyViewNorth(sf::RenderWindow& window) {
	return this->verifyViewNorth(window.getSize().y / 2);
}
bool MainScreen::verifyViewSouth(uint32_t border) {
	if (this->view.getCenter().y > border) {
		this->view.setCenter(sf::Vector2f(this->view.getCenter().x, border));
		return false;
	}
	return true;
}
bool MainScreen::verifyViewSouth(sf::RenderWindow& window) {
	return this->verifyViewSouth(64 * this->map->getStatePtr()->getMapSizePtr()->getHeight() - window.getSize().y / 2);
}
bool MainScreen::verifyViewWest(uint32_t border) {
	if (this->view.getCenter().x < border) {
		this->view.setCenter(sf::Vector2f(border, this->view.getCenter().y));
		return false;
	}
	return true;
}
bool MainScreen::verifyViewWest(sf::RenderWindow& window) {
	return this->verifyViewWest(window.getSize().x / 2);
}
bool MainScreen::verifyViewEast(uint32_t border) {
	if (this->view.getCenter().x > border) {
		this->view.setCenter(sf::Vector2f(border, this->view.getCenter().y));
		return false;
	}
	return true;
}
bool MainScreen::verifyViewEast(sf::RenderWindow& window) {
	return this->verifyViewEast(64 * this->map->getStatePtr()->getMapSizePtr()->getWidth() - window.getSize().x / 2);
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
    else if (std::shared_ptr<RefreshAttackAbilityEvent> refreshAttackAbilityEvent = std::dynamic_pointer_cast<RefreshAttackAbilityEvent>(e)) {
        this->handleRefreshAttackAbilityEvent(refreshAttackAbilityEvent);
    }
    else if (std::shared_ptr<WipeAttackAbilityEvent> wipeAttackAbilityEvent = std::dynamic_pointer_cast<WipeAttackAbilityEvent>(e)) {
        this->handleWipeAttackAbilityEvent(wipeAttackAbilityEvent);
    }
    else if (std::shared_ptr<RefreshAttackedTableEvent> refreshAttackedTableEvent = std::dynamic_pointer_cast<RefreshAttackedTableEvent>(e)) {
        this->handleRefreshAttackedTableEvent(refreshAttackedTableEvent);
    }
    else if (std::shared_ptr<MarkAsAttackedEvent> markAsAttackedEvent = std::dynamic_pointer_cast<MarkAsAttackedEvent>(e)) {
        this->handleMarkAsAttackedEvent(markAsAttackedEvent);
    }
    else if (std::shared_ptr<RefreshHealingAbilityEvent> refreshHealingAbilityEvent = std::dynamic_pointer_cast<RefreshHealingAbilityEvent>(e)) {
        this->handleRefreshHealingAbilityEvent(refreshHealingAbilityEvent);
    }
    else if (std::shared_ptr<WipeHealingAbilityEvent> wipeHealingAbilityEvent = std::dynamic_pointer_cast<WipeHealingAbilityEvent>(e)) {
        this->handleWipeHealingAbilityEvent(wipeHealingAbilityEvent);
    }
    else if (std::shared_ptr<MarkPlayerAsInactiveEvent> markPlayerAsInactiveEvent = std::dynamic_pointer_cast<MarkPlayerAsInactiveEvent>(e)) {
        this->handleMarkPlayerAsInactiveEvent(markPlayerAsInactiveEvent);
    }
    else if (std::shared_ptr<IncreaseVCSMoveCtrEvent> increaseVcsMoveCtrEvent = std::dynamic_pointer_cast<IncreaseVCSMoveCtrEvent>(e)) {
        this->handleIncreaseVCSMoveCtrEvent(increaseVcsMoveCtrEvent);
    }
    else if (std::shared_ptr<SaveGameEvent> saveGameEvent = std::dynamic_pointer_cast<SaveGameEvent>(e)) {
        this->handleSaveGameEvent(saveGameEvent);
    }
    else if (std::shared_ptr<LimitResourcesEvent> limitResourcesEvent = std::dynamic_pointer_cast<LimitResourcesEvent>(e)) {
        this->handleLimitResourcesEvent(limitResourcesEvent);
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
	this->map->getStatePtr()->getCollectionsPtr()->add(b);
}
void MainScreen::handlePlaySoundEvent(std::shared_ptr<PlaySoundEvent> e) {
    if (e->getSoundName().empty()) {
        return;
    }
	SoundQueue::get()->push(Sounds::get()->get(e->getSoundName()));
}
void MainScreen::handleCreatePopUpElementEvent(std::shared_ptr<CreateEEvent> e) {
    this->element = e->getElement();
	this->element->restart();
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
void MainScreen::handleDecreaseCurrentProdusingMovesLeftEvent(std::shared_ptr<DecreaseCurrentProducingMovesLeftEvent> e) {
	e->getSpec()->decreaseCurrentProducingMovesLeft();
}
void MainScreen::handleWarriorProducingFinishedEvent(std::shared_ptr<WarriorProducingFinishedEvent> e) {
	e->getSpec()->stopProducing();
	this->map->getStatePtr()->getCollectionsPtr()->add(e->getWarrior()->cloneWarrior());
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

		std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(StringLcl("{no_resources_for_building}"), StringLcl("{OK}"), clickEvent);
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
void MainScreen::handleRefreshAttackAbilityEvent(std::shared_ptr<RefreshAttackAbilityEvent> e) {
    e->getI()->refreshAbility();
}
void MainScreen::handleWipeAttackAbilityEvent(std::shared_ptr<WipeAttackAbilityEvent> e) {
    e->getI()->wipeAbility();
}
void MainScreen::handleRefreshAttackedTableEvent(std::shared_ptr<RefreshAttackedTableEvent> e) {
    e->getWarrior()->refreshAttackedTable();
}
void MainScreen::handleMarkAsAttackedEvent(std::shared_ptr<MarkAsAttackedEvent> e) {
    e->getAttacker()->markAsAttacked(e->getTarget());
}
void MainScreen::handleRefreshHealingAbilityEvent(std::shared_ptr<RefreshHealingAbilityEvent> e) {
    e->getWarrior()->refreshHealingAbility();
}
void MainScreen::handleWipeHealingAbilityEvent(std::shared_ptr<WipeHealingAbilityEvent> e) {
    e->getWarrior()->wipeHealingAbility();
}
void MainScreen::handleMarkPlayerAsInactiveEvent(std::shared_ptr<MarkPlayerAsInactiveEvent> e) {
    this->playerIsActive[e->getPlayerId() - 1] = false;
    uint32_t count = 0;
    for (uint32_t i = 0; i < this->playerIsActive.size(); i = i + 1) {
        count = count + this->playerIsActive.at(i);
    }
    std::shared_ptr<WindowButton> w;
    if (count == 1) {
        Events returnToMenuEvent;
        returnToMenuEvent.add(std::make_shared<PlaySoundEvent>("click"));
        returnToMenuEvent.add(std::make_shared<ReturnToMenuEvent>());
        w = std::make_shared<WindowButton>(StringLcl("{game_finished}"), StringLcl("{OK}"), returnToMenuEvent);
    }
    else {
        Events event;
        event.add(std::make_shared<PlaySoundEvent>("click"));
        if (this->currentPlayerId == e->getPlayerId()) {
            event.add(std::make_shared<ChangeMoveEvent>());
        }
        w = std::make_shared<WindowButton>(StringLcl("{player_is_out}"), StringLcl("{OK}"), event);
    }
    std::shared_ptr<CreateEEvent> createW = std::make_shared<CreateEEvent>(w);
    this->handleCreatePopUpElementEvent(createW);
}
void MainScreen::handleIncreaseVCSMoveCtrEvent(std::shared_ptr<IncreaseVCSMoveCtrEvent> e) {
    e->getSpec()->increaseMoveCtr();
}
void MainScreen::handleSaveGameEvent(std::shared_ptr<SaveGameEvent> e) {
    this->save();
}
void MainScreen::handleLimitResourcesEvent(std::shared_ptr<LimitResourcesEvent> e) {
    this->map->getStatePtr()->getPlayersPtr()->getPlayerPtr(e->getPlayerId())->limitResources(e->getLimit());
}