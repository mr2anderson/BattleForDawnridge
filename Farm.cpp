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



#include "Farm.hpp"


Farm::Farm() = default;
Farm::Farm(uint32_t x, uint32_t y, const Player* playerPtr) : 
	UpgradeableB(x, y, 3, 3, 10000, playerPtr),
	HpSensitiveB(x, y, 3, 3, 10000, playerPtr),
	Building(x, y, 3, 3, 10000, playerPtr){}
Events Farm::newMove(const Player& player) {	
	if (this->belongTo(&player) and this->exist()) {
		Events response = this->handleCurrentUpgrade();
		if (this->upgrading()) {
			return response;
		}
		response = response + this->regenerate();
		if (!this->repairing()) {
			response = response + this->collectFood();
		}
		return response;
	}
	return Events();
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
	return L"Фермы обеспечивают Ваш город едой, необходимой для содержания армии.";
}
uint32_t Farm::getRegenerationSpeed() const {
	return 5000;
}
std::wstring Farm::getReadableName() const {
	return L"ферма";
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
	GEvent gEvent;
	gEvent.addResource.push_back(Resource("food", this->getCollectionSpeed()));
	PopUpElement* element = new FlyingE("food_icon", this->getSoundName(), this->getX(), this->getY(), this->getSX(), this->getSY());
	element->addOnStartGEvent(gEvent);
	Events responce;
	responce.uiEvent.createE.push_back(element);
	return responce;
}
Events Farm::getGameObjectResponse(const Player& player) {
	if (!this->exist()) {
		return Events();
	}
	if (this->belongTo(&player)) {
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
Events Farm::getSelectionW() {
	Events response;

	std::vector<SelectionWComponent> components;
	components.emplace_back("exit_icon", L"Покинуть", true, true, GEvent());
	components.emplace_back(this->getTextureName(),
		this->getDescription() + L"\n"
		+ this->getReadableHpInfo(), false, false, GEvent());

	if (this->getCurrentLevel() < TOTAL_LEVELS) {
		GEvent gameEventUpgrade;
		gameEventUpgrade.tryToUpgrade.emplace_back(this, this->getUpgradeCost());
		components.emplace_back("upgrade_icon", 
			L"Улучшить за " + this->getUpgradeCost().getReadableInfo() + L"\n"
			"Улучшение повысит скорость добычи с " + std::to_wstring(this->getCollectionSpeed()) + L" до " + std::to_wstring(GET_COLLECTION_SPEED(this->getCurrentLevel())) + L".", true, false, gameEventUpgrade);
	}

	SelectionW* window = new SelectionW(this->getSoundName(), "click", components);
	response.uiEvent.createE.push_back(window);

	return response;
}