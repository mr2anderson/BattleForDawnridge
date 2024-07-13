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
#include "TryToCollectEvent.hpp"
#include "FlyingE.hpp"


ResourceB::ResourceB() = default;
ResourceB::ResourceB(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units, std::shared_ptr<GOCollection<ResourcePoint>> resourcePointsPtr) :
	HpSensitiveB(x, y, sx, sy, maxHp, playerId, units),
	AreaBLandscapeInsensible(x, y, sx, sy, maxHp, playerId, units),
	Building(x, y, sx, sy, maxHp, playerId, units) {
	this->resourcePointsPtr = resourcePointsPtr;
}
Events ResourceB::newMove(uint32_t playerId) {
	if (this->belongTo(playerId) and this->exist()) {
		Events response = this->regenerate();
		if (this->works()) {
			response = response + this->tryToCollectResources();
		}
		return response;
	}
	return Events();
}
Events ResourceB::tryToCollectResources() {
	Events responce;

	for (uint32_t i = 0; i < this->resourcePointsPtr->size(); i = i + 1) {
		ResourcePoint* rp = this->resourcePointsPtr->at(i);
		if (rp->exist() and this->AreaB::inRadius(rp) and rp->getResourceType() == this->getResourceType()) {
			responce.add(std::make_shared<TryToCollectEvent>(this->getPlayerId(), rp, this->getCollectionSpeed()));
			break;
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
	return Events();
}