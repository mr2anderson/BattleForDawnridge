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
	UpgradeableHpSensitiveB(x, y, 2, 2, 10000, playerPtr),
	Building(x, y, 2, 2, 10000, playerPtr){}
GOR Farm::newMove(const Player& player) {	
	if (this->belongTo(&player) and this->exist()) {
		GOR response = this->handleCurrentUpgrade();
		if (this->upgrading()) {
			return response;
		}
		return response + this->regenerate() + this->collectFood();
	}
	return GOR();
}
Resources Farm::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 5000));
	return cost;
}
uint32_t Farm::getRegenerationSpeed() const {
	return 5000;
}
std::string Farm::getTextureName() const {
	return "farm";
}
std::string Farm::getNewWindowSoundName() const {
	return "leaves";
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
GOR Farm::collectFood() const {
	GOR response;
	response.gEvent.addResource.push_back(Resource("food", this->getCollectionSpeed()));
	return response;
}
GOR Farm::getGameObjectResponse(const Player& player) {
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
		return this->getSelectionW();
	}
	return this->getUnitOfEnemyResponse();
}
GOR Farm::getSelectionW() {
	GOR response;

	std::vector<SelectionWComponent> components;
	components.emplace_back("exit_icon", L"Покинуть", true, GEvent());
	components.emplace_back(this->getTextureName(),
		L"Фермы обеспечивают Ваш город едой, необходимой для содержания армии.\n"
		+ this->getReadableHpInfo(), false, GEvent());

	if (this->getCurrentLevel() < TOTAL_LEVELS) {
		GEvent gameEventUpgrade;
		gameEventUpgrade.tryToUpgrade.emplace_back(this, this->getUpgradeCost());
		components.emplace_back("upgrade_icon", 
			L"Улучшить за " + this->getUpgradeCost().getReadableInfo() + L"\n"
			"Улучшение повысит скорость добычи с " + std::to_wstring(this->getCollectionSpeed()) + L" до " + std::to_wstring(GET_COLLECTION_SPEED(this->getCurrentLevel())) + L".", true, gameEventUpgrade);
	}

	SelectionW* window = new SelectionW(this->getNewWindowSoundName(), "click", components);
	response.popUpWindows.push(window);

	return response;
}