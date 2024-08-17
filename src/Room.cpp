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


#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <iostream>
#include <sstream>
#include "Room.hpp"
#include "ReturnToMenuButtonSpec.hpp"
#include "SaveGameButtonSpec.hpp"
#include "EndTurnButtonSpec.hpp"
#include "BuildButtonSpec.hpp"
#include "Building.hpp"
#include "Warrior.hpp"
#include "VictoryConditionSpec.hpp"
#include "WindowButton.hpp"
#include "TradingSpec.hpp"
#include "WarriorProducerSpec.hpp"
#include "SpellProducerSpec.hpp"
#include "Spell.hpp"
#include "WarriorHealer.hpp"
#include "WarriorNearMultyAttacker.hpp"
#include "ISingleAttacker.hpp"
#include "ResourceBar.hpp"
#include "ServerNetSpecs.hpp"
#include "ClientNetSpecs.hpp"
#include "RoomWasClosed.hpp"
#include "PackageLimit.hpp"
#include "IncorrectMoveRepresentation.hpp"
#include "IncorrectPlayersRepresentation.hpp"
#include "MapTooBig.hpp"
#include "TooMuchGameObjects.hpp"
#include "TooMuchPlayers.hpp"
#include "MapDeepError.hpp"
#include "CurrentRoomIDButtonSpec.hpp"
#include "GlobalRandomGenerator64.hpp"






Room::Room(RoomID id, const std::string &saveData, Restrictions restrictions) {
	this->id = id;

	this->sendOKTimer = Timer(1000, Timer::TYPE::FIRST_INSTANTLY);
	this->noOKReceivedTimer = Timer(60 * 1000, Timer::TYPE::FIRST_DEFAULT);
    this->requireInit = true;

	this->loadSaveData(saveData, restrictions);

	this->curcorVisibility = true;
	this->element = nullptr;
	this->selected = nullptr;
	this->animation = boost::none;
	this->currentGOIndexNewMoveEvent = this->map.getStatePtr()->getCollectionsPtr()->totalGOs();
	this->totalGONewMoveEvents = this->map.getStatePtr()->getCollectionsPtr()->totalGOs();
	this->buttons.emplace_back(ReturnToMenuButtonSpec());
	this->buttons.emplace_back(SaveGameButtonSpec(1));
	this->buttons.emplace_back(EndTurnButtonSpec(2));
	this->buttons.emplace_back(BuildButtonSpec(3));
	this->buttons.emplace_back(CurrentRoomIDButtonSpec(4, this->id.readableValue()));
}





RoomID Room::getID() const {
	return this->id;
}






uint32_t Room::playersNumber() const {
	return this->map.getStatePtr()->getPlayersPtr()->total();
}







void Room::update(const boost::optional<std::tuple<sf::Packet, sf::IpAddress>>& received, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers &remotePlayers) {
	if (this->noOKReceivedTimer.ready()) {
		throw RoomWasClosed();
	}

    if (this->requireInit) {
        this->sendWorldUIStateToClients(toSend, remotePlayers, SERVER_NET_SPECS::Importance::ExtremelyImportant);
        this->requireInit = false;
    }

	this->sendTimeCommandsToClients(toSend, remotePlayers);
	this->receive(received, toSend, remotePlayers);

	if (this->element != nullptr) {
		this->element->update();
		if (this->element->finished()) {
			this->element = nullptr;
		}
	}
	if (this->animation.has_value()) {
		Events animationEvent;
		animationEvent = this->animation.value().process(this->map.getStatePtr());
		this->addEvents(animationEvent, toSend, remotePlayers);
	}
	this->processNewMoveEvents(toSend, remotePlayers);
	this->processBaseEvents(toSend, remotePlayers);
}
void Room::needInit() {
    this->requireInit = true;
}







std::string Room::getSaveData() const {
	std::string serialStr;

	boost::iostreams::back_insert_device<std::string> inserter(serialStr);
	boost::iostreams::stream<boost::iostreams::back_insert_device<std::string>> s(inserter);

	oarchive ar(s);
	ar << this->map << this->playerIsActive << this->currentPlayerId << this->move;

	s.flush();

	return serialStr;
}
void Room::loadSaveData(const std::string &data, Restrictions restrictions) {
	std::stringstream stream(data);
	iarchive ar(stream);
	ar >> this->map >> this->playerIsActive >> this->currentPlayerId >> this->move;
	this->verifyLoadedData(restrictions);
}








