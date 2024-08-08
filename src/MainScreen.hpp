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
#include "RefreshAttackAbilityEvent.hpp"
#include "WipeAttackAbilityEvent.hpp"
#include "RefreshAttackedTableEvent.hpp"
#include "MarkAsAttackedEvent.hpp"
#include "RefreshHealingAbilityEvent.hpp"
#include "WipeHealingAbilityEvent.hpp"
#include "MarkPlayerAsInactiveEvent.hpp"
#include "IncreaseVCSMoveCtrEvent.hpp"
#include "SaveGameEvent.hpp"
#include "LimitResourcesEvent.hpp"
#include "IlluminanceTable.hpp"
#include "MenuResponse.hpp"
#include "MainScreenResponse.hpp"


#pragma once


class MainScreen {
public:
	MainScreen(sf::RenderWindow &window, const MenuResponse &response);
	MainScreen(const MainScreen& copy) = delete;
	MainScreenResponse run(sf::RenderWindow& window);
private:
	bool alreadyFinished;
    std::shared_ptr<Map> map;
	std::vector<bool> playerIsActive;
	uint32_t currentPlayerId;
	std::shared_ptr<PopUpElement> element;
	std::shared_ptr<IlluminanceTable> illiminanceTable;
    std::optional<SuspendingAnimation> animation;
    std::queue<std::shared_ptr<Event>> events;
	std::queue<std::tuple<uint32_t, uint32_t>> viewMovingQueue;
	uint32_t move;
	HighlightTable highlightTable;
    BuildingMode bm;
	uint32_t currentGOIndexNewMoveEvent;
	uint32_t totalGONewMoveEvents;
	ISelectable* selected;
    bool returnToMenu;
	bool curcorVisibility;
	sf::View view;
    std::vector<Button> buttons;


    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive & ar, const unsigned int version) {
        ar & this->map;
        ar & this->playerIsActive;
        ar & this->currentPlayerId;
        ar & this->move;
    }
    void save() const;


	void drawEverything(sf::RenderWindow& window);
	void drawMap(sf::RenderWindow& window);
	void drawResourceBar(sf::RenderWindow& window);
	void drawCells(sf::RenderWindow& window);
	void drawHighlightion(sf::RenderWindow& window);
    void drawDarkness(sf::RenderWindow& window);


	void removeFinishedElement();
	void processNewMoveEvents(sf::RenderWindow& window);
	bool allNewMoveEventsAdded() const;
	void changeMove();
	Player* getCurrentPlayer();
	void addButtonClickEventToQueue(sf::RenderWindow& window);
	void addGameObjectClickEventToQueue(uint8_t button, sf::RenderWindow& window);
    void processBaseEvents(sf::RenderWindow& window);
    void addEvents(Events &e, sf::RenderWindow& window);
	std::tuple<uint32_t, uint32_t> getMousePositionBasedOnView(sf::RenderWindow &window) const;
	void moveView(sf::RenderWindow &window);


	bool moveViewToNorth(uint32_t border, sf::RenderWindow& window);
	bool moveViewToNorth(sf::RenderWindow& window);
	bool moveViewToSouth(uint32_t border, sf::RenderWindow& window);
	bool moveViewToSouth(sf::RenderWindow& window);
	bool moveViewToWest(uint32_t borde, sf::RenderWindow& window);
	bool moveViewToWest(sf::RenderWindow& window);
	bool moveViewToEast(uint32_t border, sf::RenderWindow& window);
	bool moveViewToEast(sf::RenderWindow& window);

	bool verifyViewNorth(uint32_t border);
	bool verifyViewNorth(sf::RenderWindow& window);
	bool verifyViewSouth(uint32_t border);
	bool verifyViewSouth(sf::RenderWindow& window);
	bool verifyViewWest(uint32_t border);
	bool verifyViewWest(sf::RenderWindow& window);
	bool verifyViewEast(uint32_t border);
	bool verifyViewEast(sf::RenderWindow& window);


	void handleEvent(std::shared_ptr<Event> e, sf::RenderWindow& window);
	void handleAddResourceEvent(std::shared_ptr<AddResourceEvent> e);
	void handleSubResourceEvent(std::shared_ptr<SubResourceEvent> e);
	void handleAddResourcesEvent(std::shared_ptr<AddResourcesEvent> e);
	void handleSubResourcesEvent(std::shared_ptr<SubResourcesEvent> e);
	void handleSetHighlightEvent(std::shared_ptr<SetHighlightEvent> e);
	void handleAddHpEvent(std::shared_ptr<AddHpEvent> e);
	void handleDecreaseCurrentTradeMovesLeft(std::shared_ptr<DecreaseCurrentTradeMovesLeftEvent> e);
	void handleBuild(std::shared_ptr<BuildEvent> e);
	void handlePlaySoundEvent(std::shared_ptr<PlaySoundEvent> e);
	void handleCreatePopUpElementEvent(std::shared_ptr<CreateEEvent> e, sf::RenderWindow& window);
	void handleChangeMoveEvent(std::shared_ptr<ChangeMoveEvent> e);
	void handleReturnToMenuEvent(std::shared_ptr<ReturnToMenuEvent> e);
	void handleDestroyEvent(std::shared_ptr<DestroyEvent> e, sf::RenderWindow& window);
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
	void handleDoTradeEvent(std::shared_ptr<DoTradeEvent> e, sf::RenderWindow& window);
	void handleStartWarriorProducingEvent(std::shared_ptr<StartWarriorProducingEvent> e, sf::RenderWindow& window);
	void handleTryToBuildEvent(std::shared_ptr<TryToBuildEvent> e, sf::RenderWindow& window);
	void handleKillNextTurnEvent(std::shared_ptr<KillNextTurnEvent> e, sf::RenderWindow& window);
	void handleRevertKillNextTurnEvent(std::shared_ptr<RevertKillNextTurnEvent> e, sf::RenderWindow& window);
	void handleCloseAnimationEvent(std::shared_ptr<CloseAnimationEvent> e);
	void handleDecreaseSpellCreationMovesLeftEvent(std::shared_ptr<DecreaseSpellCreationMovesLeftEvent> e);
	void handleSetSpellEvent(std::shared_ptr<SetSpellEvent> e);
	void handleUseSpellEvent(std::shared_ptr<UseSpellEvent> e, sf::RenderWindow& window);
	void handleMarkSpellAsUsedEvent(std::shared_ptr<MarkSpellAsUsedEvent> e);
	void handleEnableWarriorRageModeEvent(std::shared_ptr<EnableWarriorRageModeEvent> e);
	void handleDecreaseRageModeMovesLeftEvent(std::shared_ptr<DecreaseRageModeMovesLeftEvent> e);
    void handleRefreshAttackAbilityEvent(std::shared_ptr<RefreshAttackAbilityEvent> e);
    void handleWipeAttackAbilityEvent(std::shared_ptr<WipeAttackAbilityEvent> e);
    void handleRefreshAttackedTableEvent(std::shared_ptr<RefreshAttackedTableEvent> e);
    void handleMarkAsAttackedEvent(std::shared_ptr<MarkAsAttackedEvent> e);
    void handleRefreshHealingAbilityEvent(std::shared_ptr<RefreshHealingAbilityEvent> e);
    void handleWipeHealingAbilityEvent(std::shared_ptr<WipeHealingAbilityEvent> e);
    void handleMarkPlayerAsInactiveEvent(std::shared_ptr<MarkPlayerAsInactiveEvent> e, sf::RenderWindow& window);
    void handleIncreaseVCSMoveCtrEvent(std::shared_ptr<IncreaseVCSMoveCtrEvent> e);
    void handleSaveGameEvent(std::shared_ptr<SaveGameEvent> e);
    void handleLimitResourcesEvent(std::shared_ptr<LimitResourcesEvent> e);
};