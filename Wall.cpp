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


#include "Wall.hpp"


const uint32_t Wall::LEVEL_HP[Wall::TOTAL_LEVELS] = {
	120000,
	360000,
	840000
};


Wall::Wall() = default;
Wall::Wall(uint32_t x, uint32_t y, std::shared_ptr<Player> playerPtr) :
	UpgradeableB(x, y, 2, 2, LEVEL_HP[0], playerPtr),
	Building(x, y, 2, 2, LEVEL_HP[0], playerPtr) {

}
Building* Wall::cloneBuilding() const {
	return new Wall();
}
Events Wall::newMove(std::shared_ptr<Player> player) {
	Events response;
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
Resources Wall::getCost() const {
	Resources cost;
	cost.plus(Resource("stone", 6000));
	return cost;
}
uint32_t Wall::GET_REGENERATION_SPEED(uint32_t level) {
	return LEVEL_HP[level] / 3;
}
uint32_t Wall::getRegenerationSpeed() const {
	return GET_REGENERATION_SPEED(this->getCurrentLevel() - 1);
}
std::string Wall::getTextureName() const {
	return "wall" + std::to_string(this->getCurrentLevel());
}
std::string Wall::getSoundName() const {
	return "stone";
}
std::wstring Wall::getDescription() const {
	return *Texts::get()->get("wall_description");
}
std::wstring Wall::getUpperCaseReadableName() const {
	return *Texts::get()->get("wall_upper_case_readable_name");
}
Resources Wall::getUpgradeCost() const {
	Resources upgradeCosts[TOTAL_LEVELS - 1] = {
		Resources({{"stone", 12000}}),
		Resources({{"stone", 24000}})
	};
	return upgradeCosts[this->getCurrentLevel() - 1];
}
uint32_t Wall::getUpgradeTime() const {
	uint32_t upgradeMoves[TOTAL_LEVELS - 1] = {
		3,
		6
	};
	return upgradeMoves[this->getCurrentLevel() - 1];
}
Events Wall::getSelectionW() {
	Events response;

	std::vector<GameActionWindowComponent> components;
	components.emplace_back("exit_icon", *Texts::get()->get("leave"), true, true, Events());
	components.emplace_back("wall" + std::to_string(this->getCurrentLevel()),
		this->getDescription() + L'\n'
		+ this->getReadableHpInfo(), false, false, Events());

	if (this->getCurrentLevel() != TOTAL_LEVELS) {
		Events gameEventUpgrade;
		gameEventUpgrade.add(std::make_shared<TryToUpgradeEvent>(this));
		components.emplace_back("wall" + std::to_string(this->getCurrentLevel() + 1),
			*Texts::get()->get("upgrade_for") + this->getUpgradeCost().getReadableInfo() + L'\n' +
			*Texts::get()->get("upgrade_will_increase_hp_from") + std::to_wstring(LEVEL_HP[this->getCurrentLevel() - 1]) + *Texts::get()->get("to") + std::to_wstring(LEVEL_HP[this->getCurrentLevel()]) +
			*Texts::get()->get("and_repair_speed_from") + std::to_wstring(this->getRegenerationSpeed()) + *Texts::get()->get("to") + std::to_wstring(GET_REGENERATION_SPEED(this->getCurrentLevel())) + L'.', true, false, gameEventUpgrade);
	}

	std::shared_ptr<GameActionWindow> window = std::make_shared<GameActionWindow>(this->getSoundName(), "click", components);
	response.add(std::make_shared<CreateEEvent>(window));

	return response;
}
Events Wall::getGameObjectResponse(std::shared_ptr<Player> player) {
	if (this->belongTo(player)) {
		if (this->upgrading()) {
			return this->handleBusyWithUpgrading();
		}
		return this->getSelectionW();
	}
	return this->getUnitOfEnemyResponse();
}