void Room::verifyLoadedData(Restrictions restrictions) {
	this->verifyMap();
	this->verifyIncorrectMoveRepresentation();
	this->verifyIncorrectPlayersRepresentation();
	if (restrictions == Restrictions::Enable) {
		this->verifyTooMuchGameObjects();
		this->verifyTooMuchPlayers();
		this->verifyMapTooBig();
	}
}
void Room::verifyMap() {
	if (this->map.hasError()) {
		throw MapDeepError();
	}
}
void Room::verifyIncorrectMoveRepresentation() {
	if (this->move == 0 or this->move > std::numeric_limits<uint32_t>::max() - 1000000) {
		throw IncorrectMoveRepresentation();
	}
}
void Room::verifyIncorrectPlayersRepresentation() {
	if (this->currentPlayerId == 0 or this->currentPlayerId > this->playerIsActive.size()) {
		throw IncorrectPlayersRepresentation();
	}
	
	uint32_t activePlayers = 0;
	for (uint32_t i = 0; i < this->playerIsActive.size(); i = i + 1) {
		activePlayers = activePlayers + this->playerIsActive.at(i);
	}
	if (activePlayers < 2) {
		throw IncorrectPlayersRepresentation();
	}

	for (uint32_t i = 0; i < this->playerIsActive.size(); i = i + 1) {
		if (this->playerIsActive.at(i)) {
			bool ok = false;
			for (uint32_t j = 0; j < this->map.getStatePtr()->getCollectionsPtr()->totalBuildings(); j = j + 1) {
				const Building* b = this->map.getStatePtr()->getCollectionsPtr()->getBuilding(j);
				if (b->exist() and b->getPlayerId() == i + 1 and b->isVictoryCondition()) {
					ok = true;
					break;
				}
			}
			if (!ok) {
				throw IncorrectPlayersRepresentation();
			}
		}
	}
}
void Room::verifyTooMuchGameObjects() {
	if (this->map.getStatePtr()->getCollectionsPtr()->totalGOs() > 5000) {
		throw TooMuchGameObjects();
	}
}
void Room::verifyTooMuchPlayers() {
	if (this->map.getStatePtr()->getPlayersPtr()->total() > 16) {
		throw TooMuchPlayers();
	}
}
void Room::verifyMapTooBig() {
	if (this->map.getStatePtr()->getMapSizePtr()->getWidth() > 1000 or
		this->map.getStatePtr()->getMapSizePtr()->getHeight() > 1000) {
		throw MapTooBig();
	}
}





