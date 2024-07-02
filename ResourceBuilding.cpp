/*
 *  Conquesta
 *  Copyright (C) 2024 mr2anderson
 *
 *  Conquesta is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Conquesta is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Conquesta.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "ResourceBuilding.hpp"


ResourceBuilding::ResourceBuilding(uint32_t x, uint32_t y, uint32_t maxHp, const Player* playerPtr, const std::vector<ResourcePoint*>* resourcePointsPtr) : HpSensitiveBuilding(x, y, maxHp, false, playerPtr) {
	this->resourcePointsPtr = resourcePointsPtr;
	this->resourcesLeft = true;
}
GameObjectResponse ResourceBuilding::newMove(const Player& currentPlayer, uint32_t windowW, uint32_t windowH) {
	GameObjectResponse response;
	if (this->belongTo(&currentPlayer) and this->exist()) {
		response = this->decreaseUpgradeMovesLeft();
		if (this->upgrading()) {
			return response;
		}
		response = response + this->processRegeneration();
		if (this->works() and this->resourcesLeft) {
			response = response + this->collectResources();
		}
		return response;
	}
	return GameObjectResponse();
}
GameObjectResponse ResourceBuilding::upgrade() {
	GameObjectResponse response;
	response.gameEvent = GameEvent();
	response.gameEvent.value().subResources.push_back(this->getUpgradeCost(this->getCurrentLevel() - 1));

	this->Building::upgrade(this->getUpgradeMoves(this->getCurrentLevel() - 1));

	response = response + this->handleUpgradeStart();

	return response;
}
GameObjectResponse ResourceBuilding::collectResources() {
	GameObjectResponse response;
	response.gameEvent = GameEvent();
	uint32_t left = this->getResourceNPerMove();
	for (uint32_t i = 0; i < this->resourcePointsPtr->size() and left; i = i + 1) {
		ResourcePoint* rp = this->resourcePointsPtr->at(i);
		if (rp->getResourceType() == this->getResourceType() and this->inRadius(rp)) {
			uint32_t got = std::min(left, rp->getHP());
			left = left - got;
			response.gameEvent.value().collect.emplace_back(rp, got);
		}
	}
	if (left > 0) {
		this->resourcesLeft = false;
		MessageWindow* window = new MessageWindow(this->getNewWindowSoundName(), "click", this->getResourcesOverStr());
		response.popUpWindows.push(window);
	}
	return response;
}
GameObjectResponse ResourceBuilding::highlightArea() const {
	GameObjectResponse response;
	response.gameEvent = GameEvent();

	uint32_t x1 = this->getX() + this->getRadius();
	uint32_t y1 = this->getY() + this->getRadius();
	uint32_t x2 = 0;
	uint32_t y2 = 0;
	if (this->getX() > this->getRadius()) {
		x2 = this->getX() - this->getRadius();
	}
	if (this->getY() > this->getRadius()) {
		y2 = this->getY() - this->getRadius();
	}

	for (uint32_t x = x2; x <= x1; x = x + 1) {
		for (uint32_t y = y2; y <= y1; y = y + 1) {
			response.gameEvent.value().changeHighlight.emplace_back(this, x, y);
		}
	}

	return response;
}
GameObjectResponse ResourceBuilding::getSelectWindow(const GameEvent& highlightEvent) {
	GameObjectResponse response;

	std::vector<std::tuple<std::string, std::wstring, bool, GameEvent>> data;
	data.emplace_back("exit", L"Вернуться", true, highlightEvent);
	data.emplace_back(this->getTextureName(), this->getDescription(), false, GameEvent());

	if (this->getCurrentLevel() < this->getMaxLevel()) {
		GameEvent gameEventUpgrade = highlightEvent;
		gameEventUpgrade.tryToUpgrade.emplace_back(this, this->getUpgradeCost(this->getCurrentLevel() - 1));
		data.emplace_back("upgrade", L"Увеличить скорость добычи за " + this->getUpgradeCost(this->getCurrentLevel() - 1).getReadableInfo(), true, gameEventUpgrade);
	}

	SelectWindow* window = new SelectWindow(this->getNewWindowSoundName(), "click", data);
	response.popUpWindows.push(window);

	return response;
}
GameObjectResponse ResourceBuilding::getGameObjectResponse(const Player& player, uint32_t windowW, uint32_t windowH) {
	if (!this->exist()) {
		return GameObjectResponse();
	}
	if (this->belongTo(&player)) {
		if (this->upgrading()) {
			return this->handleUpgrading();
		}
		if (!this->works()) {
			return this->handleDoesNotWork();
		}
		GameObjectResponse response = this->highlightArea();
		response = response + this->getSelectWindow(response.gameEvent.value());
		return response;
	}
	return this->getUnitOfEnemyResponse();
}
bool ResourceBuilding::inRadius(ResourcePoint* rp) const {
	uint32_t x1 = this->getX();
	uint32_t y1 = this->getY();

	uint32_t x2 = rp->getX();
	uint32_t y2 = rp->getY();

	uint32_t dx = std::max(x1, x2) - std::min(x1, x2);
	uint32_t dy = std::max(y1, y2) - std::min(y1, y2);

	return (dx <= this->getRadius() and dy <= this->getRadius());
}