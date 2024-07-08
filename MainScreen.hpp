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
#include <queue>
#include "Playlist.hpp"
#include "Castle.hpp"
#include "Market.hpp"
#include "Windmill.hpp"
#include "Sawmill.hpp"
#include "Quarry.hpp"
#include "Mine.hpp"
#include "Wall.hpp"
#include "Road.hpp"
#include "Maps.hpp"
#include "SoundQueue.hpp"
#include "HighlightTable.hpp"
#include "PlainsGeneration.hpp"
#include "BuildingMode.hpp"
#include "Sounds.hpp"
#include "WindowButton.hpp"
#include "AddResourcesEvent.hpp"
#include "TryToBuildEvent.hpp"
#include "BuildEvent.hpp"
#include "ChangeMoveEvent.hpp"
#include "ReturnToMenuEvent.hpp"
#include "WindowTwoButtons.hpp"


#pragma once


class MainScreen {
public:
	static MainScreen* get() {
		if (MainScreen::singletone == nullptr) {
			MainScreen::singletone = new MainScreen();
		}
		return MainScreen::singletone;
	}

	bool run(std::shared_ptr<Map> mapPtr, sf::RenderWindow& window);
private:
	MainScreen() = default;
	MainScreen(const MainScreen& copy) = delete;
	static MainScreen* singletone;

    std::shared_ptr<Map> map;
	std::queue<std::shared_ptr<PopUpElement>> elements;
	uint32_t move = 0;
	HighlightTable highlightTable;
	PlainsGeneration plains;

    bool returnToMenu;
	uint32_t windowW, windowH;
	std::shared_ptr<sf::View> view;
    std::vector<Button> buttons;

	void init(std::shared_ptr<Map> mapPtr, sf::RenderWindow &window);
	void initMap(std::shared_ptr<Map> mapPtr);
	void initMoveCtr();
    void initPlains();
	void initGraphics(sf::RenderWindow &window);

	void handleEvent(Events& e);

	void handleTryToAttackEvent(std::shared_ptr<TryToAttackEvent> e);
	void handleTryToTradeEvent(std::shared_ptr<TryToTradeEvent> e);
	void handleAddResourceEvent(std::shared_ptr<AddResourceEvent> e);
	void handleSubResourceEvent(std::shared_ptr<SubResourceEvent> e);
	void handleAddResourcesEvent(std::shared_ptr<AddResourcesEvent> e);
	void handleSubResourcesEvent(std::shared_ptr<SubResourcesEvent> e);
	void handleChangeHighlightEvent(std::shared_ptr<ChangeHighlightEvent> e);
	void handleCollectEvent(std::shared_ptr<CollectEvent> e);
	void handleTryToUpgradeEvent(std::shared_ptr<TryToUpgradeEvent> e);
	void handleAddHpEvent(std::shared_ptr<AddHpEvent> e);
	void handleDecreaseUpgradeMovesLeftEvent(std::shared_ptr<DecreaseUpgradeMovesLeftEvent> e);
	void handleIncreaseLevelEvent(std::shared_ptr<IncreaseLevelEvent> e);
	void handleDecreaseCurrentTradeMovesLeft(std::shared_ptr<DecreaseCurrentTradeMovesLeftEvent> e);
	void handleTryToBuild(std::shared_ptr<TryToBuildEvent> e);
	void handleBuild(std::shared_ptr<BuildEvent> e);
	void handlePlaySoundEvent(std::shared_ptr<PlaySoundEvent> e);
	void handleCreatePopUpElementEvent(std::shared_ptr<CreateEEvent> e);
    void handleChangeMoveEvent(std::shared_ptr<ChangeMoveEvent> e);
    void handleReturnToMenuEvent(std::shared_ptr<ReturnToMenuEvent> e);

	void removeFinishedElements();
	void changeMove();
	static std::wstring GET_BUILD_DESCRIPTION(std::unique_ptr<Building> b);
	std::shared_ptr<Player> getCurrentPlayer();
    bool handleButtonsClick();
	void handleGameObjectClick();
	void addPopUpWindow(std::shared_ptr<PopUpElement> w);
	void prepareToReturnToMenu(sf::RenderWindow &window);
	void drawEverything(sf::RenderWindow& window);
	void drawCells(sf::RenderWindow &window);
	void moveView();
	void updatePlayerViewPoint();

	void moveViewToNorth();
	void moveViewToSouth();
	void moveViewToWest();
	void moveViewToEast();
};