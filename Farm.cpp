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
	UpgradeableHpSensitiveBuilding(x, y, 2, 2, 10000, playerPtr),
	Building(x, y, 2, 2, 10000, playerPtr){}
GameObjectResponse Farm::newMove(const Player& player) {	
	if (this->belongTo(&player) and this->exist()) {
		GameObjectResponse response = this->decreaseUpgradeMovesLeft();
		if (this->upgrading()) {
			return response;
		}
		return response + this->processRegeneration() + this->collectFood();
	}
	return GameObjectResponse();
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
std::wstring Farm::getIsNotBuiltYetStr() const {
	return
		L"ФЕРМА ЕЩЕ НЕ ПОСТРОЕНА\n"
		"Дождитесь конца строительства.\n"
		+ this->getReadableHpInfo() + L"\n"
		+ this->getReadableRegenerationSpeed();
}
std::wstring Farm::getBuildingFinishedStr() const {
	return 
		L"ФЕРМА ПОСТРОЕНА\n"
		"Благодаря Вашим рабочим, ферма уже начинает добывать первую еду.";
}
std::string Farm::getNewWindowSoundName() const {
	return "leaves";
}
GameObjectResponse Farm::getGameObjectResponse(const Player& player) {
	if (!this->exist()) {
		return GameObjectResponse();
	}
	if (this->belongTo(&player)) {
		if (this->upgrading()) {
			return this->handleUpgrading();
		}
		if (!this->works()) {
			return this->handleDoesNotWork();
		}
		return this->getSelectWindow();
	}
	return this->getUnitOfEnemyResponse();
}
GameObjectResponse Farm::getSelectWindow() {
	GameObjectResponse response;

	std::vector<std::tuple<std::string, std::wstring, bool, GameEvent>> data;
	data.emplace_back("exit_icon", L"Покинуть", true, GameEvent());
	data.emplace_back(this->getTextureName(), 
		L"Фермы обеспечивают Ваш город едой, необходимой для содержания армии.\n"
		+ this->getReadableHpInfo(), false, GameEvent());

	if (this->getCurrentLevel() < TOTAL_LEVELS) {
		GameEvent gameEventUpgrade;
		gameEventUpgrade.tryToUpgrade.emplace_back(this, this->getUpgradeCost());
		data.emplace_back("upgrade_icon", L"Улучшить за " + this->getUpgradeCost().getReadableInfo() + L"\n"
			"Улучшение повысит скорость добычи с " + std::to_wstring(this->getCollectionSpeed()) + L" до " + std::to_wstring(GET_COLLECTION_SPEED(this->getCurrentLevel())), true, gameEventUpgrade);
	}

	SelectWindow* window = new SelectWindow(this->getNewWindowSoundName(), "click", data);
	response.popUpWindows.push(window);

	return response;
}
std::wstring Farm::getUpgradeStartDescription() const {
	return
		L"НАЧАТО УЛУЧШЕНИЕ ФЕРМЫ\n"
		"Сбор еды был приостановлен.\n"
		"Число ходов до конца улучшения: " + std::to_wstring(this->getUpgradeMoves());
}
std::wstring Farm::getUpgradeFinishDescription() const {
	return 
		L"УЛУЧШЕНИЕ ФЕРМЫ ЗАВЕРШЕНО\n"
		"Сбор еды возобновлен.";
}
std::wstring Farm::getBusyWithUpgradingDescription() const {
	return L"ФЕРМА НЕДОСТУПНА\n"
		"Подождите, пока будет завершено улучшение.\n"
		"Число ходов до конца улучшения: " + std::to_wstring(this->getUpgradeMoves());
}
Resources Farm::getUpgradeCost() const {
	Resources upgradeCosts[TOTAL_LEVELS - 1] = {
		Resources({{"wood", 10000}}),
		Resources({{"wood", 20000}})
	};
	return upgradeCosts[this->getCurrentLevel() - 1];
}
uint32_t Farm::getUpgradeMoves() const {
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
GameObjectResponse Farm::collectFood() const {
	GameObjectResponse response;
	response.gameEvent = GameEvent();
	response.gameEvent.value().addResource.push_back(Resource("food", this->getCollectionSpeed()));
	return response;
}