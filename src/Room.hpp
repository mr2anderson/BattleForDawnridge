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
#include <boost/optional.hpp>
#include "RemotePlayers.hpp"
#include "Timer.hpp"
#include "RoomID.hpp"
#include "ResourceBar.hpp"
#include "Map.hpp"
#include "PopUpElement.hpp"
#include "SuspendingAnimation.hpp"
#include "HighlightTable.hpp"
#include "BuildingMode.hpp"
#include "Button.hpp"
#include "AddResourceEvent.hpp"
#include "SubResourceEvent.hpp"
#include "AddResourcesEvent.hpp"
#include "SubResourcesEvent.hpp"
#include "AddHpEvent.hpp"
#include "DecreaseCurrentTradeMovesLeftEvent.hpp"
#include "BuildEvent.hpp"
#include "PlaySoundEvent.hpp"
#include "CreateEEvent.hpp"
#include "ChangeMoveEvent.hpp"
#include "ReturnToMenuEvent.hpp"
#include "DestroyEvent.hpp"
#include "DecreaseCurrentProducingMovesLeftEvent.hpp"
#include "WarriorProducingFinishedEvent.hpp"
#include "SelectEvent.hpp"
#include "UnselectEvent.hpp"
#include "StartWarriorAnimationEvent.hpp"
#include "RefreshMovementPointsEvent.hpp"
#include "EnableCursorEvent.hpp"
#include "DisableCursorEvent.hpp"
#include "CreateAnimationEvent.hpp"
#include "DecreaseBurningMovesLeftEvent.hpp"
#include "SubHpEvent.hpp"
#include "SetFireEvent.hpp"
#include "ChangeWarriorDirectionEvent.hpp"
#include "FocusOnEvent.hpp"
#include "ResetHighlightEvent.hpp"
#include "DoTradeEvent.hpp"
#include "StartWarriorProducingEvent.hpp"
#include "TryToBuildEvent.hpp"
#include "KillNextTurnEvent.hpp"
#include "RevertKillNextTurnEvent.hpp"
#include "CloseAnimationEvent.hpp"
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


#pragma once


class Room {
public:
	Room(const std::string &mapName); // TODO save system support
	Room(const Room& copy) = delete;

	RoomID getID() const;

	uint32_t playersNumber();

	void update(const boost::optional<std::tuple<sf::Packet, sf::IpAddress>> &received, std::vector<sf::Packet> *toSend, const RemotePlayers& remotePlayers);
private:
	RoomID id;

	Timer sendOKTimer;
	Timer sendWorldUIStateTimer;

	Map map;
	std::vector<bool> playerIsActive;
	uint32_t currentPlayerId;
	std::shared_ptr<PopUpElement> element;
	boost::optional<SuspendingAnimation> animation;
	std::queue<std::shared_ptr<Event>> events;
	uint32_t move;
	HighlightTable highlightTable;
	BuildingMode bm;
	uint32_t currentGOIndexNewMoveEvent;
	uint32_t totalGONewMoveEvents;
	ISelectable* selected;
	bool curcorVisibility;
	std::vector<Button> buttons;

	void processNewMoveEvents();
	bool allNewMoveEventsAdded() const;
	void changeMove();
	Player* getCurrentPlayer();
	void addButtonClickEventToQueue();
	void addGameObjectClickEventToQueue(uint8_t button);
	void processBaseEvents();
	void addEvents(Events& e);

	void sendEverythingToClients(std::vector<sf::Packet>* toSend, const RemotePlayers& remotePlayers);

	void sendOKToClients(std::vector<sf::Packet>* toSend, const RemotePlayers& remotePlayers);

	std::vector<std::shared_ptr<const RectangularUiElement>> makeButtonBases();
	ResourceBar makeResourceBar();
    void sendWorldUIStateToClients(std::vector<sf::Packet>* toSend, const RemotePlayers &remotePlayers);

	void sendToClients(const sf::Packet& what, std::vector<sf::Packet>* toSend, const RemotePlayers& remotePlayers);

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
	void handleRefreshAttackAbilityEvent(std::shared_ptr<RefreshAttackAbilityEvent> e);
	void handleWipeAttackAbilityEvent(std::shared_ptr<WipeAttackAbilityEvent> e);
	void handleRefreshAttackedTableEvent(std::shared_ptr<RefreshAttackedTableEvent> e);
	void handleMarkAsAttackedEvent(std::shared_ptr<MarkAsAttackedEvent> e);
	void handleRefreshHealingAbilityEvent(std::shared_ptr<RefreshHealingAbilityEvent> e);
	void handleWipeHealingAbilityEvent(std::shared_ptr<WipeHealingAbilityEvent> e);
	void handleMarkPlayerAsInactiveEvent(std::shared_ptr<MarkPlayerAsInactiveEvent> e);
	void handleIncreaseVCSMoveCtrEvent(std::shared_ptr<IncreaseVCSMoveCtrEvent> e);
	void handleSaveGameEvent(std::shared_ptr<SaveGameEvent> e);
	void handleLimitResourcesEvent(std::shared_ptr<LimitResourcesEvent> e);
};