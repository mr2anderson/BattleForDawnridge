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


#include "Castle.hpp"


const uint32_t Castle::LEVEL_HP[Castle::TOTAL_LEVELS] = {
	100000,
	400000,
	1000000
};


Castle::Castle() = default;
Castle::Castle(uint32_t x, uint32_t y, Player* playerPtr) : 
	TerritoryOriginB(x, y, 3, 3, LEVEL_HP[0], playerPtr),
	UpgradeableB(x, y, 3, 3, LEVEL_HP[0], playerPtr),
	Building(x, y, 3, 3, LEVEL_HP[0], playerPtr) {
}
Event Castle::newMove(Player* player) {
	Event response;
	if (this->belongTo(player) and this->exist()) {
		this->changeMaxHp(LEVEL_HP[this->getCurrentLevel() - 1]);
		response = this->handleCurrentUpgrade();
		if (this->upgrading()) {
			return response;
		}
		return response + this->regenerate();
	}
	return response;
}
bool Castle::works() const {
	return this->TerritoryOriginB::works() and this->UpgradeableB::works();
}
Resources Castle::getCost() const {
	Resources cost;
	cost.plus(Resource("stone", 100000));
	return cost;
}
std::string Castle::getTextureName() const {
	return "castle";
}
std::string Castle::getSoundName() const {
	return "hooray";
}
std::wstring Castle::getDescription() const {
	return *Texts::get()->get("castle_description");
}
uint32_t Castle::GET_REGENERATION_SPEED(uint32_t level) {
	return LEVEL_HP[level] / 4;
}
uint32_t Castle::getRegenerationSpeed() const {
	return GET_REGENERATION_SPEED(this->getCurrentLevel() - 1);
}
std::wstring Castle::getReadableName() const {
	return *Texts::get()->get("castle_readable_name");
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
Event Castle::getSelectionW() {
	Event response;

	std::vector<GameActionWindowComponent> components;
	components.emplace_back("exit_icon", *Texts::get()->get("leave"), true, true, this->getHighlightEvent());
	components.emplace_back("castle",
		this->getDescription() + L'\n' +
		this->getReadableHpInfo(), false, false, Event());

	if (this->getCurrentLevel() != TOTAL_LEVELS) {
		Event gameEventUpgrade;
		gameEventUpgrade.addTryToUpgradeEvent(std::make_tuple(this, this->getUpgradeCost()));
		components.emplace_back("upgrade_icon",
			*Texts::get()->get("upgrade_castle_for") + this->getUpgradeCost().getReadableInfo() + L'\n' + 
			*Texts::get()->get("upgrade_will_increase_hp_from") + std::to_wstring(LEVEL_HP[this->getCurrentLevel() - 1]) + *Texts::get()->get("to") + std::to_wstring(LEVEL_HP[this->getCurrentLevel()]) +
			*Texts::get()->get("and_repair_speed_from") + std::to_wstring(this->getRegenerationSpeed()) + *Texts::get()->get("to") + std::to_wstring(GET_REGENERATION_SPEED(this->getCurrentLevel())) + L'.', true, false, gameEventUpgrade + this->getHighlightEvent());
	}

	std::shared_ptr<GameActionWindow> window = std::make_shared<GameActionWindow>(this->getSoundName(), "click", components);
	response.addCreateEEvent(window);

	return response;
}
Event Castle::getGameObjectResponse(Player* player) {
	if (!this->exist()) {
		return Event();
	}
	if (this->belongTo(player)) {
		if (this->upgrading()) {
			return this->handleBusyWithUpgrading();
		}
		Event responce;
		responce = this->getHighlightEvent();
		return responce + this->getSelectionW();
	}
	return this->getUnitOfEnemyResponse();
}