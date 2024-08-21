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
#include "ServerNetSpecs.hpp"
#include "RoomOutputProtocol.hpp"


#pragma once


class Room {
public:
	typedef enum Restrictions {
		Enable,
		Disable
	} Restrictions;

	Room(RoomID id, const std::string &saveData, Restrictions restrictions);
	Room(const Room& copy) = delete;

	RoomID getID() const;

	uint32_t playersNumber() const;

	void update(const boost::optional<std::tuple<sf::Packet, sf::IpAddress>> &received, RoomOutputProtocol p);
    void needInit();
private:
	RoomID id;

    bool requireInit;

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

	std::string getSaveData() const;
	void loadSaveData(const std::string& data, Restrictions restrictions);

	void verifyLoadedData(Restrictions restrictions);
	void verifyMap();
	void verifyIncorrectMoveRepresentation();
	void verifyIncorrectPlayersRepresentation();
	void verifyTooMuchGameObjects();
	void verifyTooMuchPlayers();
	void verifyMapTooBig();

	void processNewMoveEvents(RoomOutputProtocol p);
	bool allNewMoveEventsAdded() const;
	void changeMove();
	Player* getCurrentPlayer();
	void addButtonClickEventToQueue(uint32_t x, uint32_t y, RoomOutputProtocol p);
	void addGameObjectClickEventToQueue(uint8_t button, uint32_t viewX, uint32_t viewY, RoomOutputProtocol p);
	void processBaseEvents(RoomOutputProtocol p, bool sendToClients = true);
	void addEvents(Events& e, RoomOutputProtocol p);

	std::vector<std::shared_ptr<const RectangularUiElement>> makeButtonBases();
	ResourceBar makeResourceBar();
    void sendWorldUIStateToClients(RoomOutputProtocol p);
	void sendPlaySoundEventToClients(RoomOutputProtocol p, const std::string& soundName);
	void sendSaveToClient(RoomOutputProtocol p);
	void sendReturnToMenuToClient(RoomOutputProtocol p);
	void sendReturnToMenuToClients(RoomOutputProtocol p);
	void sendFocusOnToClients(RoomOutputProtocol p, uint32_t x, uint32_t y, uint32_t sx, uint32_t sy);
	void sendNotYourMove(RoomOutputProtocol p, sf::IpAddress ip);

    sf::Packet makeBasePacket() const;
	void sendToClients(const sf::Packet& what, RoomOutputProtocol p);
    void sendToClient(const sf::Packet &what, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const sf::IpAddress &host);

	void receive(const boost::optional<std::tuple<sf::Packet, sf::IpAddress>>& received, RoomOutputProtocol p);
	void receiveClick(sf::Packet& remPacket, const sf::IpAddress &ip, RoomOutputProtocol p);

