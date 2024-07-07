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


#include <vector>
#include <tuple>
#include <memory>
#include "Trade.hpp"
#include "Resources.hpp"


#pragma once


class Unit;
class UpgradeableB;
class Market;
class ResourcePoint;
class HPGO;
class Building;
class PopUpElement;


class Event {
public:
	Event();

	friend Event operator+(const Event &a, const Event& b);
    bool empty() const;

    const std::vector<Unit*>* getTryToAttackEvent() const;
    const std::vector<std::tuple<Market*, Trade>>* getTryToTradeEvent() const;
    const std::vector<Resource>* getSubResourceEvent() const;
    const std::vector<Resource>* getAddResourceEvent() const;
    const std::vector<Resources>* getSubResourcesEvent() const;
    const std::vector<Resources>* getAddResourcesEvent() const;
    const std::vector<std::tuple<const Unit*, uint32_t, uint32_t>>* getChangeHighlightEvent() const;
    const std::vector<std::tuple<ResourcePoint*, uint32_t>>* getCollectEvent() const;
    const std::vector<std::tuple<UpgradeableB*, Resources>>* getTryToUpgradeEvent() const;
    const std::vector<std::tuple<HPGO*, uint32_t>>* getAddHpEvent() const;
    const std::vector<UpgradeableB*>* getDecreaseUpgradeMovesLeftEvent() const;
    const std::vector<UpgradeableB*>* getIncreaseLevelEvent() const;
    const std::vector<Market*>* getDecreaseCurrentTradeMovesLeftEvent() const;
    const std::vector<Building*>* getTryToBuildEvent() const;
    const std::vector<Building*>* getBuildEvent() const;
    const std::vector<std::string>* getPlaySoundEvent() const;
    const std::vector<std::shared_ptr<PopUpElement>>* getCreateEEvent() const;
    uint32_t getChangeMoveEvent() const;
    bool getCloseThisWindowEvent() const;
    bool getExitEvent() const;
    bool getStartGameEvent() const;
    bool getReturnToMenuEvent() const;

    void addTryToAttackEvent(Unit* unit);
    void addTryToTradeEvent(const std::tuple<Market*, Trade> &trade);
    void addSubResourceEvent(const Resource &resource);
    void addAddResourceEvent(const Resource &resource);
    void addSubResourcesEvent(const Resources &resources);
    void addAddResourcesEvent(const Resources &resources);
    void addChangeHighlightEvent(std::tuple<const Unit*, uint32_t, uint32_t> highlight);
    void addCollectEvent(std::tuple<ResourcePoint*, uint32_t> toCollect);
    void addTryToUpgradeEvent(const std::tuple<UpgradeableB*, Resources>& toUpgrade);
    void addAddHpEvent(std::tuple<HPGO*, uint32_t> addHp);
    void addDecreaseUpgradeMovesLeftEvent(UpgradeableB *b);
    void addIncreaseLevelEvent(UpgradeableB *b);
    void addDecreaseCurrentTradeMovesLeftEvent(Market *m);
    void addTryToBuildEvent(Building *b);
    void addBuildEvent(Building *b);
    void addPlaySoundEvent(const std::string &soundName);
    void addCreateEEvent(std::shared_ptr<PopUpElement> e);
    void addChangeMoveEvent();
    void addCloseThisWindowEvent();
    void addExitEvent();
    void addStartGameEvent();
    void addReturnToMenuEvent();
private:
    bool _empty;

	std::vector<Unit*> tryToAttack;
	std::vector<std::tuple<Market*, Trade>> tryToTrade;
	std::vector<Resource> subResource;
	std::vector<Resource> addResource;
	std::vector<Resources> subResources;
	std::vector<Resources> addResources;
	std::vector<std::tuple<const Unit*, uint32_t, uint32_t>> changeHighlight;
	std::vector<std::tuple<ResourcePoint*, uint32_t>> collect;
	std::vector<std::tuple<UpgradeableB*, Resources>> tryToUpgrade;
	std::vector<std::tuple<HPGO*, uint32_t>> addHp;
	std::vector<UpgradeableB*> decreaseUpgradeMovesLeft;
	std::vector<UpgradeableB*> increaseLevel;
	std::vector<Market*> decreaseCurrentTradeMovesLeft;
	std::vector<Building*> tryToBuild;
	std::vector<Building*> build;
	std::vector<std::string> playSound;
	std::vector<std::shared_ptr<PopUpElement>> createE;
    uint32_t changeMove;
    bool closeThisWindow;
    bool exit;
    bool startGame;
    bool returnToMenu;

    void onNewEvent();
};