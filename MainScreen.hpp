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


#include <fstream>
#include <queue>
#include "Playlist.hpp"
#include "Forest.hpp"
#include "Stone.hpp"
#include "Iron.hpp"
#include "Castle.hpp"
#include "Market.hpp"
#include "Farm.hpp"
#include "Sawmill.hpp"
#include "Quarry.hpp"
#include "Mine.hpp"
#include "Wall.hpp"
#include "Road.hpp"
#include "SoundQueue.hpp"
#include "HighlightTable.hpp"
#include "PlainsGeneration.hpp"
#include "BuildingMode.hpp"


#pragma once


class MainScreen {
public:
	static MainScreen* get() {
		if (MainScreen::singletone == nullptr) {
			MainScreen::singletone = new MainScreen();
		}
		return MainScreen::singletone;
	}

	int32_t run(sf::RenderWindow& window);
private:
	MainScreen() = default;
	MainScreen(const MainScreen& copy) = delete;
	static MainScreen* singletone;

	std::queue<PopUpElement*> elements;
	Player players[2];
	uint32_t move = 0;
	std::vector<GO*> *gameObjects;
	std::vector<Unit*> *units;
	std::vector<ResourcePoint*> *resourcePoints;
	std::vector<TerritoryB*> *territoryBuildings;
	std::vector<TerritoryOriginB*> *territoryOrigins;
	std::vector<TerritoryConductorB*> *territoryConductors;
	HighlightTable highlightTable;
	PlainsGeneration plains;

	uint32_t windowW, windowH;
	uint32_t *mouseX, *mouseY;
	sf::View *view;
	Button endMoveButton;
	Button buildButton;

	void init(sf::RenderWindow &window);
	void initMap(const std::string &name);
	void removeOldPopUpWindows();
	void initPlayers();
	void initMoveCtr();
	void initHighlightTable();
	void initGraphics(sf::RenderWindow &window);

	void handleEvents(const Events& e);

	void handleGameEvent(const GEvent& e);
	void handleTryToAttackEvent(const GEvent& e);
	void handleTryToTradeEvent(const GEvent& e);
	void handleAddResourceEvent(const GEvent& e);
	void handleSubResourceEvent(const GEvent& e);
	void handleAddResourcesEvent(const GEvent& e);
	void handleSubResourcesEvent(const GEvent& e);
	void handleChangeHighlightEvent(const GEvent& e);
	void handleCollectEvent(const GEvent& e);
	void handleTryToUpgradeEvent(const GEvent& e);
	void handleAddHpEvent(const GEvent& e);
	void handleDecreaseUpgradeMovesLeftEvent(const GEvent& e);
	void handleIncreaseLevelEvent(const GEvent& e);
	void handleDecreaseCurrentTradeMovesLeft(const GEvent& e);
	void handleTryToBuild(const GEvent& e);
	void handleBuild(const GEvent& e);
	
	void handleUIEvent(const UIEvent& e);
	void handlePlaySoundEvent(const UIEvent& e);
	void handleCreatePopUpElementEvent(const UIEvent& e);

	void removeFinishedElements();
	void changeMove();
	void createBuildMenu();
	static std::wstring GET_BUILD_DESCRIPTION(Building* b);
	Player* getCurrentPlayer();
	void handleGameObjectClick();
	void addPopUpWindows(std::queue<PopUpElement*> q);
	void addPopUpWindow(PopUpElement* w);
	void prepareToReturnToMenu(sf::RenderWindow &window);
	void drawEverything(sf::RenderWindow& window);
	void drawCells(sf::RenderWindow &window);
	void updateMousePosition();
	void moveView();
	void updatePlayerViewPoint();

	void moveViewToNorth();
	void moveViewToSouth();
	void moveViewToWest();
	void moveViewToEast();

	void add(GO* go);
};