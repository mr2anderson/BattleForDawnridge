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
	500000,
	1300000
};


Castle::Castle() = default;
Castle::Castle(uint32_t x, uint32_t y, std::shared_ptr<Player> playerPtr) : 
	TerritoryOriginB(x, y, 3, 3, LEVEL_HP[0], playerPtr),
	UpgradeableB(x, y, 3, 3, LEVEL_HP[0], playerPtr),
	Building(x, y, 3, 3, LEVEL_HP[0], playerPtr) {
}
Building* Castle::cloneBuilding() const {
	return new Castle(*this);
}
Events Castle::newMove(std::shared_ptr<Player> player) {
	Events response;
	if (this->belongTo(player) and this->exist()) {
		this->changeMaxHp(LEVEL_HP[this->getCurrentLevel() - 1]);
		return this->handleCurrentUpgrade() + this->regenerate();
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
GameActionWindowComponent Castle::getUpgradeComponent() {
	Events gameEventUpgrade;
	gameEventUpgrade.add(std::make_shared<TryToUpgradeEvent>(this));
	GameActionWindowComponent component = {
		"upgrade_icon",
		*Texts::get()->get("upgrade_castle_for") + this->getUpgradeCost().getReadableInfo() + L'\n' +
		*Texts::get()->get("upgrade_will_increase_hp_from") + std::to_wstring(LEVEL_HP[this->getCurrentLevel() - 1]) + *Texts::get()->get("to") + std::to_wstring(LEVEL_HP[this->getCurrentLevel()]) +
		*Texts::get()->get("and_repair_speed_from") + std::to_wstring(this->getRegenerationSpeed()) + *Texts::get()->get("to") + std::to_wstring(GET_REGENERATION_SPEED(this->getCurrentLevel())) + L'.',
		true,
		false,
		gameEventUpgrade + this->getHighlightEvent()
	};
	return component;
}
uint32_t Castle::getRegenerationSpeed() const {
	return GET_REGENERATION_SPEED(this->getCurrentLevel() - 1);
}
std::wstring Castle::getUpperCaseReadableName() const {
	return *Texts::get()->get("castle_upper_case_readable_name");
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
Events Castle::getSelectionW() {
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
Events Castle::getGameObjectResponse(std::shared_ptr<Player> player) {
	if (!this->exist()) {
		return Events();
	}
	if (this->belongTo(player)) {
		return this->getHighlightEvent() + this->getSelectionW();
	}
	return this->getUnitOfEnemyResponse();
}