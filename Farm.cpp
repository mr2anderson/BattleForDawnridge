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



#include "Farm.hpp"


Farm::Farm() = default;
Farm::Farm(uint32_t x, uint32_t y, Player* playerPtr) : 
	UpgradeableB(x, y, 3, 3, 10000, playerPtr),
	HpSensitiveB(x, y, 3, 3, 10000, playerPtr),
	Building(x, y, 3, 3, 10000, playerPtr){}
Event Farm::newMove(Player* player) {	
	if (this->belongTo(player) and this->exist()) {
		Event response = this->handleCurrentUpgrade();
		if (this->upgrading()) {
			return response;
		}
		response = response + this->regenerate();
		if (!this->repairing()) {
			response = response + this->collectFood();
		}
		return response;
	}
	return Event();
}
bool Farm::works() const {
	return this->HpSensitiveB::works() and this->UpgradeableB::works();
}
Resources Farm::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 5000));
	return cost;
}
std::string Farm::getTextureName() const {
	return "farm";
}
std::string Farm::getSoundName() const {
	return "food";
}
std::wstring Farm::getDescription() const {
	return *Texts::get()->get("farm_description");
}
uint32_t Farm::getRegenerationSpeed() const {
	return 5000;
}
std::wstring Farm::getReadableName() const {
	return *Texts::get()->get("farm_readable_name");
}
Resources Farm::getUpgradeCost() const {
	Resources upgradeCosts[TOTAL_LEVELS - 1] = {
		Resources({{"wood", 10000}}),
		Resources({{"wood", 20000}})
	};
	return upgradeCosts[this->getCurrentLevel() - 1];
}
uint32_t Farm::getUpgradeTime() const {
	uint32_t upgradeMoves[TOTAL_LEVELS - 1] = {
		2,
		3
	};
	return upgradeMoves[this->getCurrentLevel() - 1];
}
uint32_t Farm::GET_COLLECTION_SPEED(uint32_t level) {
	uint32_t collectionSpeed[TOTAL_LEVELS] = {
		1250,
		2625,
		5125
	};
	return collectionSpeed[level];
}
uint32_t Farm::getCollectionSpeed() const {
	return GET_COLLECTION_SPEED(this->getCurrentLevel() - 1);
}
Event Farm::collectFood() const {
	Event gEvent;
	gEvent.addResource.push_back(Resource("food", this->getCollectionSpeed()));
	PopUpElement* element = new FlyingE("food_icon", this->getSoundName(), this->getX(), this->getY(), this->getSX(), this->getSY());
	element->addOnStartGEvent(gEvent);
	Event responce;
	responce.createE.push_back(element);
	return responce;
}
Event Farm::getGameObjectResponse(Player* player) {
	if (!this->exist()) {
		return Event();
	}
	if (this->belongTo(player)) {
		if (this->upgrading()) {
			return this->handleBusyWithUpgrading();
		}
		if (this->repairing()) {
			return this->handleRepairing();
		}
		return this->getSelectionW();
	}
	return this->getUnitOfEnemyResponse();
}
Event Farm::getSelectionW() {
	Event response;

	std::vector<SelectionWComponent> components;
	components.emplace_back("exit_icon", *Texts::get()->get("leave"), true, true, Event());
	components.emplace_back(this->getTextureName(),
		this->getDescription() + L'\n'
		+ this->getReadableHpInfo(), false, false, Event());

	if (this->getCurrentLevel() < TOTAL_LEVELS) {
		Event gameEventUpgrade;
		gameEventUpgrade.tryToUpgrade.emplace_back(this, this->getUpgradeCost());
		components.emplace_back("upgrade_icon", 
			*Texts::get()->get("upgrade_for") + this->getUpgradeCost().getReadableInfo() + L'\n' +
			*Texts::get()->get("upgrade_will_increase_collection_speed_from") + std::to_wstring(this->getCollectionSpeed()) + *Texts::get()->get("to") + std::to_wstring(GET_COLLECTION_SPEED(this->getCurrentLevel())) + L'.', true, false, gameEventUpgrade);
	}

	SelectionW* window = new SelectionW(this->getSoundName(), "click", components);
	response.createE.push_back(window);

	return response;
}