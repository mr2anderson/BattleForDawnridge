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
ResourceB::ResourceB(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, const Player* playerPtr, std::vector<ResourcePoint*>* resourcePointsPtr) : 
	UpgradeableB(x, y, sx, sy, maxHp, playerPtr),
	HpSensitiveB(x, y, sx, sy, maxHp, playerPtr),
	AreaB(x, y, sx, sy, maxHp, playerPtr),
	Building(x, y, sx, sy, maxHp, playerPtr) {
	this->resourcePointsPtr = resourcePointsPtr;
	this->resourcesLeft = true;
}
Event ResourceB::newMove(const Player& currentPlayer) {
	Event response;
	if (this->belongTo(&currentPlayer) and this->exist()) {
		response = this->handleCurrentUpgrade();
		if (this->upgrading()) {
			return response;
		}
		response = response + this->regenerate();
		if (!this->repairing() and this->resourcesLeft) {
			response = response + this->collectResources();
		}
		return response;
	}
	return Event();
}
bool ResourceB::works() const {
	return this->UpgradeableB::works() and this->HpSensitiveB::works() and this->AreaB::works();
}
uint32_t ResourceB::getCollectionSpeed() const {
	return this->getCollectionSpeed(this->getCurrentLevel() - 1);
}
uint32_t ResourceB::getRadius() const {
	return this->getRadius(this->getCurrentLevel() - 1);
}
Event ResourceB::collectResources() {
	uint32_t left = this->getCollectionSpeed();
	std::vector<std::tuple<ResourcePoint*, uint32_t>> srcs;
	for (uint32_t i = 0; i < this->resourcePointsPtr->size() and left; i = i + 1) {
		ResourcePoint* rp = this->resourcePointsPtr->at(i);
		if (rp->getResourceType() == this->getResourceType() and this->inRadius(rp)) {
			uint32_t got = std::min(left, rp->getHP());
			left = left - got;
			if (got != 0) {
				srcs.emplace_back(rp, got);
			}
		}
	}

	Event responce;
	if (left == this->getCollectionSpeed()) {
		this->resourcesLeft = false;
	}
	else {
		for (const auto& src : srcs) {
			ResourcePoint* rp = std::get<ResourcePoint*>(src);
			uint32_t n = std::get<uint32_t>(src);
			FlyingE* element = new FlyingE(this->getResourceType() + "_icon", this->getSoundName(), rp->getX(), rp->getY(), rp->getSX(), rp->getSY());
			Event gEvent;
			gEvent.collect.emplace_back(rp, n);
			element->addOnStartGEvent(gEvent);
			responce.createE.push_back(element);
		}
	}

	return responce;
}
Event ResourceB::getSelectionW() {
	Event response;

	std::vector<SelectionWComponent> components;
	components.emplace_back("exit_icon", L"Покинуть", true, true, this->getHighlightEvent());
	components.emplace_back(this->getTextureName(), 
		this->getDescription() + L"\n" +
		this->getReadableHpInfo(), false, false, Event());

	if (this->getCurrentLevel() < TOTAL_LEVELS) {
		Event gameEventUpgrade = this->getHighlightEvent();
		gameEventUpgrade.tryToUpgrade.emplace_back(this, this->getUpgradeCost());
		components.emplace_back("upgrade_icon", L"Улучшить за " + this->getUpgradeCost().getReadableInfo() + L"\n"
			"Улучшение повысит скорость добычи с " + std::to_wstring(this->getCollectionSpeed()) + L" до " + std::to_wstring(this->getCollectionSpeed(this->getCurrentLevel())) + 
			L" и радиус добычи с " + std::to_wstring(this->getRadius()) + L" до " + std::to_wstring(this->getRadius(this->getCurrentLevel())) + L".", true, false, gameEventUpgrade);
	}

	SelectionW* window = new SelectionW(this->getSoundName(), "click", components);
	response.createE.push_back(window);

	return response;
}
Event ResourceB::getGameObjectResponse(const Player& player) {
	if (!this->exist()) {
		return Event();
	}
	if (this->belongTo(&player)) {
		if (this->upgrading()) {
			return this->handleBusyWithUpgrading();
		}
		if (this->repairing()) {
			return this->handleRepairing();
		}
		Event response;
		response = this->getHighlightEvent();
		return response + this->getSelectionW();
	}
	return this->getUnitOfEnemyResponse();
}