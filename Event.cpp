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



#include "Event.hpp"


Event::Event() {
    this->_empty = true;

    this->changeMove = 0;
    this->closeThisWindow = false;
    this->exit = false;
    this->startGame = false;
}
Event operator+(const Event &a, const Event& b) {
	Event c = a;

    c._empty = c._empty and b._empty;

	c.tryToAttack.insert(c.tryToAttack.end(), b.tryToAttack.begin(), b.tryToAttack.end());
	c.tryToTrade.insert(c.tryToTrade.end(), b.tryToTrade.begin(), b.tryToTrade.end());
	c.subResource.insert(c.subResource.end(), b.subResource.begin(), b.subResource.end());
	c.addResource.insert(c.addResource.end(), b.addResource.begin(), b.addResource.end());
	c.subResources.insert(c.subResources.end(), b.subResources.begin(), b.subResources.end());
	c.addResources.insert(c.addResources.end(), b.addResources.begin(), b.addResources.end());
	c.changeHighlight.insert(c.changeHighlight.end(), b.changeHighlight.begin(), b.changeHighlight.end());
	c.collect.insert(c.collect.end(), b.collect.begin(), b.collect.end());
	c.tryToUpgrade.insert(c.tryToUpgrade.end(), b.tryToUpgrade.begin(), b.tryToUpgrade.end());
	c.addHp.insert(c.addHp.end(), b.addHp.begin(), b.addHp.end());
	c.decreaseUpgradeMovesLeft.insert(c.decreaseUpgradeMovesLeft.end(), b.decreaseUpgradeMovesLeft.begin(), b.decreaseUpgradeMovesLeft.end());
	c.increaseLevel.insert(c.increaseLevel.end(), b.increaseLevel.begin(), b.increaseLevel.end());
	c.decreaseCurrentTradeMovesLeft.insert(c.decreaseCurrentTradeMovesLeft.end(), b.decreaseCurrentTradeMovesLeft.begin(), b.decreaseCurrentTradeMovesLeft.end());
	c.tryToBuild.insert(c.tryToBuild.end(), b.tryToBuild.begin(), b.tryToBuild.end());
	c.build.insert(c.build.end(), b.build.begin(), b.build.end());
	c.playSound.insert(c.playSound.end(), b.playSound.begin(), b.playSound.end());
	c.createE.insert(c.createE.end(), b.createE.begin(), b.createE.end());
    c.changeMove = c.changeMove + b.changeMove;
    c.closeThisWindow = c.closeThisWindow or b.closeThisWindow;
    c.exit = c.exit or b.exit;
    c.startGame = c.startGame or b.startGame;
    c.returnToMenu = c.returnToMenu or b.returnToMenu;

	return c;
}
bool Event::empty() const {
    return this->_empty;
}
const std::vector<Unit *> *Event::getTryToAttackEvent() const {
    return &this->tryToAttack;
}
const std::vector<std::tuple<Market *, Trade>> *Event::getTryToTradeEvent() const {
    return &this->tryToTrade;
}
const std::vector<Resource> *Event::getSubResourceEvent() const {
    return &this->subResource;
}
const std::vector<Resource> *Event::getAddResourceEvent() const {
    return &this->addResource;
}
const std::vector<Resources> *Event::getSubResourcesEvent() const {
    return &this->subResources;
}
const std::vector<Resources> *Event::getAddResourcesEvent() const {
    return &this->addResources;
}
const std::vector<std::tuple<const Unit *, uint32_t, uint32_t>> *Event::getChangeHighlightEvent() const {
    return &this->changeHighlight;
}
const std::vector<std::tuple<ResourcePoint *, uint32_t>> *Event::getCollectEvent() const {
    return &this->collect;
}
const std::vector<std::tuple<UpgradeableB *, Resources>> *Event::getTryToUpgradeEvent() const {
    return &this->tryToUpgrade;
}
const std::vector<std::tuple<HPGO *, uint32_t>> *Event::getAddHpEvent() const {
    return &this->addHp;
}
const std::vector<UpgradeableB *> *Event::getDecreaseUpgradeMovesLeftEvent() const {
    return &this->decreaseUpgradeMovesLeft;
}
const std::vector<UpgradeableB *> *Event::getIncreaseLevelEvent() const {
    return &this->increaseLevel;
}
const std::vector<Market *> *Event::getDecreaseCurrentTradeMovesLeftEvent() const {
    return &this->decreaseCurrentTradeMovesLeft;
}
const std::vector<Building *> *Event::getTryToBuildEvent() const {
    return &this->tryToBuild;
}
const std::vector<Building *> *Event::getBuildEvent() const {
    return &this->build;
}
const std::vector<std::string> *Event::getPlaySoundEvent() const {
    return &this->playSound;
}
const std::vector<std::shared_ptr<PopUpElement>> *Event::getCreateEEvent() const {
    return &this->createE;
}
uint32_t Event::getChangeMoveEvent() const {
    return this->changeMove;
}
bool Event::getCloseThisWindowEvent() const {
    return this->closeThisWindow;
}
bool Event::getExitEvent() const {
    return this->exit;
}
bool Event::getStartGameEvent() const {
    return this->startGame;
}
bool Event::getReturnToMenuEvent() const {
    return this->returnToMenu;
}
void Event::addTryToAttackEvent(Unit *unit) {
    this->onNewEvent();
    this->tryToAttack.push_back(unit);
}
void Event::addTryToTradeEvent(const std::tuple<Market *, Trade> &trade) {
    this->onNewEvent();
    this->tryToTrade.push_back(trade);
}
void Event::addSubResourceEvent(const Resource &resource) {
    this->onNewEvent();
    this->subResource.push_back(resource);
}
void Event::addAddResourceEvent(const Resource &resource) {
    this->onNewEvent();
    this->addResource.push_back(resource);
}
void Event::addSubResourcesEvent(const Resources &resources) {
    this->onNewEvent();
    this->subResources.push_back(resources);
}
void Event::addAddResourcesEvent(const Resources &resources) {
    this->onNewEvent();
    this->addResources.push_back(resources);
}
void Event::addChangeHighlightEvent(std::tuple<const Unit *, uint32_t, uint32_t> highlight) {
    this->onNewEvent();
    this->changeHighlight.push_back(highlight);
}
void Event::addCollectEvent(std::tuple<ResourcePoint *, uint32_t> toCollect) {
    this->onNewEvent();
    this->collect.push_back(toCollect);
}
void Event::addTryToUpgradeEvent(const std::tuple<UpgradeableB*, Resources> &toUpgrade) {
    this->onNewEvent();
    this->tryToUpgrade.push_back(toUpgrade);
}
void Event::addAddHpEvent(std::tuple<HPGO *, uint32_t> hp) {
    this->onNewEvent();
    this->addHp.push_back(hp);
}
void Event::addDecreaseUpgradeMovesLeftEvent(UpgradeableB *b) {
    this->onNewEvent();
    this->decreaseUpgradeMovesLeft.push_back(b);
}
void Event::addIncreaseLevelEvent(UpgradeableB *b) {
    this->onNewEvent();
    this->increaseLevel.push_back(b);
}
void Event::addDecreaseCurrentTradeMovesLeftEvent(Market *m) {
    this->onNewEvent();
    this->decreaseCurrentTradeMovesLeft.push_back(m);
}
void Event::addTryToBuildEvent(Building *b) {
    this->onNewEvent();
    this->tryToBuild.push_back(b);
}
void Event::addBuildEvent(Building *b) {
    this->onNewEvent();
    this->build.push_back(b);
}
void Event::addPlaySoundEvent(const std::string &soundName) {
    this->onNewEvent();
    this->playSound.push_back(soundName);
}
void Event::addCreateEEvent(std::shared_ptr<PopUpElement> e) {
    this->onNewEvent();
    this->createE.push_back(e);
}
void Event::addChangeMoveEvent() {
    this->onNewEvent();
    this->changeMove = this->changeMove + 1;
}
void Event::addCloseThisWindowEvent() {
    this->onNewEvent();
    this->closeThisWindow = true;
}
void Event::addExitEvent() {
    this->onNewEvent();
    this->exit = true;
}
void Event::addStartGameEvent() {
    this->onNewEvent();
    this->startGame = true;
}
void Event::addReturnToMenuEvent() {
    this->onNewEvent();
    this->returnToMenu = true;
}
void Event::onNewEvent() {
    this->_empty = false;
}