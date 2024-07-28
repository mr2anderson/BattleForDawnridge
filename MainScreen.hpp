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
#include "BuildEvent.hpp"
#include "ChangeMoveEvent.hpp"
#include "ReturnToMenuEvent.hpp"
#include "SubResourcesEvent.hpp"
#include "SetHighlightEvent.hpp"
#include "AddResourceEvent.hpp"
#include "SubResourceEvent.hpp"
#include "DecreaseCurrentTradeMovesLeftEvent.hpp"
#include "DecreaseCurrentProducingMovesLeftEvent.hpp"
#include "WarriorProducingFinishedEvent.hpp"
#include "SelectEvent.hpp"
#include "UnselectEvent.hpp"
#include "StartWarriorAnimationEvent.hpp"
#include "PlaySoundEvent.hpp"
#include "VictoryConditionBDestroyedEvent.hpp"
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
#include "BuildingMode.hpp"
#include "FocusOnEvent.hpp"
#include "ResetHighlightEvent.hpp"
#include "Button.hpp"
#include "DoTradeEvent.hpp"
#include "StartWarriorProducingEvent.hpp"
#include "TryToBuildEvent.hpp"
#include "KillNextTurnEvent.hpp"
#include "RevertKillNextTurnEvent.hpp"
#include "DecreaseSpellCreationMovesLeftEvent.hpp"
#include "SetSpellEvent.hpp"
#include "UseSpellEvent.hpp"
#include "MarkSpellAsUsedEvent.hpp"
#include "EnableWarriorRageModeEvent.hpp"
#include "DecreaseRageModeMovesLeftEvent.hpp"
#include "CreateEffectEvent.hpp"
#include "RefreshAttackAbilityEvent.hpp"
#include "WipeAttackAbilityEvent.hpp"
#include "RefreshAttackedTableEvent.hpp"
#include "MarkAsAttackedEvent.hpp"
#include "RefreshHealingAbilityEvent.hpp"
#include "WipeHealingAbilityEvent.hpp"


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
	uint32_t currentPlayerId;
	std::shared_ptr<PopUpElement> element;
    std::optional<SuspendingAnimation> animation;
    std::queue<std::shared_ptr<Event>> events;
	std::queue<std::tuple<uint32_t, uint32_t>> viewMovingQueue;
	uint32_t move;
	HighlightTable highlightTable;
	PlainsGeneration plains;
    BuildingMode bm;
	uint32_t currentGOIndexNewMoveEvent;
	uint32_t totalGONewMoveEvents;
	ISelectable* selected;

    bool returnToMenu;
	bool curcorVisibility;
	uint32_t windowW, windowH;
	std::shared_ptr<sf::View> view;
    std::vector<Button> buttons;

	void init(std::shared_ptr<Map> mapPtr, sf::RenderWindow &window);
	void initMap(std::shared_ptr<Map> mapPtr);
	void initPlayerIsActiveTable();
	void initCurrentPlayerId();
	void initMoveCtr();
	void initSelectable();
	void initGraphics(sf::RenderWindow &window);


	void drawEverything(sf::RenderWindow& window);
	void drawResourceBar(sf::RenderWindow& window);
	void drawCells(sf::RenderWindow& window);
	void drawHighlightion(sf::RenderWindow& window);
    void drawDarkness(sf::RenderWindow& window);


	void removeFinishedElement();
	void addNewMoveEvent();
	bool allNewMoveEventsAdded() const;
	void changeMove();
	Player* getCurrentPlayer();
	void addButtonClickEventToQueue();
	void addGameObjectClickEventToQueue(uint8_t button);
    void processBaseEvents();
    void addEvents(Events &e);
	void prepareToReturnToMenu(sf::RenderWindow& window);
	std::tuple<uint32_t, uint32_t> getMousePositionBasedOnView() const;
	void moveView();


	bool moveViewToNorth(uint32_t border);
	bool moveViewToNorth();
	bool moveViewToSouth(uint32_t border);
	bool moveViewToSouth();
	bool moveViewToWest(uint32_t border);
	bool moveViewToWest();
	bool moveViewToEast(uint32_t border);
	bool moveViewToEast();

	bool verifyViewNorth(uint32_t border);
	bool verifyViewNorth();
	bool verifyViewSouth(uint32_t border);
	bool verifyViewSouth();
	bool verifyViewWest(uint32_t border);
	bool verifyViewWest();
	bool verifyViewEast(uint32_t border);
	bool verifyViewEast();


	void handleEvent(std::shared_ptr<Event> e);
	void handleAddResourceEvent(std::shared_ptr<AddResourceEvent> e);
	void handleSubResourceEvent(std::shared_ptr<SubResourceEvent> e);
	void handleAddResourcesEvent(std::shared_ptr<AddResourcesEvent> e);
	void handleSubResourcesEvent(std::shared_ptr<SubResourcesEvent> e);
	void handleSetHighlightEvent(std::shared_ptr<SetHighlightEvent> e);
	void handleAddHpEvent(std::shared_ptr<AddHpEvent> e);
	void handleDecreaseCurrentTradeMovesLeft(std::shared_ptr<DecreaseCurrentTradeMovesLeftEvent> e);
	void handleBuild(std::shared_ptr<BuildEvent> e);
	void handlePlaySoundEvent(std::shared_ptr<PlaySoundEvent> e);
	void handleCreatePopUpElementEvent(std::shared_ptr<CreateEEvent> e);
	void handleChangeMoveEvent(std::shared_ptr<ChangeMoveEvent> e);
	void handleReturnToMenuEvent(std::shared_ptr<ReturnToMenuEvent> e);
	void handleDestroyEvent(std::shared_ptr<DestroyEvent> e);
	void handleVictoryConditionBDestroyedEvent(std::shared_ptr<VictoryConditionBDestroyedEvent> e);
	void handleDecreaseCurrentProdusingMovesLeftEvent(std::shared_ptr<DecreaseCurrentProducingMovesLeftEvent> e);
	void handleWarriorProducingFinishedEvent(std::shared_ptr<WarriorProducingFinishedEvent> e);
	void handleSelectEvent(std::shared_ptr<SelectEvent> w);
	void handleUnselectEvent(std::shared_ptr<UnselectEvent> w);
	void handleStartWarriorAnimationEvent(std::shared_ptr<StartWarriorAnimationEvent> e);
	void handleRefreshMovementPointsEvent(std::shared_ptr<RefreshMovementPointsEvent> e);
	void handleEnableCursorEvent(std::shared_ptr<EnableCursorEvent> e);
	void handleDisableCursorEvent(std::shared_ptr<DisableCursorEvent> e);
    void handleCreateAnimationEvent(std::shared_ptr<CreateAnimationEvent> e);
	void handleDecreaseBurningMovesLeftEvent(std::shared_ptr<DecreaseBurningMovesLeftEvent> e);
	void handleSubHpEvent(std::shared_ptr<SubHpEvent> e);
	void handleSetFireEvent(std::shared_ptr<SetFireEvent> e);
	void handleChangeWarriorDirectionEvent(std::shared_ptr<ChangeWarriorDirectionEvent> e);
	void handleFocusOnEvent(std::shared_ptr<FocusOnEvent> e);
	void handleResetHighlightEvent(std::shared_ptr<ResetHighlightEvent> e);
	void handleDoTradeEvent(std::shared_ptr<DoTradeEvent> e);
	void handleStartWarriorProducingEvent(std::shared_ptr<StartWarriorProducingEvent> e);
	void handleTryToBuildEvent(std::shared_ptr<TryToBuildEvent> e);
	void handleKillNextTurnEvent(std::shared_ptr<KillNextTurnEvent> e);
	void handleRevertKillNextTurnEvent(std::shared_ptr<RevertKillNextTurnEvent> e);
	void handleCloseAnimationEvent(std::shared_ptr<CloseAnimationEvent> e);
	void handleDecreaseSpellCreationMovesLeftEvent(std::shared_ptr<DecreaseSpellCreationMovesLeftEvent> e);
	void handleSetSpellEvent(std::shared_ptr<SetSpellEvent> e);
	void handleUseSpellEvent(std::shared_ptr<UseSpellEvent> e);
	void handleMarkSpellAsUsedEvent(std::shared_ptr<MarkSpellAsUsedEvent> e);
	void handleEnableWarriorRageModeEvent(std::shared_ptr<EnableWarriorRageModeEvent> e);
	void handleDecreaseRageModeMovesLeftEvent(std::shared_ptr<DecreaseRageModeMovesLeftEvent> e);
	void handleCreateEffectEvent(std::shared_ptr<CreateEffectEvent> e);
    void handleRefreshAttackAbilityEvent(std::shared_ptr<RefreshAttackAbilityEvent> e);
    void handleWipeAttackAbilityEvent(std::shared_ptr<WipeAttackAbilityEvent> e);
    void handleRefreshAttackedTableEvent(std::shared_ptr<RefreshAttackedTableEvent> e);
    void handleMarkAsAttackedEvent(std::shared_ptr<MarkAsAttackedEvent> e);
    void handleRefreshHealingAbilityEvent(std::shared_ptr<RefreshHealingAbilityEvent> e);
    void handleWipeHealingAbilityEvent(std::shared_ptr<WipeHealingAbilityEvent> e);
};