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


#include "MessageWindow.hpp"
#include "Fort.hpp"


const Resources Fort::UPGRADE_COSTS[Fort::TOTAL_LEVELS - 1] = {
	Resources({{"stone", 75000}}),
	Resources({{"stone", 75000}}),
};
const uint32_t Fort::UPGRADE_MOVES[Fort::TOTAL_LEVELS - 1] = {
	2,
	2,
};
const uint32_t Fort::UPGRADE_HP[Fort::TOTAL_LEVELS - 1] = {
	200000,
	300000
};


Fort::Fort(uint32_t x, uint32_t y, const Player* playerPtr) : Building(x, y, 100000, true, playerPtr) {}
GameObjectResponse Fort::newMove(const Player& player, uint32_t windowW, uint32_t windowH) {
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
GameObjectResponse Fort::upgrade() {
	GameObjectResponse response;
	response.gameEvent = GameEvent();
	response.gameEvent.value().subResources.push_back(UPGRADE_COSTS[this->getCurrentLevel() - 1]);

	this->Building::upgrade(UPGRADE_MOVES[this->getCurrentLevel() - 1]);

	response = response + this->handleUpgradeStart();

	return response;
}
Resources Fort::getCost() const {
	Resources cost;
	cost.plus(Resource("stone", 200000));
	return cost;
}
GameObjectResponse Fort::getSelectWindow() {
	GameObjectResponse response;

	std::vector<std::tuple<std::string, std::wstring, bool, GameEvent>> data;
	data.emplace_back("exit", L"Покинуть", true, GameEvent());
	data.emplace_back("fort", L"Замок — сердце города. Защищайте его любой ценой. Разгром всех замков приведет к поражению. ", false, GameEvent());

	if (this->getCurrentLevel() != TOTAL_LEVELS) {
		GameEvent gameEventUpgrade;
		gameEventUpgrade.tryToUpgrade.emplace_back(this, UPGRADE_COSTS[this->getCurrentLevel() - 1]);
		data.emplace_back("upgrade", L"Увеличить защиту за " + UPGRADE_COSTS[this->getCurrentLevel() - 1].getReadableInfo(), true, gameEventUpgrade);
	}

	SelectWindow* window = new SelectWindow("hooray", "click", data);
	response.popUpWindows.push(window);

	return response;
}
uint32_t Fort::getRegenerationSpeed() const {
	return 10000;
}
std::string Fort::getTextureName() const {
	return "fort";
}
std::string Fort::getNewWindowSoundName() const {
	return "hooray";
}
std::wstring Fort::getUpgradeStartDescription() const {
	return L"Начато улучшение замка\nПодождите, пока оно закончится.";
}
std::wstring Fort::getUpgradeFinishDescription() const {
	return L"Улучшение замка завершено!";
}
std::wstring Fort::getBusyWithUpgradingDescription() const {
	return L"Замок не доступен\nПодождите, пока будет завершено улучшение.";
}
GameObjectResponse Fort::decreaseUpgradeMovesLeft() {
	GameObjectResponse response = this->Building::decreaseUpgradeMovesLeft();
	if (!response.popUpWindows.empty()) {
		this->changeMaxHp(UPGRADE_HP[this->getCurrentLevel() - 1 - 1]);
	}
	return response;
}
GameObjectResponse Fort::getGameObjectResponse(const Player& player, uint32_t windowW, uint32_t windowH) {
	if (this->belongTo(&player)) {
		if (this->upgrading()) {
			return this->handleUpgrading();
		}
		return this->getSelectWindow();
	}
	return this->getUnitOfEnemyResponse();
}