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

	void update(const boost::optional<std::tuple<sf::Packet, sf::IpAddress>> &received, std::vector<std::tuple<sf::Packet, sf::IpAddress>> *toSend, const RemotePlayers& remotePlayers);
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

	void processNewMoveEvents(std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	bool allNewMoveEventsAdded() const;
	void changeMove();
	Player* getCurrentPlayer();
	void addButtonClickEventToQueue(uint32_t x, uint32_t y, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void addGameObjectClickEventToQueue(uint8_t button, uint32_t viewX, uint32_t viewY, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void processBaseEvents(std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers, bool sendToClients = true);
	void addEvents(Events& e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);

	std::vector<std::shared_ptr<const RectangularUiElement>> makeButtonBases();
	ResourceBar makeResourceBar();
    void sendWorldUIStateToClients(std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers &remotePlayers);

    sf::Packet makeBasePacket() const;
	void sendToClients(const sf::Packet& what, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
    void sendToClient(const sf::Packet &what,  std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const sf::IpAddress &host);

	void receive(const boost::optional<std::tuple<sf::Packet, sf::IpAddress>>& received, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void receiveClick(sf::Packet& remPacket, const sf::IpAddress &ip, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);

	void handleEvent(std::shared_ptr<Event> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleAddResourceEvent(std::shared_ptr<AddResourceEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleSubResourceEvent(std::shared_ptr<SubResourceEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleAddResourcesEvent(std::shared_ptr<AddResourcesEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleSubResourcesEvent(std::shared_ptr<SubResourcesEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleSetHighlightEvent(std::shared_ptr<SetHighlightEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleAddHpEvent(std::shared_ptr<AddHpEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleDecreaseCurrentTradeMovesLeft(std::shared_ptr<DecreaseCurrentTradeMovesLeftEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleBuild(std::shared_ptr<BuildEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handlePlaySoundEvent(std::shared_ptr<PlaySoundEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleCreatePopUpElementEvent(std::shared_ptr<CreateEEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleChangeMoveEvent(std::shared_ptr<ChangeMoveEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleReturnToMenuEvent(std::shared_ptr<ReturnToMenuEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleDestroyEvent(std::shared_ptr<DestroyEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleDecreaseCurrentProdusingMovesLeftEvent(std::shared_ptr<DecreaseCurrentProducingMovesLeftEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleWarriorProducingFinishedEvent(std::shared_ptr<WarriorProducingFinishedEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleSelectEvent(std::shared_ptr<SelectEvent> w, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleUnselectEvent(std::shared_ptr<UnselectEvent> w, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleStartWarriorAnimationEvent(std::shared_ptr<StartWarriorAnimationEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleRefreshMovementPointsEvent(std::shared_ptr<RefreshMovementPointsEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleEnableCursorEvent(std::shared_ptr<EnableCursorEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleDisableCursorEvent(std::shared_ptr<DisableCursorEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleCreateAnimationEvent(std::shared_ptr<CreateAnimationEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleDecreaseBurningMovesLeftEvent(std::shared_ptr<DecreaseBurningMovesLeftEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleSubHpEvent(std::shared_ptr<SubHpEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleSetFireEvent(std::shared_ptr<SetFireEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleChangeWarriorDirectionEvent(std::shared_ptr<ChangeWarriorDirectionEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleFocusOnEvent(std::shared_ptr<FocusOnEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleResetHighlightEvent(std::shared_ptr<ResetHighlightEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleDoTradeEvent(std::shared_ptr<DoTradeEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleStartWarriorProducingEvent(std::shared_ptr<StartWarriorProducingEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleTryToBuildEvent(std::shared_ptr<TryToBuildEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleKillNextTurnEvent(std::shared_ptr<KillNextTurnEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleRevertKillNextTurnEvent(std::shared_ptr<RevertKillNextTurnEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleCloseAnimationEvent(std::shared_ptr<CloseAnimationEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleDecreaseSpellCreationMovesLeftEvent(std::shared_ptr<DecreaseSpellCreationMovesLeftEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleSetSpellEvent(std::shared_ptr<SetSpellEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleUseSpellEvent(std::shared_ptr<UseSpellEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleMarkSpellAsUsedEvent(std::shared_ptr<MarkSpellAsUsedEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleEnableWarriorRageModeEvent(std::shared_ptr<EnableWarriorRageModeEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleDecreaseRageModeMovesLeftEvent(std::shared_ptr<DecreaseRageModeMovesLeftEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleRefreshAttackAbilityEvent(std::shared_ptr<RefreshAttackAbilityEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleWipeAttackAbilityEvent(std::shared_ptr<WipeAttackAbilityEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleRefreshAttackedTableEvent(std::shared_ptr<RefreshAttackedTableEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleMarkAsAttackedEvent(std::shared_ptr<MarkAsAttackedEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleRefreshHealingAbilityEvent(std::shared_ptr<RefreshHealingAbilityEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleWipeHealingAbilityEvent(std::shared_ptr<WipeHealingAbilityEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleMarkPlayerAsInactiveEvent(std::shared_ptr<MarkPlayerAsInactiveEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleIncreaseVCSMoveCtrEvent(std::shared_ptr<IncreaseVCSMoveCtrEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleSaveGameEvent(std::shared_ptr<SaveGameEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
	void handleLimitResourcesEvent(std::shared_ptr<LimitResourcesEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers);
};