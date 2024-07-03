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


Castle::Castle(uint32_t x, uint32_t y, const Player* playerPtr) : Building(x, y, 2, 2, LEVEL_HP[0], true, playerPtr) {}
GameObjectResponse Castle::newMove(const Player& player) {
	GameObjectResponse response;
	if (this->belongTo(&player) and this->exist()) {
		response = this->decreaseUpgradeMovesLeft();
		if (this->upgrading()) {
			return response;
		}
		return response + this->processRegeneration();
	}
	return response;
}
Resources Castle::getCost() const {
	Resources cost;
	cost.plus(Resource("stone", 100000));
	return cost;
}
GameObjectResponse Castle::getSelectWindow() {
	GameObjectResponse response;

	std::vector<std::tuple<std::string, std::wstring, bool, GameEvent>> data;
	data.emplace_back("exit_icon", L"Покинуть", true, GameEvent());
	data.emplace_back("castle", L"Замок — сердце города. Защищайте его любой ценой. Разгром всех замков приведет к поражению. Защита: " + std::to_wstring(this->getHP()) + L" / " + std::to_wstring(this->getMaxHP()) + L".", false, GameEvent());

	if (this->getCurrentLevel() != TOTAL_LEVELS) {
		GameEvent gameEventUpgrade;
		gameEventUpgrade.tryToUpgrade.emplace_back(this, this->getUpgradeCost());
		data.emplace_back("upgrade_icon", L"Улучшить замок за " + this->getUpgradeCost().getReadableInfo() + L". Улучшение увеличит защиту и скорость ремонта.", true, gameEventUpgrade);
	}

	SelectWindow* window = new SelectWindow("hooray", "click", data);
	response.popUpWindows.push(window);

	return response;
}
uint32_t Castle::getRegenerationSpeed() const {
	return LEVEL_HP[this->getCurrentLevel()] / 4;
}
std::string Castle::getTextureName() const {
	return "castle";
}
std::string Castle::getNewWindowSoundName() const {
	return "hooray";
}
std::wstring Castle::getUpgradeStartDescription() const {
	return L"НАЧАТО УЛУЧШЕНИЕ ЗАМКА\nПодождите, пока оно закончится.";
}
std::wstring Castle::getUpgradeFinishDescription() const {
	return L"УЛУЧШЕНИЕ ЗАМКА ЗАВЕРШЕНО";
}
std::wstring Castle::getBusyWithUpgradingDescription() const {
	return L"ЗАМОК НЕДОСТУПЕН\nПодождите, пока будет завершено улучшение.";
}
Resources Castle::getUpgradeCost() const {
	Resources upgradeCosts[TOTAL_LEVELS - 1] = {
		Resources({{"stone", 200000}}),
		Resources({{"stone", 400000}})
	};
	return upgradeCosts[this->getCurrentLevel() - 1];
}
uint32_t Castle::getUpgradeMoves() const {
	uint32_t upgradeMoves[TOTAL_LEVELS - 1] = {
		4,
		8
	};
	return upgradeMoves[this->getCurrentLevel() - 1];
}
GameObjectResponse Castle::decreaseUpgradeMovesLeft() {
	GameObjectResponse response = this->Building::decreaseUpgradeMovesLeft();
	if (!response.popUpWindows.empty()) {
		this->changeMaxHp(LEVEL_HP[this->getCurrentLevel() - 1]);
	}
	return response;
}
GameObjectResponse Castle::getGameObjectResponse(const Player& player) {
	if (this->belongTo(&player)) {
		if (this->upgrading()) {
			return this->handleUpgrading();
		}
		return this->getSelectWindow();
	}
	return this->getUnitOfEnemyResponse();
}