	void handleEvent(std::shared_ptr<Event> e, RoomOutputProtocol p);
	void handleAddResourceEvent(std::shared_ptr<AddResourceEvent> e, RoomOutputProtocol p);
	void handleSubResourceEvent(std::shared_ptr<SubResourceEvent> e, RoomOutputProtocol p);
	void handleAddResourcesEvent(std::shared_ptr<AddResourcesEvent> e, RoomOutputProtocol p);
	void handleSubResourcesEvent(std::shared_ptr<SubResourcesEvent> e, RoomOutputProtocol p);
	void handleSetHighlightEvent(std::shared_ptr<SetHighlightEvent> e, RoomOutputProtocol p);
	void handleAddHpEvent(std::shared_ptr<AddHpEvent> e, RoomOutputProtocol p);
	void handleDecreaseCurrentTradeMovesLeft(std::shared_ptr<DecreaseCurrentTradeMovesLeftEvent> e, RoomOutputProtocol p);
	void handleBuild(std::shared_ptr<BuildEvent> e, RoomOutputProtocol p);
	void handlePlaySoundEvent(std::shared_ptr<PlaySoundEvent> e, RoomOutputProtocol p);
	void handleCreatePopUpElementEvent(std::shared_ptr<CreateEEvent> e, RoomOutputProtocol p);
	void handleChangeMoveEvent(std::shared_ptr<ChangeMoveEvent> e, RoomOutputProtocol p);
	void handleReturnToMenuEvent(std::shared_ptr<ReturnToMenuEvent> e, RoomOutputProtocol p);
	void handleDestroyEvent(std::shared_ptr<DestroyEvent> e, RoomOutputProtocol p);
	void handleDecreaseCurrentProdusingMovesLeftEvent(std::shared_ptr<DecreaseCurrentProducingMovesLeftEvent> e, RoomOutputProtocol p);
	void handleWarriorProducingFinishedEvent(std::shared_ptr<WarriorProducingFinishedEvent> e, RoomOutputProtocol p);
	void handleSelectEvent(std::shared_ptr<SelectEvent> w, RoomOutputProtocol p);
	void handleUnselectEvent(std::shared_ptr<UnselectEvent> w, RoomOutputProtocol p);
	void handleStartWarriorAnimationEvent(std::shared_ptr<StartWarriorAnimationEvent> e, RoomOutputProtocol p);
	void handleRefreshMovementPointsEvent(std::shared_ptr<RefreshMovementPointsEvent> e, RoomOutputProtocol p);
	void handleEnableCursorEvent(std::shared_ptr<EnableCursorEvent> e, RoomOutputProtocol p);
	void handleDisableCursorEvent(std::shared_ptr<DisableCursorEvent> e, RoomOutputProtocol p);
	void handleCreateAnimationEvent(std::shared_ptr<CreateAnimationEvent> e, RoomOutputProtocol p);
	void handleDecreaseBurningMovesLeftEvent(std::shared_ptr<DecreaseBurningMovesLeftEvent> e, RoomOutputProtocol p);
	void handleSubHpEvent(std::shared_ptr<SubHpEvent> e, RoomOutputProtocol p);
	void handleSetFireEvent(std::shared_ptr<SetFireEvent> e, RoomOutputProtocol p);
	void handleChangeWarriorDirectionEvent(std::shared_ptr<ChangeWarriorDirectionEvent> e, RoomOutputProtocol p);
	void handleFocusOnEvent(std::shared_ptr<FocusOnEvent> e, RoomOutputProtocol p);
	void handleResetHighlightEvent(std::shared_ptr<ResetHighlightEvent> e, RoomOutputProtocol p);
	void handleDoTradeEvent(std::shared_ptr<DoTradeEvent> e, RoomOutputProtocol p);
	void handleStartWarriorProducingEvent(std::shared_ptr<StartWarriorProducingEvent> e, RoomOutputProtocol p);
	void handleTryToBuildEvent(std::shared_ptr<TryToBuildEvent> e, RoomOutputProtocol p);
	void handleKillNextTurnEvent(std::shared_ptr<KillNextTurnEvent> e, RoomOutputProtocol p);
	void handleRevertKillNextTurnEvent(std::shared_ptr<RevertKillNextTurnEvent> e, RoomOutputProtocol p);
	void handleCloseAnimationEvent(std::shared_ptr<CloseAnimationEvent> e, RoomOutputProtocol p);
	void handleDecreaseSpellCreationMovesLeftEvent(std::shared_ptr<DecreaseSpellCreationMovesLeftEvent> e, RoomOutputProtocol p);
	void handleSetSpellEvent(std::shared_ptr<SetSpellEvent> e, RoomOutputProtocol p);
	void handleUseSpellEvent(std::shared_ptr<UseSpellEvent> e, RoomOutputProtocol p);
	void handleMarkSpellAsUsedEvent(std::shared_ptr<MarkSpellAsUsedEvent> e, RoomOutputProtocol p);
	void handleEnableWarriorRageModeEvent(std::shared_ptr<EnableWarriorRageModeEvent> e, RoomOutputProtocol p);
	void handleDecreaseRageModeMovesLeftEvent(std::shared_ptr<DecreaseRageModeMovesLeftEvent> e, RoomOutputProtocol p);
	void handleRefreshAttackAbilityEvent(std::shared_ptr<RefreshAttackAbilityEvent> e, RoomOutputProtocol p);
	void handleWipeAttackAbilityEvent(std::shared_ptr<WipeAttackAbilityEvent> e, RoomOutputProtocol p);
	void handleRefreshAttackedTableEvent(std::shared_ptr<RefreshAttackedTableEvent> e, RoomOutputProtocol p);
	void handleMarkAsAttackedEvent(std::shared_ptr<MarkAsAttackedEvent> e, RoomOutputProtocol p);
	void handleRefreshHealingAbilityEvent(std::shared_ptr<RefreshHealingAbilityEvent> e, RoomOutputProtocol p);
	void handleWipeHealingAbilityEvent(std::shared_ptr<WipeHealingAbilityEvent> e, RoomOutputProtocol p);
	void handleMarkPlayerAsInactiveEvent(std::shared_ptr<MarkPlayerAsInactiveEvent> e, RoomOutputProtocol p);
	void handleIncreaseVCSMoveCtrEvent(std::shared_ptr<IncreaseVCSMoveCtrEvent> e, RoomOutputProtocol p);
	void handleSaveGameEvent(std::shared_ptr<SaveGameEvent> e, RoomOutputProtocol p);
	void handleLimitResourcesEvent(std::shared_ptr<LimitResourcesEvent> e, RoomOutputProtocol p);
};