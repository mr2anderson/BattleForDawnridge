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
#include "IncorrectMoveRepresentation.hpp"
#include "IncorrectPlayersRepresentation.hpp"
#include "MapTooBig.hpp"
#include "TooMuchGameObjects.hpp"
#include "TooMuchPlayers.hpp"
#include "MapDeepError.hpp"
#include "CurrentRoomIDButtonSpec.hpp"
#include "GlobalRandomGenerator64.hpp"
#include "NoActivePlayers.hpp"






Room::Room(RoomID id, const std::string &saveData, Restrictions restrictions) {
	this->id = id;
	this->restrictions = restrictions;
	this->timeoutTimer = Timer(60 * 60 * 1000, Timer::TYPE::FIRST_DEFAULT);

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
	this->buttons.emplace_back(CurrentRoomIDButtonSpec(4, this->id.value()));
}





RoomID Room::getID() const {
	return this->id;
}






uint32_t Room::playersNumber() const {
	return this->map.getStatePtr()->getPlayersPtr()->total();
}







void Room::update(const boost::optional<std::tuple<sf::Packet, sf::IpAddress>>& received, RoomOutputProtocol p) {
	if (this->restrictions == Restrictions::Enable and this->timeoutTimer.ready()) {
		throw NoActivePlayers();
	}

    if (this->requireInit) {
		p.logs->emplace_back("{sending_init_world_ui_state}");
        this->sendWorldUIStateToClients(p);
        this->requireInit = false;
    }

	this->receive(received, p);

	if (this->element != nullptr) {
		this->element->update();
		if (this->element->finished()) {
			this->element = nullptr;
		}
	}
	if (this->animation.has_value()) {
		Events animationEvent;
		animationEvent = this->animation.value().process(this->map.getStatePtr());
		this->addEvents(animationEvent, p);
	}
	this->processNewMoveEvents(p);
	this->processBaseEvents(p);
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





void Room::processNewMoveEvents(RoomOutputProtocol p) {
	bool somethingProcessed = false;
	while (this->currentGOIndexNewMoveEvent != this->totalGONewMoveEvents) {
		if (this->element != nullptr or !this->events.empty()) {
			break;
		}
		Events newMoveEvent = this->map.getStatePtr()->getCollectionsPtr()->getGO(this->currentGOIndexNewMoveEvent, FILTER::NEW_MOVE_PRIORITY)->newMove(this->map.getStatePtr(), this->getCurrentPlayer()->getId());
		this->addEvents(newMoveEvent, p);
		this->currentGOIndexNewMoveEvent = this->currentGOIndexNewMoveEvent + 1;
		this->processBaseEvents(p, false);
		somethingProcessed = true;
	}
	if (somethingProcessed) {
		this->sendWorldUIStateToClients(p);
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
void Room::addButtonClickEventToQueue(uint32_t x, uint32_t y, RoomOutputProtocol p) {
	for (uint32_t i = 0; i < this->buttons.size(); i = i + 1) {
		Events buttonClickEvent = this->buttons.at(i).click(x, y);
		if (!buttonClickEvent.empty()) {
			this->addEvents(buttonClickEvent, p);
			break;
		}
	}
}
void Room::addGameObjectClickEventToQueue(uint8_t button, uint32_t viewX, uint32_t viewY, RoomOutputProtocol p) {
	for (uint32_t i = 0; i < this->map.getStatePtr()->getCollectionsPtr()->totalGOs(); i = i + 1) {
		GO* go = this->map.getStatePtr()->getCollectionsPtr()->getGO(i, FILTER::CLICK_PRIORITY);
		Events gor = go->click(this->map.getStatePtr(), this->getCurrentPlayer()->getId(), button, viewX, viewY);
		if (!gor.empty()) {
			this->addEvents(gor, p);
			return;
		}
	}
}
void Room::processBaseEvents(RoomOutputProtocol p, bool sendToClients) {
	bool somethingProcessed = false;
	while (!this->events.empty()) {
		if (this->element != nullptr or this->animation.has_value()) {
			break;
		}
		this->handleEvent(this->events.front(), p);
		this->events.pop();
		somethingProcessed = true;
	}
	if (sendToClients and somethingProcessed) {
		this->sendWorldUIStateToClients(p);
	}
}
void Room::addEvents(Events& e, RoomOutputProtocol p) {
    bool urgentHandled = false;

	for (uint32_t i = 0; i < e.size(); i = i + 1) {
		std::shared_ptr<Event> event = e.at(i);
		if (event->isUrgent()) {
			this->handleEvent(event, p);
            urgentHandled = true;
		}
		else {
			this->events.push(event);
		}
	}

    if (urgentHandled) {
        this->sendWorldUIStateToClients(p);
    }
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
void Room::sendWorldUIStateToClients(RoomOutputProtocol p) {
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

	p.logs->emplace_back("{sending_world_ui_state}" + std::to_string(packet.getDataSize()) + " kb");

	this->sendToClients(packet, p);
}
void Room::sendPlaySoundEventToClients(RoomOutputProtocol p, const std::string& soundName) {
	p.logs->emplace_back("{sending_sound_to_players}");
	
	sf::Packet packet = this->makeBasePacket();
	packet << SERVER_NET_SPECS::CODES::SOUND;
	packet << soundName;

	this->sendToClients(packet, p);
}
void Room::sendSaveToClient(RoomOutputProtocol p) {
	p.logs->emplace_back("{sending_save_to_current_player}");

	sf::Packet packet = this->makeBasePacket();
	packet << SERVER_NET_SPECS::CODES::SAVE;
	packet << this->getSaveData();

	this->sendToClient(packet, p.toSend, p.remotePlayers->get(this->currentPlayerId).getIp());
}
void Room::sendReturnToMenuToClient(RoomOutputProtocol p) {
	p.logs->emplace_back("{sending_return_to_menu_to_current_player}");

	sf::Packet packet = this->makeBasePacket();
	packet << SERVER_NET_SPECS::CODES::RETURN_TO_MENU;

	this->sendToClient(packet, p.toSend, p.remotePlayers->get(this->currentPlayerId).getIp());
}
void Room::sendReturnToMenuToClients(RoomOutputProtocol p) {
	p.logs->emplace_back("{sending_return_to_menu_to_players}");

	sf::Packet packet = this->makeBasePacket();
	packet << SERVER_NET_SPECS::CODES::RETURN_TO_MENU;

	this->sendToClients(packet, p);
}
void Room::sendFocusOnToClients(RoomOutputProtocol p, uint32_t x, uint32_t y, uint32_t sx, uint32_t sy) {
	sf::Packet packet = this->makeBasePacket();

	packet << SERVER_NET_SPECS::CODES::FOCUS;
	packet << x;
	packet << y;
	packet << sx;
	packet << sy;

	this->sendToClients(packet, p);
}
void Room::sendNotYourMove(RoomOutputProtocol p, sf::IpAddress ip) {
	p.logs->emplace_back("{sending_not_your_move_to}" + ip.toString());

	sf::Packet packet = this->makeBasePacket();

	packet << SERVER_NET_SPECS::CODES::NOT_YOUR_MOVE;

	this->sendToClient(packet, p.toSend, ip);
}






sf::Packet Room::makeBasePacket() const {
    sf::Packet packet;
    packet << this->getID().value();
    return packet;
}
void Room::sendToClients(const sf::Packet& what, RoomOutputProtocol p) {
	std::unordered_map<uint32_t, bool> clientTable; // in case one host has more than one player
	for (uint32_t i = 1; i <= this->map.getStatePtr()->getPlayersPtr()->total(); i = i + 1) {
		sf::IpAddress clientIp = p.remotePlayers->get(i).getIp();
		uint32_t clientIpInt = clientIp.toInteger();
		if (clientTable.find(clientIpInt) == clientTable.end()) {
			clientTable[clientIpInt] = true;
            this->sendToClient(what, p.toSend, clientIp);
		}
	}
}
void Room::sendToClient(const sf::Packet &what, std::vector<std::tuple<sf::Packet, sf::IpAddress>>* toSend, const sf::IpAddress &host) {
    toSend->emplace_back(what, host);
}





void Room::receive(const boost::optional<std::tuple<sf::Packet, sf::IpAddress>>& received, RoomOutputProtocol p) {
	if (!received.has_value()) {
		return;
	}

	sf::Packet packet = std::get<0>(received.value());
	sf::IpAddress ip = std::get<1>(received.value());

    std::string roomId;
    packet >> roomId;
    if (this->id.value() == roomId) {
		this->timeoutTimer.reset();
        uint8_t code;
        packet >> code;
        if (code == CLIENT_NET_SPECS::CODES::CLICK) {
            this->receiveClick(packet, ip, p);
        }
    }
}
void Room::receiveClick(sf::Packet& remPacket, const sf::IpAddress &ip, RoomOutputProtocol p) {
	uint8_t mouseButton;
	uint32_t x, y, viewX, viewY, w, h;
	remPacket >> mouseButton >> x >> y >> viewX >> viewY >> w >> h;
	p.logs->emplace_back("{received_click} " + std::to_string(mouseButton) + " (" + std::to_string(x) + ", " + std::to_string(y) + ") (" + std::to_string(viewX) + ", " + std::to_string(viewY) + ") (" + std::to_string(w) + ", " + std::to_string(h) + ") " + ip.toString());
	if (p.remotePlayers->get(this->currentPlayerId).getIp() != ip) {
		this->sendNotYourMove(p, ip);
		return;
	}
	if (this->selected == nullptr) {
		if (this->element == nullptr) {
			if (!this->animation.has_value() and this->events.empty() and this->allNewMoveEventsAdded()) {
				if (mouseButton == sf::Mouse::Button::Left) {
					this->addButtonClickEventToQueue(x, y, p);
				}
				if (this->events.empty()) {
					this->addGameObjectClickEventToQueue(mouseButton, viewX, viewY, p);
				}
			}
		}
		else {
			if (mouseButton == sf::Mouse::Button::Left) {
				Events events = this->element->click(x, y, w, h);
				this->addEvents(events, p);
			}	
		}
	}
	else {
		Events events = this->selected->unselect(this->map.getStatePtr(), viewX / 64, viewY / 64, mouseButton);
		this->addEvents(events, p);
	}
}







void Room::handleEvent(std::shared_ptr<Event> e, RoomOutputProtocol p) {
	if (std::shared_ptr<AddResourceEvent> addResourceEvent = std::dynamic_pointer_cast<AddResourceEvent>(e)) {
		this->handleAddResourceEvent(addResourceEvent, p);
	}
	else if (std::shared_ptr<SubResourceEvent> subResourceEvent = std::dynamic_pointer_cast<SubResourceEvent>(e)) {
		this->handleSubResourceEvent(subResourceEvent, p);
	}
	else if (std::shared_ptr<AddResourcesEvent> addResourcesEvent = std::dynamic_pointer_cast<AddResourcesEvent>(e)) {
		this->handleAddResourcesEvent(addResourcesEvent, p);
	}
	else if (std::shared_ptr<SubResourcesEvent> subResourcesEvent = std::dynamic_pointer_cast<SubResourcesEvent>(e)) {
		this->handleSubResourcesEvent(subResourcesEvent, p);
	}
	else if (std::shared_ptr<SetHighlightEvent> changeHighlightEvent = std::dynamic_pointer_cast<SetHighlightEvent>(e)) {
		this->handleSetHighlightEvent(changeHighlightEvent, p);
	}
	else if (std::shared_ptr<AddHpEvent> addHpEvent = std::dynamic_pointer_cast<AddHpEvent>(e)) {
		this->handleAddHpEvent(addHpEvent, p);
	}
	else if (std::shared_ptr<DecreaseCurrentTradeMovesLeftEvent> decreaseCurrentTradeMovesLeftEvent = std::dynamic_pointer_cast<DecreaseCurrentTradeMovesLeftEvent>(e)) {
		this->handleDecreaseCurrentTradeMovesLeft(decreaseCurrentTradeMovesLeftEvent, p);
	}
	else if (std::shared_ptr<BuildEvent> buildEvent = std::dynamic_pointer_cast<BuildEvent>(e)) {
		this->handleBuild(buildEvent, p);
	}
	else if (std::shared_ptr<PlaySoundEvent> playSoundEvent = std::dynamic_pointer_cast<PlaySoundEvent>(e)) {
		this->handlePlaySoundEvent(playSoundEvent, p);
	}
	else if (std::shared_ptr<CreateEEvent> createEEvent = std::dynamic_pointer_cast<CreateEEvent>(e)) {
		this->handleCreatePopUpElementEvent(createEEvent, p);
	}
	else if (std::shared_ptr<ChangeMoveEvent> changeMoveEvent = std::dynamic_pointer_cast<ChangeMoveEvent>(e)) {
		this->handleChangeMoveEvent(changeMoveEvent, p);
	}
	else if (std::shared_ptr<ReturnToMenuEvent> returnToMenuEvent = std::dynamic_pointer_cast<ReturnToMenuEvent>(e)) {
		this->handleReturnToMenuEvent(returnToMenuEvent, p);
	}
	else if (std::shared_ptr<DestroyEvent> destroyEvent = std::dynamic_pointer_cast<DestroyEvent>(e)) {
		this->handleDestroyEvent(destroyEvent, p);
	}
	else if (std::shared_ptr<DecreaseCurrentProducingMovesLeftEvent> decreaseCurrentProducingMovesLeftEvent = std::dynamic_pointer_cast<DecreaseCurrentProducingMovesLeftEvent>(e)) {
		this->handleDecreaseCurrentProdusingMovesLeftEvent(decreaseCurrentProducingMovesLeftEvent, p);
	}
	else if (std::shared_ptr<WarriorProducingFinishedEvent> warriorProducingFinishedEvent = std::dynamic_pointer_cast<WarriorProducingFinishedEvent>(e)) {
		this->handleWarriorProducingFinishedEvent(warriorProducingFinishedEvent, p);
	}
	else if (std::shared_ptr<SelectEvent> selectEvent = std::dynamic_pointer_cast<SelectEvent>(e)) {
		this->handleSelectEvent(selectEvent, p);
	}
	else if (std::shared_ptr<UnselectEvent> unselectEvent = std::dynamic_pointer_cast<UnselectEvent>(e)) {
		this->handleUnselectEvent(unselectEvent, p);
	}
	else if (std::shared_ptr<StartWarriorAnimationEvent> startWarriorAnimationEvent = std::dynamic_pointer_cast<StartWarriorAnimationEvent>(e)) {
		this->handleStartWarriorAnimationEvent(startWarriorAnimationEvent, p);
	}
	else if (std::shared_ptr<RefreshMovementPointsEvent> refreshMovementPointsEvent = std::dynamic_pointer_cast<RefreshMovementPointsEvent>(e)) {
		this->handleRefreshMovementPointsEvent(refreshMovementPointsEvent, p);
	}
	else if (std::shared_ptr<EnableCursorEvent> enableCursorEvent = std::dynamic_pointer_cast<EnableCursorEvent>(e)) {
		this->handleEnableCursorEvent(enableCursorEvent, p);
	}
	else if (std::shared_ptr<DisableCursorEvent> disableCursorEvent = std::dynamic_pointer_cast<DisableCursorEvent>(e)) {
		this->handleDisableCursorEvent(disableCursorEvent, p);
	}
	else if (std::shared_ptr<CreateAnimationEvent> createAnimationEvent = std::dynamic_pointer_cast<CreateAnimationEvent>(e)) {
		this->handleCreateAnimationEvent(createAnimationEvent, p);
	}
	else if (std::shared_ptr<DecreaseBurningMovesLeftEvent> decreaseBurningMovesLeftEvent = std::dynamic_pointer_cast<DecreaseBurningMovesLeftEvent>(e)) {
		this->handleDecreaseBurningMovesLeftEvent(decreaseBurningMovesLeftEvent, p);
	}
	else if (std::shared_ptr<SubHpEvent> subHpEvent = std::dynamic_pointer_cast<SubHpEvent>(e)) {
		this->handleSubHpEvent(subHpEvent, p);
	}
	else if (std::shared_ptr<SetFireEvent> setFireEvent = std::dynamic_pointer_cast<SetFireEvent>(e)) {
		this->handleSetFireEvent(setFireEvent, p);
	}
	else if (std::shared_ptr<ChangeWarriorDirectionEvent> changeWarriorDirectionEvent = std::dynamic_pointer_cast<ChangeWarriorDirectionEvent>(e)) {
		this->handleChangeWarriorDirectionEvent(changeWarriorDirectionEvent, p);
	}
	else if (std::shared_ptr<FocusOnEvent> focusOnEvent = std::dynamic_pointer_cast<FocusOnEvent>(e)) {
		this->handleFocusOnEvent(focusOnEvent, p);
	}
	else if (std::shared_ptr<ResetHighlightEvent> resetHighlightEvent = std::dynamic_pointer_cast<ResetHighlightEvent>(e)) {
		this->handleResetHighlightEvent(resetHighlightEvent, p);
	}
	else if (std::shared_ptr<DoTradeEvent> doTradeEvent = std::dynamic_pointer_cast<DoTradeEvent>(e)) {
		this->handleDoTradeEvent(doTradeEvent, p);
	}
	else if (std::shared_ptr<StartWarriorProducingEvent> startWarriorProducingEvent = std::dynamic_pointer_cast<StartWarriorProducingEvent>(e)) {
		this->handleStartWarriorProducingEvent(startWarriorProducingEvent, p);
	}
	else if (std::shared_ptr<TryToBuildEvent> tryToBuildEvent = std::dynamic_pointer_cast<TryToBuildEvent>(e)) {
		this->handleTryToBuildEvent(tryToBuildEvent, p);
	}
	else if (std::shared_ptr<KillNextTurnEvent> killNextTurnEvent = std::dynamic_pointer_cast<KillNextTurnEvent>(e)) {
		this->handleKillNextTurnEvent(killNextTurnEvent, p);
	}
	else if (std::shared_ptr<RevertKillNextTurnEvent> revertKillNextTurnEvent = std::dynamic_pointer_cast<RevertKillNextTurnEvent>(e)) {
		this->handleRevertKillNextTurnEvent(revertKillNextTurnEvent, p);
	}
	else if (std::shared_ptr<CloseAnimationEvent> closeAnimationEvent = std::dynamic_pointer_cast<CloseAnimationEvent>(e)) {
		this->handleCloseAnimationEvent(closeAnimationEvent, p);
	}
	else if (std::shared_ptr<DecreaseSpellCreationMovesLeftEvent> decreaseSpellCreationMovesLeftEvent = std::dynamic_pointer_cast<DecreaseSpellCreationMovesLeftEvent>(e)) {
		this->handleDecreaseSpellCreationMovesLeftEvent(decreaseSpellCreationMovesLeftEvent, p);
	}
	else if (std::shared_ptr<SetSpellEvent> setSpellEvent = std::dynamic_pointer_cast<SetSpellEvent>(e)) {
		this->handleSetSpellEvent(setSpellEvent, p);
	}
	else if (std::shared_ptr<UseSpellEvent> useSpellEvent = std::dynamic_pointer_cast<UseSpellEvent>(e)) {
		this->handleUseSpellEvent(useSpellEvent, p);
	}
	else if (std::shared_ptr<MarkSpellAsUsedEvent> markSpellAsUsedEvent = std::dynamic_pointer_cast<MarkSpellAsUsedEvent>(e)) {
		this->handleMarkSpellAsUsedEvent(markSpellAsUsedEvent, p);
	}
	else if (std::shared_ptr<EnableWarriorRageModeEvent> enableWarriorRageModeEvent = std::dynamic_pointer_cast<EnableWarriorRageModeEvent>(e)) {
		this->handleEnableWarriorRageModeEvent(enableWarriorRageModeEvent, p);
	}
	else if (std::shared_ptr<DecreaseRageModeMovesLeftEvent> decreaseRageModeMovesLeftEvent = std::dynamic_pointer_cast<DecreaseRageModeMovesLeftEvent>(e)) {
		this->handleDecreaseRageModeMovesLeftEvent(decreaseRageModeMovesLeftEvent, p);
	}
	else if (std::shared_ptr<RefreshAttackAbilityEvent> refreshAttackAbilityEvent = std::dynamic_pointer_cast<RefreshAttackAbilityEvent>(e)) {
		this->handleRefreshAttackAbilityEvent(refreshAttackAbilityEvent, p);
	}
	else if (std::shared_ptr<WipeAttackAbilityEvent> wipeAttackAbilityEvent = std::dynamic_pointer_cast<WipeAttackAbilityEvent>(e)) {
		this->handleWipeAttackAbilityEvent(wipeAttackAbilityEvent, p);
	}
	else if (std::shared_ptr<RefreshAttackedTableEvent> refreshAttackedTableEvent = std::dynamic_pointer_cast<RefreshAttackedTableEvent>(e)) {
		this->handleRefreshAttackedTableEvent(refreshAttackedTableEvent, p);
	}
	else if (std::shared_ptr<MarkAsAttackedEvent> markAsAttackedEvent = std::dynamic_pointer_cast<MarkAsAttackedEvent>(e)) {
		this->handleMarkAsAttackedEvent(markAsAttackedEvent, p);
	}
	else if (std::shared_ptr<RefreshHealingAbilityEvent> refreshHealingAbilityEvent = std::dynamic_pointer_cast<RefreshHealingAbilityEvent>(e)) {
		this->handleRefreshHealingAbilityEvent(refreshHealingAbilityEvent, p);
	}
	else if (std::shared_ptr<WipeHealingAbilityEvent> wipeHealingAbilityEvent = std::dynamic_pointer_cast<WipeHealingAbilityEvent>(e)) {
		this->handleWipeHealingAbilityEvent(wipeHealingAbilityEvent, p);
	}
	else if (std::shared_ptr<MarkPlayerAsInactiveEvent> markPlayerAsInactiveEvent = std::dynamic_pointer_cast<MarkPlayerAsInactiveEvent>(e)) {
		this->handleMarkPlayerAsInactiveEvent(markPlayerAsInactiveEvent, p);
	}
	else if (std::shared_ptr<IncreaseVCSMoveCtrEvent> increaseVcsMoveCtrEvent = std::dynamic_pointer_cast<IncreaseVCSMoveCtrEvent>(e)) {
		this->handleIncreaseVCSMoveCtrEvent(increaseVcsMoveCtrEvent, p);
	}
	else if (std::shared_ptr<SaveGameEvent> saveGameEvent = std::dynamic_pointer_cast<SaveGameEvent>(e)) {
		this->handleSaveGameEvent(saveGameEvent, p);
	}
	else if (std::shared_ptr<LimitResourcesEvent> limitResourcesEvent = std::dynamic_pointer_cast<LimitResourcesEvent>(e)) {
		this->handleLimitResourcesEvent(limitResourcesEvent, p);
	}
	else {
		p.logs->emplace_back("{unknown_event_handled}");
	}
}
void Room::handleAddResourceEvent(std::shared_ptr<AddResourceEvent> e, RoomOutputProtocol p) {
	this->getCurrentPlayer()->addResource(e->getResource(), e->getLimit().get(e->getResource().type));
}
void Room::handleSubResourceEvent(std::shared_ptr<SubResourceEvent> e, RoomOutputProtocol p) {
	this->getCurrentPlayer()->subResource(e->getResource());
}
void Room::handleAddResourcesEvent(std::shared_ptr<AddResourcesEvent> e, RoomOutputProtocol p) {
	this->getCurrentPlayer()->addResources(e->getResources(), e->getLimit());
}
void Room::handleSubResourcesEvent(std::shared_ptr<SubResourcesEvent> e, RoomOutputProtocol p) {
	this->getCurrentPlayer()->subResources(e->getResources());
}
void Room::handleSetHighlightEvent(std::shared_ptr<SetHighlightEvent> e, RoomOutputProtocol p) {
	this->highlightTable.mark(*e);
}
void Room::handleAddHpEvent(std::shared_ptr<AddHpEvent> e, RoomOutputProtocol p) {
	HPGO* go = e->getHPGO();
	uint32_t n = e->getN();
	go->addHp(n);
}
void Room::handleDecreaseCurrentTradeMovesLeft(std::shared_ptr<DecreaseCurrentTradeMovesLeftEvent> e, RoomOutputProtocol p) {
	e->getSpec()->decreaseCurrentTradeMovesLeft();
}
void Room::handleBuild(std::shared_ptr<BuildEvent> e, RoomOutputProtocol p) {
	Building* b = e->getBuilding();
	this->map.getStatePtr()->getCollectionsPtr()->add(b);
}
void Room::handlePlaySoundEvent(std::shared_ptr<PlaySoundEvent> e, RoomOutputProtocol p) {
	this->sendPlaySoundEventToClients(p, e->getSoundName());
}
void Room::handleCreatePopUpElementEvent(std::shared_ptr<CreateEEvent> e, RoomOutputProtocol p) {
	this->element = e->getElement();
	this->element->restart();
}
void Room::handleChangeMoveEvent(std::shared_ptr<ChangeMoveEvent> e, RoomOutputProtocol p) {
	this->changeMove();
}
void Room::handleReturnToMenuEvent(std::shared_ptr<ReturnToMenuEvent> e, RoomOutputProtocol p) {
	switch (e->getType()) {
	case ReturnToMenuEvent::Type::CURRENT_PLAYER:
		this->sendReturnToMenuToClient(p);
		break;
	case ReturnToMenuEvent::Type::EVERY_PLAYER:
		this->sendReturnToMenuToClients(p);
		break;
	}
}
void Room::handleDestroyEvent(std::shared_ptr<DestroyEvent> e, RoomOutputProtocol p) {
	Events destroyBuildingEvent = e->getBuilding()->destroy(this->map.getStatePtr());
	this->addEvents(destroyBuildingEvent, p);
}
void Room::handleDecreaseCurrentProdusingMovesLeftEvent(std::shared_ptr<DecreaseCurrentProducingMovesLeftEvent> e, RoomOutputProtocol p) {
	e->getSpec()->decreaseCurrentProducingMovesLeft();
}
void Room::handleWarriorProducingFinishedEvent(std::shared_ptr<WarriorProducingFinishedEvent> e, RoomOutputProtocol p) {
	e->getSpec()->stopProducing();
	this->map.getStatePtr()->getCollectionsPtr()->add(e->getWarrior()->cloneWarrior());
}
void Room::handleSelectEvent(std::shared_ptr<SelectEvent> e, RoomOutputProtocol p) {
	this->selected = e->getSelectable();
}
void Room::handleUnselectEvent(std::shared_ptr<UnselectEvent> e, RoomOutputProtocol p) {
	this->selected = nullptr;
}
void Room::handleStartWarriorAnimationEvent(std::shared_ptr<StartWarriorAnimationEvent> e, RoomOutputProtocol p) {
	e->getWarrior()->startAnimation(e->getAnimation());
}
void Room::handleRefreshMovementPointsEvent(std::shared_ptr<RefreshMovementPointsEvent> e, RoomOutputProtocol p) {
	e->getWarrior()->refreshMovementPoints();
}
void Room::handleEnableCursorEvent(std::shared_ptr<EnableCursorEvent> e, RoomOutputProtocol p) {
	this->curcorVisibility = true;
}
void Room::handleDisableCursorEvent(std::shared_ptr<DisableCursorEvent> e, RoomOutputProtocol p) {
	this->curcorVisibility = false;
}
void Room::handleCreateAnimationEvent(std::shared_ptr<CreateAnimationEvent> e, RoomOutputProtocol p) {
	this->animation = e->getAnimation();
}
void Room::handleDecreaseBurningMovesLeftEvent(std::shared_ptr<DecreaseBurningMovesLeftEvent> e, RoomOutputProtocol p) {
	e->getBuilding()->decreaseBurningMovesLeft();
}
void Room::handleSubHpEvent(std::shared_ptr<SubHpEvent> e, RoomOutputProtocol p) {
	e->getHPGO()->subHp(e->getValue());
}
void Room::handleSetFireEvent(std::shared_ptr<SetFireEvent> e, RoomOutputProtocol p) {
	e->getBuilding()->setFire();
}
void Room::handleChangeWarriorDirectionEvent(std::shared_ptr<ChangeWarriorDirectionEvent> e, RoomOutputProtocol p) {
	e->getWarrior()->changeDirection(e->getDirection());
}
void Room::handleFocusOnEvent(std::shared_ptr<FocusOnEvent> e, RoomOutputProtocol p) {
	this->sendFocusOnToClients(p, e->getX(), e->getY(), e->getSX(), e->getSY());
}
void Room::handleResetHighlightEvent(std::shared_ptr<ResetHighlightEvent> e, RoomOutputProtocol p) {
	this->highlightTable.clear();
}
void Room::handleDoTradeEvent(std::shared_ptr<DoTradeEvent> e, RoomOutputProtocol p) {
	Events events = e->getSpec()->doTrade(e->getBuilding(), e->getTrade());
	this->addEvents(events, p);
}
void Room::handleStartWarriorProducingEvent(std::shared_ptr<StartWarriorProducingEvent> e, RoomOutputProtocol p) {
	Events events = e->getSpec()->startProducing(e->getWarrior());
	this->addEvents(events, p);
}
void Room::handleTryToBuildEvent(std::shared_ptr<TryToBuildEvent> e, RoomOutputProtocol p) {
	if (this->getCurrentPlayer()->getResources() >= e->getBuilding()->getCost()) {
		this->bm = BuildingMode(e->getBuilding(), this->getCurrentPlayer()->getId());
		Events bmStartEvent = bm.start(this->map.getStatePtr());
		this->addEvents(bmStartEvent, p);
	}
	else {
		Events clickEvent;
		clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

		std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(StringLcl("{no_resources_for_building}"), StringLcl("{OK}"), clickEvent);
		Events unableToBuildEvent;
		unableToBuildEvent.add(std::make_shared<CreateEEvent>(w));
		this->addEvents(unableToBuildEvent, p);
	}
}
void Room::handleKillNextTurnEvent(std::shared_ptr<KillNextTurnEvent> e, RoomOutputProtocol p) {
	Events events = e->getWarrior()->killNextTurn();
	this->addEvents(events, p);
}
void Room::handleRevertKillNextTurnEvent(std::shared_ptr<RevertKillNextTurnEvent> e, RoomOutputProtocol p) {
	Events events = e->getWarrior()->revertKillNextTurn();
	this->addEvents(events, p);
}
void Room::handleCloseAnimationEvent(std::shared_ptr<CloseAnimationEvent> e, RoomOutputProtocol p) {
	this->animation = boost::none;
}
void Room::handleDecreaseSpellCreationMovesLeftEvent(std::shared_ptr<DecreaseSpellCreationMovesLeftEvent> e, RoomOutputProtocol p) {
	e->getSpell()->decreaseCreationMovesLeft();
}
void Room::handleSetSpellEvent(std::shared_ptr<SetSpellEvent> e, RoomOutputProtocol p) {
	e->getSpec()->setSpell(e->getSpell());
}
void Room::handleUseSpellEvent(std::shared_ptr<UseSpellEvent> e, RoomOutputProtocol p) {
	Events events = e->getSpell()->use();
	this->addEvents(events, p);
}
void Room::handleMarkSpellAsUsedEvent(std::shared_ptr<MarkSpellAsUsedEvent> e, RoomOutputProtocol p) {
	e->getSpell()->markAsUsed();
}
void Room::handleEnableWarriorRageModeEvent(std::shared_ptr<EnableWarriorRageModeEvent> e, RoomOutputProtocol p) {
	e->getWarrior()->enableRageMode();
}
void Room::handleDecreaseRageModeMovesLeftEvent(std::shared_ptr<DecreaseRageModeMovesLeftEvent> e, RoomOutputProtocol p) {
	e->getWarrior()->decreaseRageModeMovesLeft();
}
void Room::handleRefreshAttackAbilityEvent(std::shared_ptr<RefreshAttackAbilityEvent> e, RoomOutputProtocol p) {
	e->getI()->refreshAbility();
}
void Room::handleWipeAttackAbilityEvent(std::shared_ptr<WipeAttackAbilityEvent> e, RoomOutputProtocol p) {
	e->getI()->wipeAbility();
}
void Room::handleRefreshAttackedTableEvent(std::shared_ptr<RefreshAttackedTableEvent> e, RoomOutputProtocol p) {
	e->getWarrior()->refreshAttackedTable();
}
void Room::handleMarkAsAttackedEvent(std::shared_ptr<MarkAsAttackedEvent> e, RoomOutputProtocol p) {
	e->getAttacker()->markAsAttacked(e->getTarget());
}
void Room::handleRefreshHealingAbilityEvent(std::shared_ptr<RefreshHealingAbilityEvent> e, RoomOutputProtocol p) {
	e->getWarrior()->refreshHealingAbility();
}
void Room::handleWipeHealingAbilityEvent(std::shared_ptr<WipeHealingAbilityEvent> e, RoomOutputProtocol p) {
	e->getWarrior()->wipeHealingAbility();
}
void Room::handleMarkPlayerAsInactiveEvent(std::shared_ptr<MarkPlayerAsInactiveEvent> e, RoomOutputProtocol p) {
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
	this->addEvents(result, p);
}
void Room::handleIncreaseVCSMoveCtrEvent(std::shared_ptr<IncreaseVCSMoveCtrEvent> e, RoomOutputProtocol p) {
	e->getSpec()->increaseMoveCtr();
}
void Room::handleSaveGameEvent(std::shared_ptr<SaveGameEvent> e, RoomOutputProtocol p) {
	this->sendSaveToClient(p);
}
void Room::handleLimitResourcesEvent(std::shared_ptr<LimitResourcesEvent> e, RoomOutputProtocol p) {
	this->map.getStatePtr()->getPlayersPtr()->getPlayerPtr(e->getPlayerId())->limitResources(e->getLimit());
}