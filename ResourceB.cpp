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


#include "ResourceB.hpp"


ResourceB::ResourceB() = default;
ResourceB::ResourceB(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, const Player* playerPtr, const std::vector<ResourcePoint*>* resourcePointsPtr) : 
	UpgradeableHpSensitiveB(x, y, sx, sy, maxHp, playerPtr),
	Building(x, y, sx, sy, maxHp, playerPtr) {
	this->resourcePointsPtr = resourcePointsPtr;
	this->resourcesLeft = true;
}
GOR ResourceB::newMove(const Player& currentPlayer) {
	GOR response;
	if (this->belongTo(&currentPlayer) and this->exist()) {
		response = this->handleCurrentUpgrade();
		if (this->upgrading()) {
			return response;
		}
		response = response + this->regenerate();
		if (this->works() and this->resourcesLeft) {
			response = response + this->collectResources();
		}
		return response;
	}
	return GOR();
}
uint32_t ResourceB::getCollectionSpeed() const {
	return this->getCollectionSpeed(this->getCurrentLevel() - 1);
}
uint32_t ResourceB::getRadius() const {
	return this->getRadius(this->getCurrentLevel() - 1);
}
bool ResourceB::inRadius(ResourcePoint* rp) const {
	uint32_t x1, y1;
	std::tie(x1, y1) = this->getCenter();

	uint32_t x2_1 = rp->getX();
	uint32_t y2_1 = rp->getY();

	uint32_t x2_2 = rp->getX() + rp->getSX() - 1;
	uint32_t y2_2 = rp->getY() + rp->getSY() - 1;

	uint32_t dx_1 = std::max(x1, x2_1) - std::min(x1, x2_1);
	uint32_t dy_1 = std::max(y1, y2_1) - std::min(y1, y2_1);

	uint32_t dx_2 = std::max(x1, x2_2) - std::min(x1, x2_2);
	uint32_t dy_2 = std::max(y1, y2_2) - std::min(y1, y2_2);

	uint32_t dx = std::max(dx_1, dx_2);
	uint32_t dy = std::max(dy_1, dy_2);

	return (dx <= this->getRadius() and dy <= this->getRadius());
}
std::tuple<uint32_t, uint32_t> ResourceB::getCenter() const {
	uint32_t x1 = this->getX() + (this->getSX() - 1) / 2;
	uint32_t y1 = this->getY() + (this->getSY() - 1) / 2;
	return std::make_tuple(x1, y1);
}
GOR ResourceB::collectResources() {
	uint32_t left = this->getCollectionSpeed();
	GEvent gEvent;
	for (uint32_t i = 0; i < this->resourcePointsPtr->size() and left; i = i + 1) {
		ResourcePoint* rp = this->resourcePointsPtr->at(i);
		if (rp->getResourceType() == this->getResourceType() and this->inRadius(rp)) {
			uint32_t got = std::min(left, rp->getHP());
			left = left - got;
			gEvent.collect.emplace_back(rp, got);
		}
	}

	GOR responce;
	if (left > 0) {
		this->resourcesLeft = false;
		MessageW* window = new MessageW(this->getNewWindowSoundName(), "click", this->getResourcesOverStr());
		window->addOnStartGEvent(gEvent);
		responce.windows.push(window);
	}
	else {
		responce.events.push_back(gEvent);
	}
	return responce;
}
GOR ResourceB::highlightArea() const {
	GEvent gEvent;

	uint32_t x1, y1;
	uint32_t x2, y2;
	std::tie(x1, y1) = this->getCenter();
	std::tie(x2, y2) = std::make_tuple(x1, y1);

	x1 = x1 + this->getRadius();
	y1 = y1 + this->getRadius();
	
	if (this->getX() > this->getRadius()) {
		x2 = this->getX() - this->getRadius();
	}
	else {
		x2 = 0;
	}

	if (this->getY() > this->getRadius()) {
		y2 = this->getY() - this->getRadius();
	}
	else {
		y2 = 0;
	}

	for (uint32_t x = x2; x <= x1; x = x + 1) {
		for (uint32_t y = y2; y <= y1; y = y + 1) {
			gEvent.changeHighlight.emplace_back(this, x, y);
		}
	}

	GOR responce;
	responce.events.push_back(gEvent);
	return responce;
}
GOR ResourceB::getSelectionW(const GEvent& highlightEvent) {
	GOR response;

	std::vector<SelectionWComponent> components;
	components.emplace_back("exit_icon", L"Покинуть", true, highlightEvent);
	components.emplace_back(this->getTextureName(), this->getDescription(), false, GEvent());

	if (this->getCurrentLevel() < TOTAL_LEVELS) {
		GEvent gameEventUpgrade = highlightEvent;
		gameEventUpgrade.tryToUpgrade.emplace_back(this, this->getUpgradeCost());
		components.emplace_back("upgrade_icon", L"Улучшить за " + this->getUpgradeCost().getReadableInfo() + L"\n"
			"Улучшение повысит скорость добычи с " + std::to_wstring(this->getCollectionSpeed()) + L" до " + std::to_wstring(this->getCollectionSpeed(this->getCurrentLevel())) + 
			L" и радиус добычи с " + std::to_wstring(this->getRadius()) + L" до " + std::to_wstring(this->getRadius(this->getCurrentLevel())) + L".", true, gameEventUpgrade);
	}

	SelectionW* window = new SelectionW(this->getNewWindowSoundName(), "click", components);
	response.windows.push(window);

	return response;
}
GOR ResourceB::getGameObjectResponse(const Player& player) {
	if (!this->exist()) {
		return GOR();
	}
	if (this->belongTo(&player)) {
		if (this->upgrading()) {
			return this->handleBusyWithUpgrading();
		}
		if (!this->works()) {
			return this->handleDoesNotWork();
		}
		GOR response = this->highlightArea();
		return response + this->getSelectionW(response.events.back());
	}
	return this->getUnitOfEnemyResponse();
}