void Room::processNewMoveEvents(std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	bool somethingProcessed = false;
	while (this->currentGOIndexNewMoveEvent != this->totalGONewMoveEvents) {
		if (this->element != nullptr or !this->events.empty()) {
			break;
		}
		Events newMoveEvent = this->map.getStatePtr()->getCollectionsPtr()->getGO(this->currentGOIndexNewMoveEvent, FILTER::NEW_MOVE_PRIORITY)->newMove(this->map.getStatePtr(), this->getCurrentPlayer()->getId());
		this->addEvents(newMoveEvent, toSend, remotePlayers);
		this->currentGOIndexNewMoveEvent = this->currentGOIndexNewMoveEvent + 1;
		this->processBaseEvents(toSend, remotePlayers, false);
		somethingProcessed = true;
	}
	if (somethingProcessed) {
		this->sendWorldUIStateToClients(toSend, remotePlayers, SERVER_NET_SPECS::Importance::Important);
	}
}
bool Room::allNewMoveEventsAdded() const {
	return (this->currentGOIndexNewMoveEvent == this->totalGONewMoveEvents);
}
void Room::changeMove() {
	this->move = this->move + 1;
	this->currentGOIndexNewMoveEvent = 0;
	this->totalGONewMoveEvents = this->map.getStatePtr()->getCollectionsPtr()->totalGOs();
	do {
		this->currentPlayerId = this->currentPlayerId + 1;
		if (this->currentPlayerId > this->map.getStatePtr()->getPlayersPtr()->total()) {
			this->currentPlayerId = 1;
		}
	} while (!this->playerIsActive.at(this->currentPlayerId - 1));
}
Player* Room::getCurrentPlayer() {
	return this->map.getStatePtr()->getPlayersPtr()->getPlayerPtr(this->currentPlayerId);
}
void Room::addButtonClickEventToQueue(uint32_t x, uint32_t y, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	for (uint32_t i = 0; i < this->buttons.size(); i = i + 1) {
		Events buttonClickEvent = this->buttons.at(i).click(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
		if (!buttonClickEvent.empty()) {
			this->addEvents(buttonClickEvent, toSend, remotePlayers);
			break;
		}
	}
}
void Room::addGameObjectClickEventToQueue(uint8_t button, uint32_t viewX, uint32_t viewY, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	for (uint32_t i = 0; i < this->map.getStatePtr()->getCollectionsPtr()->totalGOs(); i = i + 1) {
		GO* go = this->map.getStatePtr()->getCollectionsPtr()->getGO(i, FILTER::CLICK_PRIORITY);
		Events gor = go->click(this->map.getStatePtr(), this->getCurrentPlayer()->getId(), button, viewX, viewY);
		if (!gor.empty()) {
			this->addEvents(gor, toSend, remotePlayers);
			return;
		}
	}
}
void Room::processBaseEvents(std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers, bool sendToClients) {
	bool somethingProcessed = false;
	while (!this->events.empty()) {
		if (this->element != nullptr or this->animation.has_value()) {
			break;
		}
		this->handleEvent(this->events.front(), toSend, remotePlayers);
		this->events.pop();
		somethingProcessed = true;
	}
	if (sendToClients and somethingProcessed) {
		this->sendWorldUIStateToClients(toSend, remotePlayers, SERVER_NET_SPECS::Importance::Important);
	}
}
void Room::addEvents(Events& e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
    bool urgentHandled = false;

	for (uint32_t i = 0; i < e.size(); i = i + 1) {
		std::shared_ptr<Event> event = e.at(i);
		if (event->isUrgent()) {
			this->handleEvent(event, toSend, remotePlayers);
            urgentHandled = true;
		}
		else {
			this->events.push(event);
		}
	}

    if (urgentHandled) {
        this->sendWorldUIStateToClients(toSend, remotePlayers, SERVER_NET_SPECS::Importance::Important);
    }
}








void Room::sendTimeCommandsToClients(std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	if (this->sendOKTimer.ready()) {
		this->sendOKTimer.reset();
		this->sendOKToClients(toSend, remotePlayers);
	}
}
void Room::sendOKToClients(std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	sf::Packet packet = this->makeBasePacket();
	packet << SERVER_NET_SPECS::CODES::OK;

	this->sendToClients(packet, toSend, remotePlayers, SERVER_NET_SPECS::Importance::NotImportant);
}
std::vector<std::shared_ptr<const RectangularUiElement>> Room::makeButtonBases() {
	std::vector<std::shared_ptr<const RectangularUiElement>> bases(this->buttons.size());

	for (uint32_t i = 0; i < this->buttons.size(); i = i + 1) {
		bases.at(i) = this->buttons.at(i).getBase();
	}

	return bases;
}
ResourceBar Room::makeResourceBar() {
	ResourceBar bar;

	bar.setResources(this->getCurrentPlayer()->getResources());

	Resources limit;
	for (uint32_t i = 0; i < this->map.getStatePtr()->getCollectionsPtr()->totalBuildings(); i = i + 1) {
		Building* b = this->map.getStatePtr()->getCollectionsPtr()->getBuilding(i);
		if (b->exist() and b->getPlayerId() == this->getCurrentPlayer()->getId()) {
			limit.plus(b->getLimit());
		}
	}
	bar.setLimit(limit);

	uint32_t population = 0;
	for (uint32_t i = 0; i < this->map.getStatePtr()->getCollectionsPtr()->totalWarriors(); i = i + 1) {
		Warrior* w = this->map.getStatePtr()->getCollectionsPtr()->getWarrior(i);
		if (w->exist() and w->getPlayerId() == this->getCurrentPlayer()->getId()) {
			population = population + w->getPopulation();
		}
	}
	bar.setPopulation(population);

	uint32_t populationLimit = 0;
	for (uint32_t i = 0; i < this->map.getStatePtr()->getCollectionsPtr()->totalBuildings(); i = i + 1) {
		Building* b = this->map.getStatePtr()->getCollectionsPtr()->getBuilding(i);
		if (b->exist() and b->getPlayerId() == this->getCurrentPlayer()->getId()) {
			populationLimit = populationLimit + b->getPopulationLimit();
		}
	}
	bar.setPopulationLimit(populationLimit);
	
	return bar;
}
void Room::sendWorldUIStateToClients(std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers &remotePlayers, SERVER_NET_SPECS::Importance importance) {
	for (uint32_t i = 0; i < this->map.getStatePtr()->getCollectionsPtr()->totalGOs(); i = i + 1) {
		this->map.getStatePtr()->getCollectionsPtr()->getGO(i, FILTER::DEFAULT_PRIORITY)->update(this->map.getStatePtr(), this->getCurrentPlayer()->getId());
	}

	std::vector<std::shared_ptr<const RectangularUiElement>> buttonBases = this->makeButtonBases();
	ResourceBar resourceBar = this->makeResourceBar();

	std::string serialStr;
	boost::iostreams::back_insert_device<std::string> inserter(serialStr);
	boost::iostreams::stream<boost::iostreams::back_insert_device<std::string>> s(inserter);
	oarchive ar(s);
	ar << this->map << this->element << this->selected << this->highlightTable << this->curcorVisibility << buttonBases << resourceBar;
	s.flush();

	sf::Packet packet = this->makeBasePacket();
	packet << SERVER_NET_SPECS::CODES::WORLD_UI_STATE;
	packet << serialStr;

	this->sendToClients(packet, toSend, remotePlayers, importance);
}






sf::Packet Room::makeBasePacket() const {
    sf::Packet packet;
    packet << (sf::Uint64)GlobalRandomGenerator64::get()->gen();
    packet << (sf::Uint64)this->getID().value();
    return packet;
}
void Room::sendToClients(const sf::Packet& what, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers, SERVER_NET_SPECS::Importance importance) {
	std::unordered_map<uint32_t, bool> clientTable; // in case one host has more than one player
	for (uint32_t i = 1; i <= this->map.getStatePtr()->getPlayersPtr()->total(); i = i + 1) {
		sf::IpAddress clientIp = remotePlayers.get(i).getIp();
		uint32_t clientIpInt = clientIp.toInteger();
		if (clientTable.find(clientIpInt) == clientTable.end()) {
			clientTable[clientIpInt] = true;
            this->sendToClient(what, toSend, clientIp, importance);
		}
	}
}
void Room::sendToClient(const sf::Packet &what, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const sf::IpAddress &host, SERVER_NET_SPECS::Importance importance) {
    if (what.getDataSize() > sf::UdpSocket::MaxDatagramSize) {
        throw PackageLimit();
    }

    for (uint32_t i = 0; i < importance; i = i + 1) {
        toSend->emplace_back(what, host);
    }
}





void Room::receive(const boost::optional<std::tuple<sf::Packet, sf::IpAddress>>& received, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	if (!received.has_value()) {
		return;
	}

	sf::Packet packet = std::get<0>(received.value());
	sf::IpAddress ip = std::get<1>(received.value());

    sf::Uint64 packageUID;
    packet >> packageUID;
    if (this->receivedPackages.find(packageUID) == this->receivedPackages.end()) {
        this->receivedPackages[packageUID] = true;
        sf::Uint64 roomId;
        packet >> roomId;
        if (this->id.value() == roomId) {
            uint8_t code;
            packet >> code;
            if (code == CLIENT_NET_SPECS::CODES::OK) {
                this->receiveOK();
            }
            else if (code == CLIENT_NET_SPECS::CODES::CLICK) {
                this->receiveClick(packet, ip, toSend, remotePlayers);
            }
        }
    }
}
void Room::receiveOK() {
	this->noOKReceivedTimer.reset();
}
void Room::receiveClick(sf::Packet& remPacket, const sf::IpAddress &ip, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	uint8_t mouseButton;
	uint32_t x, y, viewX, viewY, w, h;
	remPacket >> mouseButton >> x >> y >> viewX >> viewY >> w >> h;
	if (this->selected == nullptr) {
		if (this->element == nullptr) {
			if (!this->animation.has_value() and this->events.empty() and this->allNewMoveEventsAdded()) {
				if (mouseButton == sf::Mouse::Button::Left) {
					this->addButtonClickEventToQueue(x, y, toSend, remotePlayers);
				}
				if (this->events.empty()) {
					this->addGameObjectClickEventToQueue(mouseButton, viewX, viewY, toSend, remotePlayers);
				}
			}
		}
		else {
			if (mouseButton == sf::Mouse::Button::Left) {
				Events events = this->element->click(x, y, w, h);
				this->addEvents(events, toSend, remotePlayers);
			}	
		}
	}
	else {
		Events events = this->selected->unselect(this->map.getStatePtr(), viewX / 64, viewY / 64, mouseButton);
		this->addEvents(events, toSend, remotePlayers);
	}
}







void Room::handleEvent(std::shared_ptr<Event> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	if (std::shared_ptr<AddResourceEvent> addResourceEvent = std::dynamic_pointer_cast<AddResourceEvent>(e)) {
		this->handleAddResourceEvent(addResourceEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<SubResourceEvent> subResourceEvent = std::dynamic_pointer_cast<SubResourceEvent>(e)) {
		this->handleSubResourceEvent(subResourceEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<AddResourcesEvent> addResourcesEvent = std::dynamic_pointer_cast<AddResourcesEvent>(e)) {
		this->handleAddResourcesEvent(addResourcesEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<SubResourcesEvent> subResourcesEvent = std::dynamic_pointer_cast<SubResourcesEvent>(e)) {
		this->handleSubResourcesEvent(subResourcesEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<SetHighlightEvent> changeHighlightEvent = std::dynamic_pointer_cast<SetHighlightEvent>(e)) {
		this->handleSetHighlightEvent(changeHighlightEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<AddHpEvent> addHpEvent = std::dynamic_pointer_cast<AddHpEvent>(e)) {
		this->handleAddHpEvent(addHpEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<DecreaseCurrentTradeMovesLeftEvent> decreaseCurrentTradeMovesLeftEvent = std::dynamic_pointer_cast<DecreaseCurrentTradeMovesLeftEvent>(e)) {
		this->handleDecreaseCurrentTradeMovesLeft(decreaseCurrentTradeMovesLeftEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<BuildEvent> buildEvent = std::dynamic_pointer_cast<BuildEvent>(e)) {
		this->handleBuild(buildEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<PlaySoundEvent> playSoundEvent = std::dynamic_pointer_cast<PlaySoundEvent>(e)) {
		this->handlePlaySoundEvent(playSoundEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<CreateEEvent> createEEvent = std::dynamic_pointer_cast<CreateEEvent>(e)) {
		this->handleCreatePopUpElementEvent(createEEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<ChangeMoveEvent> changeMoveEvent = std::dynamic_pointer_cast<ChangeMoveEvent>(e)) {
		this->handleChangeMoveEvent(changeMoveEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<ReturnToMenuEvent> returnToMenuEvent = std::dynamic_pointer_cast<ReturnToMenuEvent>(e)) {
		this->handleReturnToMenuEvent(returnToMenuEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<DestroyEvent> destroyEvent = std::dynamic_pointer_cast<DestroyEvent>(e)) {
		this->handleDestroyEvent(destroyEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<DecreaseCurrentProducingMovesLeftEvent> decreaseCurrentProducingMovesLeftEvent = std::dynamic_pointer_cast<DecreaseCurrentProducingMovesLeftEvent>(e)) {
		this->handleDecreaseCurrentProdusingMovesLeftEvent(decreaseCurrentProducingMovesLeftEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<WarriorProducingFinishedEvent> warriorProducingFinishedEvent = std::dynamic_pointer_cast<WarriorProducingFinishedEvent>(e)) {
		this->handleWarriorProducingFinishedEvent(warriorProducingFinishedEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<SelectEvent> selectEvent = std::dynamic_pointer_cast<SelectEvent>(e)) {
		this->handleSelectEvent(selectEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<UnselectEvent> unselectEvent = std::dynamic_pointer_cast<UnselectEvent>(e)) {
		this->handleUnselectEvent(unselectEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<StartWarriorAnimationEvent> startWarriorAnimationEvent = std::dynamic_pointer_cast<StartWarriorAnimationEvent>(e)) {
		this->handleStartWarriorAnimationEvent(startWarriorAnimationEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<RefreshMovementPointsEvent> refreshMovementPointsEvent = std::dynamic_pointer_cast<RefreshMovementPointsEvent>(e)) {
		this->handleRefreshMovementPointsEvent(refreshMovementPointsEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<EnableCursorEvent> enableCursorEvent = std::dynamic_pointer_cast<EnableCursorEvent>(e)) {
		this->handleEnableCursorEvent(enableCursorEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<DisableCursorEvent> disableCursorEvent = std::dynamic_pointer_cast<DisableCursorEvent>(e)) {
		this->handleDisableCursorEvent(disableCursorEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<CreateAnimationEvent> createAnimationEvent = std::dynamic_pointer_cast<CreateAnimationEvent>(e)) {
		this->handleCreateAnimationEvent(createAnimationEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<DecreaseBurningMovesLeftEvent> decreaseBurningMovesLeftEvent = std::dynamic_pointer_cast<DecreaseBurningMovesLeftEvent>(e)) {
		this->handleDecreaseBurningMovesLeftEvent(decreaseBurningMovesLeftEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<SubHpEvent> subHpEvent = std::dynamic_pointer_cast<SubHpEvent>(e)) {
		this->handleSubHpEvent(subHpEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<SetFireEvent> setFireEvent = std::dynamic_pointer_cast<SetFireEvent>(e)) {
		this->handleSetFireEvent(setFireEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<ChangeWarriorDirectionEvent> changeWarriorDirectionEvent = std::dynamic_pointer_cast<ChangeWarriorDirectionEvent>(e)) {
		this->handleChangeWarriorDirectionEvent(changeWarriorDirectionEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<FocusOnEvent> focusOnEvent = std::dynamic_pointer_cast<FocusOnEvent>(e)) {
		this->handleFocusOnEvent(focusOnEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<ResetHighlightEvent> resetHighlightEvent = std::dynamic_pointer_cast<ResetHighlightEvent>(e)) {
		this->handleResetHighlightEvent(resetHighlightEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<DoTradeEvent> doTradeEvent = std::dynamic_pointer_cast<DoTradeEvent>(e)) {
		this->handleDoTradeEvent(doTradeEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<StartWarriorProducingEvent> startWarriorProducingEvent = std::dynamic_pointer_cast<StartWarriorProducingEvent>(e)) {
		this->handleStartWarriorProducingEvent(startWarriorProducingEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<TryToBuildEvent> tryToBuildEvent = std::dynamic_pointer_cast<TryToBuildEvent>(e)) {
		this->handleTryToBuildEvent(tryToBuildEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<KillNextTurnEvent> killNextTurnEvent = std::dynamic_pointer_cast<KillNextTurnEvent>(e)) {
		this->handleKillNextTurnEvent(killNextTurnEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<RevertKillNextTurnEvent> revertKillNextTurnEvent = std::dynamic_pointer_cast<RevertKillNextTurnEvent>(e)) {
		this->handleRevertKillNextTurnEvent(revertKillNextTurnEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<CloseAnimationEvent> closeAnimationEvent = std::dynamic_pointer_cast<CloseAnimationEvent>(e)) {
		this->handleCloseAnimationEvent(closeAnimationEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<DecreaseSpellCreationMovesLeftEvent> decreaseSpellCreationMovesLeftEvent = std::dynamic_pointer_cast<DecreaseSpellCreationMovesLeftEvent>(e)) {
		this->handleDecreaseSpellCreationMovesLeftEvent(decreaseSpellCreationMovesLeftEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<SetSpellEvent> setSpellEvent = std::dynamic_pointer_cast<SetSpellEvent>(e)) {
		this->handleSetSpellEvent(setSpellEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<UseSpellEvent> useSpellEvent = std::dynamic_pointer_cast<UseSpellEvent>(e)) {
		this->handleUseSpellEvent(useSpellEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<MarkSpellAsUsedEvent> markSpellAsUsedEvent = std::dynamic_pointer_cast<MarkSpellAsUsedEvent>(e)) {
		this->handleMarkSpellAsUsedEvent(markSpellAsUsedEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<EnableWarriorRageModeEvent> enableWarriorRageModeEvent = std::dynamic_pointer_cast<EnableWarriorRageModeEvent>(e)) {
		this->handleEnableWarriorRageModeEvent(enableWarriorRageModeEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<DecreaseRageModeMovesLeftEvent> decreaseRageModeMovesLeftEvent = std::dynamic_pointer_cast<DecreaseRageModeMovesLeftEvent>(e)) {
		this->handleDecreaseRageModeMovesLeftEvent(decreaseRageModeMovesLeftEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<RefreshAttackAbilityEvent> refreshAttackAbilityEvent = std::dynamic_pointer_cast<RefreshAttackAbilityEvent>(e)) {
		this->handleRefreshAttackAbilityEvent(refreshAttackAbilityEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<WipeAttackAbilityEvent> wipeAttackAbilityEvent = std::dynamic_pointer_cast<WipeAttackAbilityEvent>(e)) {
		this->handleWipeAttackAbilityEvent(wipeAttackAbilityEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<RefreshAttackedTableEvent> refreshAttackedTableEvent = std::dynamic_pointer_cast<RefreshAttackedTableEvent>(e)) {
		this->handleRefreshAttackedTableEvent(refreshAttackedTableEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<MarkAsAttackedEvent> markAsAttackedEvent = std::dynamic_pointer_cast<MarkAsAttackedEvent>(e)) {
		this->handleMarkAsAttackedEvent(markAsAttackedEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<RefreshHealingAbilityEvent> refreshHealingAbilityEvent = std::dynamic_pointer_cast<RefreshHealingAbilityEvent>(e)) {
		this->handleRefreshHealingAbilityEvent(refreshHealingAbilityEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<WipeHealingAbilityEvent> wipeHealingAbilityEvent = std::dynamic_pointer_cast<WipeHealingAbilityEvent>(e)) {
		this->handleWipeHealingAbilityEvent(wipeHealingAbilityEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<MarkPlayerAsInactiveEvent> markPlayerAsInactiveEvent = std::dynamic_pointer_cast<MarkPlayerAsInactiveEvent>(e)) {
		this->handleMarkPlayerAsInactiveEvent(markPlayerAsInactiveEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<IncreaseVCSMoveCtrEvent> increaseVcsMoveCtrEvent = std::dynamic_pointer_cast<IncreaseVCSMoveCtrEvent>(e)) {
		this->handleIncreaseVCSMoveCtrEvent(increaseVcsMoveCtrEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<SaveGameEvent> saveGameEvent = std::dynamic_pointer_cast<SaveGameEvent>(e)) {
		this->handleSaveGameEvent(saveGameEvent, toSend, remotePlayers);
	}
	else if (std::shared_ptr<LimitResourcesEvent> limitResourcesEvent = std::dynamic_pointer_cast<LimitResourcesEvent>(e)) {
		this->handleLimitResourcesEvent(limitResourcesEvent, toSend, remotePlayers);
	}
	else {
		std::cerr << "Room: warning: unknown event handled" << std::endl;
	}
}
void Room::handleAddResourceEvent(std::shared_ptr<AddResourceEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	this->getCurrentPlayer()->addResource(e->getResource(), e->getLimit().get(e->getResource().type));
}
void Room::handleSubResourceEvent(std::shared_ptr<SubResourceEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	this->getCurrentPlayer()->subResource(e->getResource());
}
void Room::handleAddResourcesEvent(std::shared_ptr<AddResourcesEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	this->getCurrentPlayer()->addResources(e->getResources(), e->getLimit());
}
void Room::handleSubResourcesEvent(std::shared_ptr<SubResourcesEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	this->getCurrentPlayer()->subResources(e->getResources());
}
void Room::handleSetHighlightEvent(std::shared_ptr<SetHighlightEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	this->highlightTable.mark(*e);
}
void Room::handleAddHpEvent(std::shared_ptr<AddHpEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	HPGO* go = e->getHPGO();
	uint32_t n = e->getN();
	go->addHp(n);
}
void Room::handleDecreaseCurrentTradeMovesLeft(std::shared_ptr<DecreaseCurrentTradeMovesLeftEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	e->getSpec()->decreaseCurrentTradeMovesLeft();
}
void Room::handleBuild(std::shared_ptr<BuildEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	Building* b = e->getBuilding();
	this->map.getStatePtr()->getCollectionsPtr()->add(b);
}
void Room::handlePlaySoundEvent(std::shared_ptr<PlaySoundEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	sf::Packet packet = this->makeBasePacket();
	packet << SERVER_NET_SPECS::CODES::SOUND;
	packet << e->getSoundName();
	this->sendToClients(packet, toSend, remotePlayers, SERVER_NET_SPECS::Importance::ExtremelyImportant);
}
void Room::handleCreatePopUpElementEvent(std::shared_ptr<CreateEEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	this->element = e->getElement();
	this->element->restart();
}
void Room::handleChangeMoveEvent(std::shared_ptr<ChangeMoveEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	this->changeMove();
}
void Room::handleReturnToMenuEvent(std::shared_ptr<ReturnToMenuEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	sf::Packet packet = this->makeBasePacket();
	packet << SERVER_NET_SPECS::CODES::RETURN_TO_MENU;
	switch (e->getType()) {
	case ReturnToMenuEvent::Type::CURRENT_PLAYER:
		this->sendToClient(packet, toSend, remotePlayers.get(this->currentPlayerId).getIp(), SERVER_NET_SPECS::Importance::ExtremelyImportant);
		break;
	case ReturnToMenuEvent::Type::EVERY_PLAYER:
		this->sendToClients(packet, toSend, remotePlayers, SERVER_NET_SPECS::Importance::ExtremelyImportant);
		break;
	}
}
void Room::handleDestroyEvent(std::shared_ptr<DestroyEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	Events destroyBuildingEvent = e->getBuilding()->destroy(this->map.getStatePtr());
	this->addEvents(destroyBuildingEvent, toSend, remotePlayers);
}
void Room::handleDecreaseCurrentProdusingMovesLeftEvent(std::shared_ptr<DecreaseCurrentProducingMovesLeftEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	e->getSpec()->decreaseCurrentProducingMovesLeft();
}
void Room::handleWarriorProducingFinishedEvent(std::shared_ptr<WarriorProducingFinishedEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	e->getSpec()->stopProducing();
	this->map.getStatePtr()->getCollectionsPtr()->add(e->getWarrior()->cloneWarrior());
}
void Room::handleSelectEvent(std::shared_ptr<SelectEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	this->selected = e->getSelectable();
}
void Room::handleUnselectEvent(std::shared_ptr<UnselectEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	this->selected = nullptr;
}
void Room::handleStartWarriorAnimationEvent(std::shared_ptr<StartWarriorAnimationEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	e->getWarrior()->startAnimation(e->getAnimation());
}
void Room::handleRefreshMovementPointsEvent(std::shared_ptr<RefreshMovementPointsEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	e->getWarrior()->refreshMovementPoints();
}
void Room::handleEnableCursorEvent(std::shared_ptr<EnableCursorEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	this->curcorVisibility = true;
}
void Room::handleDisableCursorEvent(std::shared_ptr<DisableCursorEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	this->curcorVisibility = false;
}
void Room::handleCreateAnimationEvent(std::shared_ptr<CreateAnimationEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	this->animation = e->getAnimation();
}
void Room::handleDecreaseBurningMovesLeftEvent(std::shared_ptr<DecreaseBurningMovesLeftEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	e->getBuilding()->decreaseBurningMovesLeft();
}
void Room::handleSubHpEvent(std::shared_ptr<SubHpEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	e->getHPGO()->subHp(e->getValue());
}
void Room::handleSetFireEvent(std::shared_ptr<SetFireEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	e->getBuilding()->setFire();
}
void Room::handleChangeWarriorDirectionEvent(std::shared_ptr<ChangeWarriorDirectionEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	e->getWarrior()->changeDirection(e->getDirection());
}
void Room::handleFocusOnEvent(std::shared_ptr<FocusOnEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	sf::Packet packet = this->makeBasePacket();
	packet << SERVER_NET_SPECS::CODES::FOCUS;
	packet << e->getX();
	packet << e->getY();
	packet << e->getSX();
	packet << e->getSY();
	this->sendToClients(packet, toSend, remotePlayers, SERVER_NET_SPECS::Importance::ExtremelyImportant);
}
void Room::handleResetHighlightEvent(std::shared_ptr<ResetHighlightEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	this->highlightTable.clear();
}
void Room::handleDoTradeEvent(std::shared_ptr<DoTradeEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	Events events = e->getSpec()->doTrade(e->getBuilding(), e->getTrade());
	this->addEvents(events, toSend, remotePlayers);
}
void Room::handleStartWarriorProducingEvent(std::shared_ptr<StartWarriorProducingEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	Events events = e->getSpec()->startProducing(e->getWarrior());
	this->addEvents(events, toSend, remotePlayers);
}
void Room::handleTryToBuildEvent(std::shared_ptr<TryToBuildEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	if (this->getCurrentPlayer()->getResources() >= e->getBuilding()->getCost()) {
		this->bm = BuildingMode(e->getBuilding(), this->getCurrentPlayer()->getId());
		Events bmStartEvent = bm.start(this->map.getStatePtr());
		this->addEvents(bmStartEvent, toSend, remotePlayers);
	}
	else {
		Events clickEvent;
		clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

		std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(StringLcl("{no_resources_for_building}"), StringLcl("{OK}"), clickEvent);
		Events unableToBuildEvent;
		unableToBuildEvent.add(std::make_shared<CreateEEvent>(w));
		this->addEvents(unableToBuildEvent, toSend, remotePlayers);
	}
}
void Room::handleKillNextTurnEvent(std::shared_ptr<KillNextTurnEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	Events events = e->getWarrior()->killNextTurn();
	this->addEvents(events, toSend, remotePlayers);
}
void Room::handleRevertKillNextTurnEvent(std::shared_ptr<RevertKillNextTurnEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	Events events = e->getWarrior()->revertKillNextTurn();
	this->addEvents(events, toSend, remotePlayers);
}
void Room::handleCloseAnimationEvent(std::shared_ptr<CloseAnimationEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	this->animation = boost::none;
}
void Room::handleDecreaseSpellCreationMovesLeftEvent(std::shared_ptr<DecreaseSpellCreationMovesLeftEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	e->getSpell()->decreaseCreationMovesLeft();
}
void Room::handleSetSpellEvent(std::shared_ptr<SetSpellEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	e->getSpec()->setSpell(e->getSpell());
}
void Room::handleUseSpellEvent(std::shared_ptr<UseSpellEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	Events events = e->getSpell()->use();
	this->addEvents(events, toSend, remotePlayers);
}
void Room::handleMarkSpellAsUsedEvent(std::shared_ptr<MarkSpellAsUsedEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	e->getSpell()->markAsUsed();
}
void Room::handleEnableWarriorRageModeEvent(std::shared_ptr<EnableWarriorRageModeEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	e->getWarrior()->enableRageMode();
}
void Room::handleDecreaseRageModeMovesLeftEvent(std::shared_ptr<DecreaseRageModeMovesLeftEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	e->getWarrior()->decreaseRageModeMovesLeft();
}
void Room::handleRefreshAttackAbilityEvent(std::shared_ptr<RefreshAttackAbilityEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	e->getI()->refreshAbility();
}
void Room::handleWipeAttackAbilityEvent(std::shared_ptr<WipeAttackAbilityEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	e->getI()->wipeAbility();
}
void Room::handleRefreshAttackedTableEvent(std::shared_ptr<RefreshAttackedTableEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	e->getWarrior()->refreshAttackedTable();
}
void Room::handleMarkAsAttackedEvent(std::shared_ptr<MarkAsAttackedEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	e->getAttacker()->markAsAttacked(e->getTarget());
}
void Room::handleRefreshHealingAbilityEvent(std::shared_ptr<RefreshHealingAbilityEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	e->getWarrior()->refreshHealingAbility();
}
void Room::handleWipeHealingAbilityEvent(std::shared_ptr<WipeHealingAbilityEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	e->getWarrior()->wipeHealingAbility();
}
void Room::handleMarkPlayerAsInactiveEvent(std::shared_ptr<MarkPlayerAsInactiveEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	this->playerIsActive[e->getPlayerId() - 1] = false;
	uint32_t count = 0;
	for (uint32_t i = 0; i < this->playerIsActive.size(); i = i + 1) {
		count = count + this->playerIsActive.at(i);
	}
	std::shared_ptr<WindowButton> w;
	if (count == 1) {
		Events returnToMenuEvent;
		returnToMenuEvent.add(std::make_shared<PlaySoundEvent>("click"));
		returnToMenuEvent.add(std::make_shared<ReturnToMenuEvent>(ReturnToMenuEvent::Type::EVERY_PLAYER));
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
	Events result;
	result.add(std::make_shared<CreateEEvent>(w));
	this->addEvents(result, toSend, remotePlayers);
}
void Room::handleIncreaseVCSMoveCtrEvent(std::shared_ptr<IncreaseVCSMoveCtrEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	e->getSpec()->increaseMoveCtr();
}
void Room::handleSaveGameEvent(std::shared_ptr<SaveGameEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	sf::Packet packet = this->makeBasePacket();
	packet << SERVER_NET_SPECS::CODES::SAVE;
	packet << this->getSaveData();
	this->sendToClient(packet, toSend, remotePlayers.get(this->currentPlayerId).getIp(), SERVER_NET_SPECS::Importance::ExtremelyImportant);
}
void Room::handleLimitResourcesEvent(std::shared_ptr<LimitResourcesEvent> e, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const RemotePlayers& remotePlayers) {
	this->map.getStatePtr()->getPlayersPtr()->getPlayerPtr(e->getPlayerId())->limitResources(e->getLimit());
}