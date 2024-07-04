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


#include "Castle.hpp"


const uint32_t Castle::LEVEL_HP[Castle::TOTAL_LEVELS] = {
	100000,
	400000,
	1000000
};


Castle::Castle() = default;
Castle::Castle(uint32_t x, uint32_t y, const Player* playerPtr) : 
	UpgradeableB(x, y, 3, 3, LEVEL_HP[0], playerPtr),
	TerritoryB(x, y, 3, 3, LEVEL_HP[0], playerPtr),
	Building(x, y, 3, 3, LEVEL_HP[0], playerPtr) {

}
GOR Castle::newMove(const Player& player) {
	GOR response;
	if (this->belongTo(&player) and this->exist()) {
		this->changeMaxHp(LEVEL_HP[this->getCurrentLevel() - 1]);
		response = this->handleCurrentUpgrade();
		if (this->upgrading()) {
			return response;
		}
		return response + this->regenerate();
	}
	return response;
}
Resources Castle::getCost() const {
	Resources cost;
	cost.plus(Resource("stone", 100000));
	return cost;
}
std::string Castle::getTextureName() const {
	return "castle";
}
std::wstring Castle::getDescription() const {
	return L"Замок — сердце города. Защищайте его любой ценой. Разгром всех замков приведет к поражению.";
}
uint32_t Castle::GET_REGENERATION_SPEED(uint32_t level) {
	return LEVEL_HP[level] / 4;
}
uint32_t Castle::getRegenerationSpeed() const {
	return GET_REGENERATION_SPEED(this->getCurrentLevel() - 1);
}
std::string Castle::getNewWindowSoundName() const {
	return "hooray";
}
std::wstring Castle::getReadableName() const {
	return L"замок";
}
Resources Castle::getUpgradeCost() const {
	Resources upgradeCosts[TOTAL_LEVELS - 1] = {
		Resources({{"stone", 200000}}),
		Resources({{"stone", 400000}})
	};
	return upgradeCosts[this->getCurrentLevel() - 1];
}
uint32_t Castle::getUpgradeTime() const {
	uint32_t upgradeMoves[TOTAL_LEVELS - 1] = {
		4,
		8
	};
	return upgradeMoves[this->getCurrentLevel() - 1];
}
uint32_t Castle::getRadius() const {
	return 3;
}
GOR Castle::getSelectionW() {
	GOR response;

	std::vector<SelectionWComponent> components;
	components.emplace_back("exit_icon", L"Покинуть", true, true, this->getHighlightEvent());
	components.emplace_back("castle",
		this->getDescription() + L"\n"
		+ this->getReadableHpInfo(), false, false, GEvent());

	if (this->getCurrentLevel() != TOTAL_LEVELS) {
		GEvent gameEventUpgrade;
		gameEventUpgrade.tryToUpgrade.emplace_back(this, this->getUpgradeCost());
		components.emplace_back("upgrade_icon",
			L"Улучшить замок за " + this->getUpgradeCost().getReadableInfo() + L"\n"
			"Улучшение увеличит защиту с " + std::to_wstring(LEVEL_HP[this->getCurrentLevel() - 1]) + L" до " + std::to_wstring(LEVEL_HP[this->getCurrentLevel()]) +
			L" и скорость ремонта с " + std::to_wstring(this->getRegenerationSpeed()) + L" до " + std::to_wstring(GET_REGENERATION_SPEED(this->getCurrentLevel())) + L".", true, false, gameEventUpgrade + this->getHighlightEvent());
	}

	SelectionW* window = new SelectionW(this->getNewWindowSoundName(), "click", components);
	response.elements.push(window);

	return response;
}
GOR Castle::getGameObjectResponse(const Player& player) {
	if (this->belongTo(&player)) {
		if (this->upgrading()) {
			return this->handleBusyWithUpgrading();
		}
		GOR responce;
		responce.events.push_back(this->getHighlightEvent());
		return responce + this->getSelectionW();
	}
	return this->getUnitOfEnemyResponse();
}