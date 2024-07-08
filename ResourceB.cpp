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


#include "ResourceB.hpp"


ResourceB::ResourceB() = default;
ResourceB::ResourceB(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, Player* playerPtr, std::vector<ResourcePoint*>* resourcePointsPtr) : 
	UpgradeableB(x, y, sx, sy, maxHp, playerPtr),
	HpSensitiveB(x, y, sx, sy, maxHp, playerPtr),
	AreaB(x, y, sx, sy, maxHp, playerPtr),
	Building(x, y, sx, sy, maxHp, playerPtr) {
	this->resourcePointsPtr = resourcePointsPtr;
	this->resourcesLeft = true;
}
Events ResourceB::newMove(Player* currentPlayer) {
	Events response;
	if (this->belongTo(currentPlayer) and this->exist()) {
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
	return Events();
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
Events ResourceB::collectResources() {
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

	Events responce;
	if (left == this->getCollectionSpeed()) {
		this->resourcesLeft = false;
	}
	else {
		for (const auto& src : srcs) {
			ResourcePoint* rp = std::get<ResourcePoint*>(src);
			uint32_t n = std::get<uint32_t>(src);
			std::shared_ptr<FlyingE> element = std::make_shared<FlyingE>(this->getResourceType() + "_icon", this->getSoundName(), rp->getX(), rp->getY(), rp->getSX(), rp->getSY());
			Events gEvent;
			gEvent.add(std::make_shared<CollectEvent>(rp, n));
			element->addOnStartGEvent(gEvent);
			responce.add(std::make_shared<CreateEEvent>(element));
		}
	}

	return responce;
}
Events ResourceB::getSelectionW() {
	Events response;

	std::vector<GameActionWindowComponent> components;
	components.emplace_back("exit_icon", *Texts::get()->get("leave"), true, true, this->getHighlightEvent());
	components.emplace_back(this->getTextureName(), 
		this->getDescription() + L'\n' +
		this->getReadableHpInfo(), false, false, Events());

	if (this->getCurrentLevel() < TOTAL_LEVELS) {
		Events gameEventUpgrade = this->getHighlightEvent();
		gameEventUpgrade.add(std::make_shared<TryToUpgradeEvent>(this));
		components.emplace_back("upgrade_icon", *Texts::get()->get("upgrade_for") + this->getUpgradeCost().getReadableInfo() + L'\n' +
			*Texts::get()->get("upgrade_will_increase_collection_speed_from") + std::to_wstring(this->getCollectionSpeed()) + *Texts::get()->get("to") + std::to_wstring(this->getCollectionSpeed(this->getCurrentLevel())) +
			*Texts::get()->get("and_collection_radius_from") + std::to_wstring(this->getRadius()) + *Texts::get()->get("to") + std::to_wstring(this->getRadius(this->getCurrentLevel())) + L'.', true, false, gameEventUpgrade);
	}

	std::shared_ptr<GameActionWindow> window = std::make_shared<GameActionWindow>(this->getSoundName(), "click", components);
	response.add(std::make_shared<CreateEEvent>(window));

	return response;
}
Events ResourceB::getGameObjectResponse(Player* player) {
	if (!this->exist()) {
		return Events();
	}
	if (this->belongTo(player)) {
		if (this->upgrading()) {
			return this->handleBusyWithUpgrading();
		}
		if (this->repairing()) {
			return this->handleRepairing();
		}
		Events response;
		response = this->getHighlightEvent();
		return response + this->getSelectionW();
	}
	return this->getUnitOfEnemyResponse();
}