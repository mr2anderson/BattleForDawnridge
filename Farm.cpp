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
Farm::Farm(uint32_t x, uint32_t y, std::shared_ptr<Player> playerPtr) : 
	UpgradeableB(x, y, 3, 3, 10000, playerPtr),
	HpSensitiveB(x, y, 3, 3, 10000, playerPtr),
	Building(x, y, 3, 3, 10000, playerPtr){}
Events Farm::newMove(std::shared_ptr<Player> player) {	
	if (this->belongTo(player) and this->exist()) {
		Events response = this->handleCurrentUpgrade() + this->regenerate();
		if (this->works()) {
			response = response + this->collectFood();
		}
		return response;
	}
	return Events();
}
Building* Farm::cloneBuilding() const {
	return new Farm(*this);
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
GameActionWindowComponent Farm::getUpgradeComponent() {
	Events gameEventUpgrade;
	gameEventUpgrade.add(std::make_shared<TryToUpgradeEvent>(this));
	GameActionWindowComponent component = {
		"upgrade_icon",
		*Texts::get()->get("upgrade_for") + this->getUpgradeCost().getReadableInfo() + L'\n' +
		*Texts::get()->get("upgrade_will_increase_collection_speed_from") + std::to_wstring(this->getCollectionSpeed()) + *Texts::get()->get("to") + std::to_wstring(GET_COLLECTION_SPEED(this->getCurrentLevel())) + L'.',
		true,
		false,
		gameEventUpgrade
	};
	return component;
}
uint32_t Farm::getRegenerationSpeed() const {
	return 5000;
}
std::wstring Farm::getUpperCaseReadableName() const {
	return *Texts::get()->get("farm_upper_case_readable_name");
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
Events Farm::collectFood() const {
	Events gEvent;
	gEvent.add(std::make_shared<AddResourceEvent>(Resource("food", this->getCollectionSpeed())));
	std::shared_ptr<PopUpElement> element = std::make_shared<FlyingE>("food_icon", this->getSoundName(), this->getX(), this->getY(), this->getSX(), this->getSY());
	element->addOnStartGEvent(gEvent);
	Events responce;
	responce.add(std::make_shared<CreateEEvent>(element));
	return responce;
}
Events Farm::getGameObjectResponse(std::shared_ptr<Player> player) {
	if (!this->exist()) {
		return Events();
	}
	if (this->belongTo(player)) {
		return this->getSelectionW();
	}
	return this->getUnitOfEnemyResponse();
}
Events Farm::getSelectionW() {
	Events response;

	std::vector<GameActionWindowComponent> components;
	components.push_back(this->getExitComponent());
	components.push_back(this->getDescriptionComponent());
	components.push_back(this->getHpInfoComponent());
	if (this->repairing()) {
		components.push_back(this->getBusyWithRepairingComponent());
	}
	if (this->upgrading()) {
		components.push_back(this->getBusyWithUpgradingComponent());
	}
	if (this->works() and this->getCurrentLevel() != TOTAL_LEVELS) {
		components.push_back(this->getUpgradeComponent());
	}

	std::shared_ptr<GameActionWindow> window = std::make_shared<GameActionWindow>(this->getSoundName(), "click", components);
	response.add(std::make_shared<CreateEEvent>(window));

	return response;
}