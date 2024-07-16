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


#include <queue>
#include <optional>
#include "Map.hpp"
#include "HighlightTable.hpp"
#include "PlainsGeneration.hpp"
#include "AddResourcesEvent.hpp"
#include "TryToBuildEvent.hpp"
#include "BuildEvent.hpp"
#include "ChangeMoveEvent.hpp"
#include "ReturnToMenuEvent.hpp"
#include "SubResourcesEvent.hpp"
#include "ChangeHighlightEvent.hpp"
#include "TryToTradeEvent.hpp"
#include "AddResourceEvent.hpp"
#include "SubResourceEvent.hpp"
#include "CollectEvent.hpp"
#include "DecreaseCurrentTradeMovesLeftEvent.hpp"
#include "DecreaseCurrentProducingMovesLeftEvent.hpp"
#include "TryToProduceEvent.hpp"
#include "WarriorProducingFinishedEvent.hpp"
#include "SelectEvent.hpp"
#include "UnselectEvent.hpp"
#include "StartWarriorAnimationEvent.hpp"
#include "PlaySoundEvent.hpp"
#include "ResourceStorageBDestroyedEvent.hpp"
#include "VictoryConditionBDestroyedEvent.hpp"
#include "TryToCollectEvent.hpp"
#include "RefreshMovementPointsEvent.hpp"
#include "EnableCursorEvent.hpp"
#include "DisableCursorEvent.hpp"
#include "CreateAnimationEvent.hpp"
#include "CloseAnimationEvent.hpp"
#include "AddHpEvent.hpp"
#include "CreateEEvent.hpp"
#include "DestroyEvent.hpp"
#include "DecreaseBurningMovesLeftEvent.hpp"
#include "SubHpEvent.hpp"
#include "SetFireEvent.hpp"
#include "ChangeWarriorDirectionEvent.hpp"
#include "TryToRaiseDragonEvent.hpp"
#include "DecreaseDragonRecoverMovesLeftEvent.hpp"
#include "ResetDragonRecoverMovesLeftEvent.hpp"


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
	std::vector<bool> playerIsActive;
	uint32_t currentPlayerIndex;
	std::shared_ptr<PopUpElement> element;
    std::optional<Animation> animation;
    std::queue<std::shared_ptr<Event>> baseEvents;
	uint32_t move = 0;
	HighlightTable highlightTable;
	PlainsGeneration plains;
	uint32_t currentGOIndexNewMoveEvent;
	uint32_t totalGONewMoveEvents;
	Selectable* selected;

    bool returnToMenu;
	bool curcorVisibility;
	uint32_t windowW, windowH;
	std::shared_ptr<sf::View> view;
    std::vector<Button> buttons;

	void init(std::shared_ptr<Map> mapPtr, sf::RenderWindow &window);
	void initMap(std::shared_ptr<Map> mapPtr);
	void initPlayerIsActiveTable();
	void initCurrentPlayerIndex();
	void initMoveCtr();
	void initSelectable();
    void initPlains();
	void initGraphics(sf::RenderWindow &window);
	static std::wstring GET_BUILD_DESCRIPTION(std::unique_ptr<Building> b);


	void drawEverything(sf::RenderWindow& window);
	void drawResourceBar(sf::RenderWindow& window);
	void drawCells(sf::RenderWindow& window);
	void drawHighlightion(sf::RenderWindow& window);


	void removeFinishedElement();
	void addNewMoveEvent();
	bool allNewMoveEventsAdded() const;
	void changeMove();
	Player* getCurrentPlayer();
	Resources getResourcesLimit();
	void addButtonClickEventToQueue();
	void addGameObjectClickEventToQueue();
    void processBaseEvents();
    void addEvents(Events &e);
	void prepareToReturnToMenu(sf::RenderWindow& window);
	std::tuple<uint32_t, uint32_t> getMousePositionBasedOnView() const;
	void moveView();
	void updatePlayerViewPoint();



	void moveViewToNorth();
	void moveViewToSouth();
	void moveViewToWest();
	void moveViewToEast();
	void verifyViewNorth();
	void verifyViewSouth();
	void verifyViewWest();
	void verifyViewEast();


	void handleBaseEvent(std::shared_ptr<Event> e);
	void handleTryToTradeEvent(std::shared_ptr<TryToTradeEvent> e);
	void handleAddResourceEvent(std::shared_ptr<AddResourceEvent> e);
	void handleSubResourceEvent(std::shared_ptr<SubResourceEvent> e);
	void handleAddResourcesEvent(std::shared_ptr<AddResourcesEvent> e);
	void handleSubResourcesEvent(std::shared_ptr<SubResourcesEvent> e);
	void handleChangeHighlightEvent(std::shared_ptr<ChangeHighlightEvent> e);
	void handleCollectEvent(std::shared_ptr<CollectEvent> e);
	void handleAddHpEvent(std::shared_ptr<AddHpEvent> e);
	void handleDecreaseCurrentTradeMovesLeft(std::shared_ptr<DecreaseCurrentTradeMovesLeftEvent> e);
	void handleTryToBuild(std::shared_ptr<TryToBuildEvent> e);
	void handleBuild(std::shared_ptr<BuildEvent> e);
	void handlePlaySoundEvent(std::shared_ptr<PlaySoundEvent> e);
	void handleCreatePopUpElementEvent(std::shared_ptr<CreateEEvent> e);
	void handleChangeMoveEvent(std::shared_ptr<ChangeMoveEvent> e);
	void handleReturnToMenuEvent(std::shared_ptr<ReturnToMenuEvent> e);
	void handleDestroyEvent(std::shared_ptr<DestroyEvent> e);
	void handleResourceStorageBDestroyedEvent(std::shared_ptr<ResourceStorageBDestroyedEvent> e);
	void handleVictoryConditionBDestroyedEvent(std::shared_ptr<VictoryConditionBDestroyedEvent> e);
	void handleDecreaseCurrentProdusingMovesLeftEvent(std::shared_ptr<DecreaseCurrentProducingMovesLeftEvent> e);
	void handleTryToProduceEvent(std::shared_ptr<TryToProduceEvent> e);
	void handleWarriorProducingFinishedEvent(std::shared_ptr<WarriorProducingFinishedEvent> e);
	void handleSelectEvent(std::shared_ptr<SelectEvent> w);
	void handleUnselectEvent(std::shared_ptr<UnselectEvent> w);
	void handleStartWarriorAnimationEvent(std::shared_ptr<StartWarriorAnimationEvent> e);
	void handleTryToCollectEvent(std::shared_ptr<TryToCollectEvent> e);
	void handleRefreshMovementPointsEvent(std::shared_ptr<RefreshMovementPointsEvent> e);
	void handleEnableCursorEvent(std::shared_ptr<EnableCursorEvent> e);
	void handleDisableCursorEvent(std::shared_ptr<DisableCursorEvent> e);
    void handleCreateAnimationEvent(std::shared_ptr<CreateAnimationEvent> e);
    void handleCloseAnimationEvent(std::shared_ptr<CloseAnimationEvent> e);
	void handleDecreaseBurningMovesLeftEvent(std::shared_ptr<DecreaseBurningMovesLeftEvent> e);
	void handleSubHpEvent(std::shared_ptr<SubHpEvent> e);
	void handleSetFireEvent(std::shared_ptr<SetFireEvent> e);
	void handleChangeWarriorDirectionEvent(std::shared_ptr<ChangeWarriorDirectionEvent> e);
	void handleTryToRaiseDragonEvent(std::shared_ptr<TryToRaiseDragonEvent> e);
	void handleDecreaseDragonRecoverMovesLeftEvent(std::shared_ptr<DecreaseDragonRecoverMovesLeftEvent> e);
	void handleResetDragonRecoverMovesLeftEvent(std::shared_ptr<ResetDragonRecoverMovesLeftEvent> e);
};