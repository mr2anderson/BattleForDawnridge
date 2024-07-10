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
#include "CollectEvent.hpp"
#include "FlyingE.hpp"


ResourceB::ResourceB() = default;
ResourceB::ResourceB(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, uint32_t playerId, std::shared_ptr<GOCollection<ResourcePoint>> resourcePointsPtr) : 
	HpSensitiveB(x, y, sx, sy, maxHp, playerId),
	AreaB(x, y, sx, sy, maxHp, playerId),
	Building(x, y, sx, sy, maxHp, playerId) {
	this->resourcePointsPtr = resourcePointsPtr;
	this->resourcesLeft = true;
}
Events ResourceB::newMove(uint32_t playerId) {
	if (this->belongTo(playerId) and this->exist()) {
		Events response = this->regenerate();
		if (this->works() and this->resourcesLeft) {
			response = response + this->collectResources();
		}
		return response;
	}
	return Events();
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
			responce.add(std::make_shared<CreateEEvent>(element));
			responce.add(std::make_shared<CollectEvent>(rp, n));
		}
	}

	return responce;
}
Events ResourceB::getSelectionW() {
	Events response;

	std::vector<HorizontalSelectionWindowComponent> components;
	components.push_back(this->getExitComponent());
	components.push_back(this->getDescriptionComponent());
	components.push_back(this->getHpInfoComponent());
	components.push_back(this->getDestroyComponent());
	if (this->repairing()) {
		components.push_back(this->getBusyWithRepairingComponent());
	}

	std::shared_ptr<HorizontalSelectionWindow> window = std::make_shared<HorizontalSelectionWindow>(this->getSoundName(), "click", components);
	response.add(std::make_shared<CreateEEvent>(window));

	return response;
}
Events ResourceB::getGameObjectResponse(uint32_t playerId) {
	if (!this->exist()) {
		return Events();
	}
	if (this->belongTo(playerId)) {
		return this->getSelectionW() + this->getHighlightEvent();
	}
	return this->getUnitOfEnemyResponse();
}