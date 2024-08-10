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


#include "Room.hpp"
#include "Maps.hpp"
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






Room::Room(const std::string& mapName) {
	Maps::get()->load(mapName, &this->map);
	this->playerIsActive.resize(this->map.getStatePtr()->getPlayersPtr()->total(), true);
	this->currentPlayerId = 1;
	this->move = 1;
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
}







void Room::newFrame(sf::UdpSocket& socket, const RemotePlayers& players) {
	// TODO handling clicks getting from remote players
	if (this->element != nullptr) {
		this->element->update();
		if (this->element->finished()) {
			this->element = nullptr;
		}
	}
	if (this->animation.has_value()) {
		Events animationEvent;
		animationEvent = this->animation.value().process(this->map.getStatePtr());
		this->addEvents(animationEvent);
	}
	for (uint32_t i = 0; i < this->map.getStatePtr()->getCollectionsPtr()->totalGOs(); i = i + 1) {
		this->map.getStatePtr()->getCollectionsPtr()->getGO(i, FILTER::DEFAULT_PRIORITY)->newFrame(this->map.getStatePtr(), this->getCurrentPlayer()->getId());
	}
	this->processNewMoveEvents();
	this->processBaseEvents();
}







void Room::processNewMoveEvents() {
	while (this->currentGOIndexNewMoveEvent != this->totalGONewMoveEvents) {
		if (this->element != nullptr or !this->events.empty()) {
			break;
		}
		Events newMoveEvent = this->map.getStatePtr()->getCollectionsPtr()->getGO(this->currentGOIndexNewMoveEvent, FILTER::NEW_MOVE_PRIORITY)->newMove(this->map.getStatePtr(), this->getCurrentPlayer()->getId());
		this->addEvents(newMoveEvent);
		this->currentGOIndexNewMoveEvent = this->currentGOIndexNewMoveEvent + 1;
		this->processBaseEvents();
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
void Room::addButtonClickEventToQueue() {
	// TODO
}
void Room::addGameObjectClickEventToQueue(uint8_t button) {
	// TODO
}
void Room::processBaseEvents() {
	while (!this->events.empty()) {
		if (this->element != nullptr or this->animation.has_value()) {
			break;
		}
		this->handleEvent(this->events.front());
		this->events.pop();
	}
}
void Room::addEvents(Events& e) {
	for (uint32_t i = 0; i < e.size(); i = i + 1) {
		std::shared_ptr<Event> event = e.at(i);
		if (event->isUrgent()) {
			this->handleEvent(event);
		}
		else {
			this->events.push(event);
		}
	}
}





void Room::handleEvent(std::shared_ptr<Event> e) {
	if (std::shared_ptr<AddResourceEvent> addResourceEvent = std::dynamic_pointer_cast<AddResourceEvent>(e)) {
		this->handleAddResourceEvent(addResourceEvent);
	}
	else if (std::shared_ptr<SubResourceEvent> subResourceEvent = std::dynamic_pointer_cast<SubResourceEvent>(e)) {
		this->handleSubResourceEvent(subResourceEvent);
	}
	else if (std::shared_ptr<AddResourcesEvent> addResourcesEvent = std::dynamic_pointer_cast<AddResourcesEvent>(e)) {
		this->handleAddResourcesEvent(addResourcesEvent);
	}
	else if (std::shared_ptr<SubResourcesEvent> subResourcesEvent = std::dynamic_pointer_cast<SubResourcesEvent>(e)) {
		this->handleSubResourcesEvent(subResourcesEvent);
	}
	else if (std::shared_ptr<SetHighlightEvent> changeHighlightEvent = std::dynamic_pointer_cast<SetHighlightEvent>(e)) {
		this->handleSetHighlightEvent(changeHighlightEvent);
	}
	else if (std::shared_ptr<AddHpEvent> addHpEvent = std::dynamic_pointer_cast<AddHpEvent>(e)) {
		this->handleAddHpEvent(addHpEvent);
	}
	else if (std::shared_ptr<DecreaseCurrentTradeMovesLeftEvent> decreaseCurrentTradeMovesLeftEvent = std::dynamic_pointer_cast<DecreaseCurrentTradeMovesLeftEvent>(e)) {
		this->handleDecreaseCurrentTradeMovesLeft(decreaseCurrentTradeMovesLeftEvent);
	}
	else if (std::shared_ptr<BuildEvent> buildEvent = std::dynamic_pointer_cast<BuildEvent>(e)) {
		this->handleBuild(buildEvent);
	}
	else if (std::shared_ptr<PlaySoundEvent> playSoundEvent = std::dynamic_pointer_cast<PlaySoundEvent>(e)) {
		this->handlePlaySoundEvent(playSoundEvent);
	}
	else if (std::shared_ptr<CreateEEvent> createEEvent = std::dynamic_pointer_cast<CreateEEvent>(e)) {
		this->handleCreatePopUpElementEvent(createEEvent);
	}
	else if (std::shared_ptr<ChangeMoveEvent> changeMoveEvent = std::dynamic_pointer_cast<ChangeMoveEvent>(e)) {
		this->handleChangeMoveEvent(changeMoveEvent);
	}
	else if (std::shared_ptr<ReturnToMenuEvent> returnToMenuEvent = std::dynamic_pointer_cast<ReturnToMenuEvent>(e)) {
		this->handleReturnToMenuEvent(returnToMenuEvent);
	}
	else if (std::shared_ptr<DestroyEvent> destroyEvent = std::dynamic_pointer_cast<DestroyEvent>(e)) {
		this->handleDestroyEvent(destroyEvent);
	}
	else if (std::shared_ptr<DecreaseCurrentProducingMovesLeftEvent> decreaseCurrentProducingMovesLeftEvent = std::dynamic_pointer_cast<DecreaseCurrentProducingMovesLeftEvent>(e)) {
		this->handleDecreaseCurrentProdusingMovesLeftEvent(decreaseCurrentProducingMovesLeftEvent);
	}
	else if (std::shared_ptr<WarriorProducingFinishedEvent> warriorProducingFinishedEvent = std::dynamic_pointer_cast<WarriorProducingFinishedEvent>(e)) {
		this->handleWarriorProducingFinishedEvent(warriorProducingFinishedEvent);
	}
	else if (std::shared_ptr<SelectEvent> selectEvent = std::dynamic_pointer_cast<SelectEvent>(e)) {
		this->handleSelectEvent(selectEvent);
	}
	else if (std::shared_ptr<UnselectEvent> unselectEvent = std::dynamic_pointer_cast<UnselectEvent>(e)) {
		this->handleUnselectEvent(unselectEvent);
	}
	else if (std::shared_ptr<StartWarriorAnimationEvent> startWarriorAnimationEvent = std::dynamic_pointer_cast<StartWarriorAnimationEvent>(e)) {
		this->handleStartWarriorAnimationEvent(startWarriorAnimationEvent);
	}
	else if (std::shared_ptr<RefreshMovementPointsEvent> refreshMovementPointsEvent = std::dynamic_pointer_cast<RefreshMovementPointsEvent>(e)) {
		this->handleRefreshMovementPointsEvent(refreshMovementPointsEvent);
	}
	else if (std::shared_ptr<EnableCursorEvent> enableCursorEvent = std::dynamic_pointer_cast<EnableCursorEvent>(e)) {
		this->handleEnableCursorEvent(enableCursorEvent);
	}
	else if (std::shared_ptr<DisableCursorEvent> disableCursorEvent = std::dynamic_pointer_cast<DisableCursorEvent>(e)) {
		this->handleDisableCursorEvent(disableCursorEvent);
	}
	else if (std::shared_ptr<CreateAnimationEvent> createAnimationEvent = std::dynamic_pointer_cast<CreateAnimationEvent>(e)) {
		this->handleCreateAnimationEvent(createAnimationEvent);
	}
	else if (std::shared_ptr<DecreaseBurningMovesLeftEvent> decreaseBurningMovesLeftEvent = std::dynamic_pointer_cast<DecreaseBurningMovesLeftEvent>(e)) {
		this->handleDecreaseBurningMovesLeftEvent(decreaseBurningMovesLeftEvent);
	}
	else if (std::shared_ptr<SubHpEvent> subHpEvent = std::dynamic_pointer_cast<SubHpEvent>(e)) {
		this->handleSubHpEvent(subHpEvent);
	}
	else if (std::shared_ptr<SetFireEvent> setFireEvent = std::dynamic_pointer_cast<SetFireEvent>(e)) {
		this->handleSetFireEvent(setFireEvent);
	}
	else if (std::shared_ptr<ChangeWarriorDirectionEvent> changeWarriorDirectionEvent = std::dynamic_pointer_cast<ChangeWarriorDirectionEvent>(e)) {
		this->handleChangeWarriorDirectionEvent(changeWarriorDirectionEvent);
	}
	else if (std::shared_ptr<FocusOnEvent> focusOnEvent = std::dynamic_pointer_cast<FocusOnEvent>(e)) {
		this->handleFocusOnEvent(focusOnEvent);
	}
	else if (std::shared_ptr<ResetHighlightEvent> resetHighlightEvent = std::dynamic_pointer_cast<ResetHighlightEvent>(e)) {
		this->handleResetHighlightEvent(resetHighlightEvent);
	}
	else if (std::shared_ptr<DoTradeEvent> doTradeEvent = std::dynamic_pointer_cast<DoTradeEvent>(e)) {
		this->handleDoTradeEvent(doTradeEvent);
	}
	else if (std::shared_ptr<StartWarriorProducingEvent> startWarriorProducingEvent = std::dynamic_pointer_cast<StartWarriorProducingEvent>(e)) {
		this->handleStartWarriorProducingEvent(startWarriorProducingEvent);
	}
	else if (std::shared_ptr<TryToBuildEvent> tryToBuildEvent = std::dynamic_pointer_cast<TryToBuildEvent>(e)) {
		this->handleTryToBuildEvent(tryToBuildEvent);
	}
	else if (std::shared_ptr<KillNextTurnEvent> killNextTurnEvent = std::dynamic_pointer_cast<KillNextTurnEvent>(e)) {
		this->handleKillNextTurnEvent(killNextTurnEvent);
	}
	else if (std::shared_ptr<RevertKillNextTurnEvent> revertKillNextTurnEvent = std::dynamic_pointer_cast<RevertKillNextTurnEvent>(e)) {
		this->handleRevertKillNextTurnEvent(revertKillNextTurnEvent);
	}
	else if (std::shared_ptr<CloseAnimationEvent> closeAnimationEvent = std::dynamic_pointer_cast<CloseAnimationEvent>(e)) {
		this->handleCloseAnimationEvent(closeAnimationEvent);
	}
	else if (std::shared_ptr<DecreaseSpellCreationMovesLeftEvent> decreaseSpellCreationMovesLeftEvent = std::dynamic_pointer_cast<DecreaseSpellCreationMovesLeftEvent>(e)) {
		this->handleDecreaseSpellCreationMovesLeftEvent(decreaseSpellCreationMovesLeftEvent);
	}
	else if (std::shared_ptr<SetSpellEvent> setSpellEvent = std::dynamic_pointer_cast<SetSpellEvent>(e)) {
		this->handleSetSpellEvent(setSpellEvent);
	}
	else if (std::shared_ptr<UseSpellEvent> useSpellEvent = std::dynamic_pointer_cast<UseSpellEvent>(e)) {
		this->handleUseSpellEvent(useSpellEvent);
	}
	else if (std::shared_ptr<MarkSpellAsUsedEvent> markSpellAsUsedEvent = std::dynamic_pointer_cast<MarkSpellAsUsedEvent>(e)) {
		this->handleMarkSpellAsUsedEvent(markSpellAsUsedEvent);
	}
	else if (std::shared_ptr<EnableWarriorRageModeEvent> enableWarriorRageModeEvent = std::dynamic_pointer_cast<EnableWarriorRageModeEvent>(e)) {
		this->handleEnableWarriorRageModeEvent(enableWarriorRageModeEvent);
	}
	else if (std::shared_ptr<DecreaseRageModeMovesLeftEvent> decreaseRageModeMovesLeftEvent = std::dynamic_pointer_cast<DecreaseRageModeMovesLeftEvent>(e)) {
		this->handleDecreaseRageModeMovesLeftEvent(decreaseRageModeMovesLeftEvent);
	}
	else if (std::shared_ptr<RefreshAttackAbilityEvent> refreshAttackAbilityEvent = std::dynamic_pointer_cast<RefreshAttackAbilityEvent>(e)) {
		this->handleRefreshAttackAbilityEvent(refreshAttackAbilityEvent);
	}
	else if (std::shared_ptr<WipeAttackAbilityEvent> wipeAttackAbilityEvent = std::dynamic_pointer_cast<WipeAttackAbilityEvent>(e)) {
		this->handleWipeAttackAbilityEvent(wipeAttackAbilityEvent);
	}
	else if (std::shared_ptr<RefreshAttackedTableEvent> refreshAttackedTableEvent = std::dynamic_pointer_cast<RefreshAttackedTableEvent>(e)) {
		this->handleRefreshAttackedTableEvent(refreshAttackedTableEvent);
	}
	else if (std::shared_ptr<MarkAsAttackedEvent> markAsAttackedEvent = std::dynamic_pointer_cast<MarkAsAttackedEvent>(e)) {
		this->handleMarkAsAttackedEvent(markAsAttackedEvent);
	}
	else if (std::shared_ptr<RefreshHealingAbilityEvent> refreshHealingAbilityEvent = std::dynamic_pointer_cast<RefreshHealingAbilityEvent>(e)) {
		this->handleRefreshHealingAbilityEvent(refreshHealingAbilityEvent);
	}
	else if (std::shared_ptr<WipeHealingAbilityEvent> wipeHealingAbilityEvent = std::dynamic_pointer_cast<WipeHealingAbilityEvent>(e)) {
		this->handleWipeHealingAbilityEvent(wipeHealingAbilityEvent);
	}
	else if (std::shared_ptr<MarkPlayerAsInactiveEvent> markPlayerAsInactiveEvent = std::dynamic_pointer_cast<MarkPlayerAsInactiveEvent>(e)) {
		this->handleMarkPlayerAsInactiveEvent(markPlayerAsInactiveEvent);
	}
	else if (std::shared_ptr<IncreaseVCSMoveCtrEvent> increaseVcsMoveCtrEvent = std::dynamic_pointer_cast<IncreaseVCSMoveCtrEvent>(e)) {
		this->handleIncreaseVCSMoveCtrEvent(increaseVcsMoveCtrEvent);
	}
	else if (std::shared_ptr<SaveGameEvent> saveGameEvent = std::dynamic_pointer_cast<SaveGameEvent>(e)) {
		this->handleSaveGameEvent(saveGameEvent);
	}
	else if (std::shared_ptr<LimitResourcesEvent> limitResourcesEvent = std::dynamic_pointer_cast<LimitResourcesEvent>(e)) {
		this->handleLimitResourcesEvent(limitResourcesEvent);
	}
}
void Room::handleAddResourceEvent(std::shared_ptr<AddResourceEvent> e) {
	this->getCurrentPlayer()->addResource(e->getResource(), e->getLimit().get(e->getResource().type));
}
void Room::handleSubResourceEvent(std::shared_ptr<SubResourceEvent> e) {
	this->getCurrentPlayer()->subResource(e->getResource());
}
void Room::handleAddResourcesEvent(std::shared_ptr<AddResourcesEvent> e) {
	this->getCurrentPlayer()->addResources(e->getResources(), e->getLimit());
}
void Room::handleSubResourcesEvent(std::shared_ptr<SubResourcesEvent> e) {
	this->getCurrentPlayer()->subResources(e->getResources());
}
void Room::handleSetHighlightEvent(std::shared_ptr<SetHighlightEvent> e) {
	this->highlightTable.mark(*e);
}
void Room::handleAddHpEvent(std::shared_ptr<AddHpEvent> e) {
	HPGO* go = e->getHPGO();
	uint32_t n = e->getN();
	go->addHp(n);
}
void Room::handleDecreaseCurrentTradeMovesLeft(std::shared_ptr<DecreaseCurrentTradeMovesLeftEvent> e) {
	e->getSpec()->decreaseCurrentTradeMovesLeft();
}
void Room::handleBuild(std::shared_ptr<BuildEvent> e) {
	Building* b = e->getBuilding();
	this->map.getStatePtr()->getCollectionsPtr()->add(b);
}
void Room::handlePlaySoundEvent(std::shared_ptr<PlaySoundEvent> e) {
	// TODO
}
void Room::handleCreatePopUpElementEvent(std::shared_ptr<CreateEEvent> e) {
	this->element = e->getElement();
	this->element->restart();
}
void Room::handleChangeMoveEvent(std::shared_ptr<ChangeMoveEvent> e) {
	this->changeMove();
}
void Room::handleReturnToMenuEvent(std::shared_ptr<ReturnToMenuEvent> e) {
	// TODO
}
void Room::handleDestroyEvent(std::shared_ptr<DestroyEvent> e) {
	Events destroyBuildingEvent = e->getBuilding()->destroy(this->map.getStatePtr());
	this->addEvents(destroyBuildingEvent);
}
void Room::handleDecreaseCurrentProdusingMovesLeftEvent(std::shared_ptr<DecreaseCurrentProducingMovesLeftEvent> e) {
	e->getSpec()->decreaseCurrentProducingMovesLeft();
}
void Room::handleWarriorProducingFinishedEvent(std::shared_ptr<WarriorProducingFinishedEvent> e) {
	e->getSpec()->stopProducing();
	this->map.getStatePtr()->getCollectionsPtr()->add(e->getWarrior()->cloneWarrior());
}
void Room::handleSelectEvent(std::shared_ptr<SelectEvent> e) {
	this->selected = e->getSelectable();
}
void Room::handleUnselectEvent(std::shared_ptr<UnselectEvent> e) {
	this->selected = nullptr;
}
void Room::handleStartWarriorAnimationEvent(std::shared_ptr<StartWarriorAnimationEvent> e) {
	e->getWarrior()->startAnimation(e->getAnimation());
}
void Room::handleRefreshMovementPointsEvent(std::shared_ptr<RefreshMovementPointsEvent> e) {
	e->getWarrior()->refreshMovementPoints();
}
void Room::handleEnableCursorEvent(std::shared_ptr<EnableCursorEvent> e) {
	this->curcorVisibility = true;
}
void Room::handleDisableCursorEvent(std::shared_ptr<DisableCursorEvent> e) {
	this->curcorVisibility = false;
}
void Room::handleCreateAnimationEvent(std::shared_ptr<CreateAnimationEvent> e) {
	this->animation = e->getAnimation();
}
void Room::handleDecreaseBurningMovesLeftEvent(std::shared_ptr<DecreaseBurningMovesLeftEvent> e) {
	e->getBuilding()->decreaseBurningMovesLeft();
}
void Room::handleSubHpEvent(std::shared_ptr<SubHpEvent> e) {
	e->getHPGO()->subHp(e->getValue());
}
void Room::handleSetFireEvent(std::shared_ptr<SetFireEvent> e) {
	e->getBuilding()->setFire();
}
void Room::handleChangeWarriorDirectionEvent(std::shared_ptr<ChangeWarriorDirectionEvent> e) {
	e->getWarrior()->changeDirection(e->getDirection());
}
void Room::handleFocusOnEvent(std::shared_ptr<FocusOnEvent> e) {
	// TODO
}
void Room::handleResetHighlightEvent(std::shared_ptr<ResetHighlightEvent> e) {
	this->highlightTable.clear();
}
void Room::handleDoTradeEvent(std::shared_ptr<DoTradeEvent> e) {
	Events events = e->getSpec()->doTrade(e->getBuilding(), e->getTrade());
	this->addEvents(events);
}
void Room::handleStartWarriorProducingEvent(std::shared_ptr<StartWarriorProducingEvent> e) {
	Events events = e->getSpec()->startProducing(e->getWarrior());
	this->addEvents(events);
}
void Room::handleTryToBuildEvent(std::shared_ptr<TryToBuildEvent> e) {
	if (this->getCurrentPlayer()->getResources() >= e->getBuilding()->getCost()) {
		this->bm = BuildingMode(e->getBuilding(), this->getCurrentPlayer()->getId());
		Events bmStartEvent = bm.start(this->map.getStatePtr());
		this->addEvents(bmStartEvent);
	}
	else {
		Events clickEvent;
		clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

		std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(StringLcl("{no_resources_for_building}"), StringLcl("{OK}"), clickEvent);
		Events unableToBuildEvent;
		unableToBuildEvent.add(std::make_shared<CreateEEvent>(w));
		this->addEvents(unableToBuildEvent);
	}
}
void Room::handleKillNextTurnEvent(std::shared_ptr<KillNextTurnEvent> e) {
	Events events = e->getWarrior()->killNextTurn();
	this->addEvents(events);
}
void Room::handleRevertKillNextTurnEvent(std::shared_ptr<RevertKillNextTurnEvent> e) {
	Events events = e->getWarrior()->revertKillNextTurn();
	this->addEvents(events);
}
void Room::handleCloseAnimationEvent(std::shared_ptr<CloseAnimationEvent> e) {
	this->animation = boost::none;
}
void Room::handleDecreaseSpellCreationMovesLeftEvent(std::shared_ptr<DecreaseSpellCreationMovesLeftEvent> e) {
	e->getSpell()->decreaseCreationMovesLeft();
}
void Room::handleSetSpellEvent(std::shared_ptr<SetSpellEvent> e) {
	e->getSpec()->setSpell(e->getSpell());
}
void Room::handleUseSpellEvent(std::shared_ptr<UseSpellEvent> e) {
	Events events = e->getSpell()->use();
	this->addEvents(events);
}
void Room::handleMarkSpellAsUsedEvent(std::shared_ptr<MarkSpellAsUsedEvent> e) {
	e->getSpell()->markAsUsed();
}
void Room::handleEnableWarriorRageModeEvent(std::shared_ptr<EnableWarriorRageModeEvent> e) {
	e->getWarrior()->enableRageMode();
}
void Room::handleDecreaseRageModeMovesLeftEvent(std::shared_ptr<DecreaseRageModeMovesLeftEvent> e) {
	e->getWarrior()->decreaseRageModeMovesLeft();
}
void Room::handleRefreshAttackAbilityEvent(std::shared_ptr<RefreshAttackAbilityEvent> e) {
	e->getI()->refreshAbility();
}
void Room::handleWipeAttackAbilityEvent(std::shared_ptr<WipeAttackAbilityEvent> e) {
	e->getI()->wipeAbility();
}
void Room::handleRefreshAttackedTableEvent(std::shared_ptr<RefreshAttackedTableEvent> e) {
	e->getWarrior()->refreshAttackedTable();
}
void Room::handleMarkAsAttackedEvent(std::shared_ptr<MarkAsAttackedEvent> e) {
	e->getAttacker()->markAsAttacked(e->getTarget());
}
void Room::handleRefreshHealingAbilityEvent(std::shared_ptr<RefreshHealingAbilityEvent> e) {
	e->getWarrior()->refreshHealingAbility();
}
void Room::handleWipeHealingAbilityEvent(std::shared_ptr<WipeHealingAbilityEvent> e) {
	e->getWarrior()->wipeHealingAbility();
}
void Room::handleMarkPlayerAsInactiveEvent(std::shared_ptr<MarkPlayerAsInactiveEvent> e) {
	this->playerIsActive[e->getPlayerId() - 1] = false;
	uint32_t count = 0;
	for (uint32_t i = 0; i < this->playerIsActive.size(); i = i + 1) {
		count = count + this->playerIsActive.at(i);
	}
	std::shared_ptr<WindowButton> w;
	if (count == 1) {
		Events returnToMenuEvent;
		returnToMenuEvent.add(std::make_shared<PlaySoundEvent>("click"));
		returnToMenuEvent.add(std::make_shared<ReturnToMenuEvent>());
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
	std::shared_ptr<CreateEEvent> createW = std::make_shared<CreateEEvent>(w);
	this->handleCreatePopUpElementEvent(createW);
}
void Room::handleIncreaseVCSMoveCtrEvent(std::shared_ptr<IncreaseVCSMoveCtrEvent> e) {
	e->getSpec()->increaseMoveCtr();
}
void Room::handleSaveGameEvent(std::shared_ptr<SaveGameEvent> e) {
	// TODO
}
void Room::handleLimitResourcesEvent(std::shared_ptr<LimitResourcesEvent> e) {
	this->map.getStatePtr()->getPlayersPtr()->getPlayerPtr(e->getPlayerId())->limitResources(e->getLimit());
}