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



#include "Sawmill.hpp"


Sawmill::Sawmill() = default;
Sawmill::Sawmill(uint32_t x, uint32_t y, const Player* playerPtr, const std::vector<ResourcePoint*>* resourcePointsPtr) : 
	ResourceBuilding(x, y, 2, 2, 20000, playerPtr, resourcePointsPtr),
	Building(x, y, 2, 2, 20000, playerPtr) {}
Resources Sawmill::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 10000));
	return cost;
}
uint32_t Sawmill::getRegenerationSpeed() const {
	return 10000;
}
std::string Sawmill::getTextureName() const {
	return "sawmill";
}
std::wstring Sawmill::getIsNotBuiltYetStr() const {
	return
		L"ЛЕСОПИЛКА ЕЩЕ НЕ ПОСТРОЕНА\n"
		"Дождитесь конца строительства.\n"
		+ this->getReadableHpInfo() + L"\n"
		+ this->getReadableRegenerationSpeed();
}
std::wstring Sawmill::getBuildingFinishedStr() const {
	return
		L"ЛЕСОПИЛКА ПОСТРОЕНА\n"
		"Благодаря Вашим рабочим, лесопилка уже начинает добывать первую древесину.";
}
std::string Sawmill::getNewWindowSoundName() const {
	return "sawmill";
}
std::string Sawmill::getResourceType() const {
	return "wood";
}
uint32_t Sawmill::getResourceNPerMove(uint32_t level) const {
	float levelCollectionSpeedBonus[TOTAL_LEVELS] = {
		1,
		3,
		6
	};
	return 2500 * levelCollectionSpeedBonus[level];
}
uint32_t Sawmill::getRadius(uint32_t level) const {
	uint32_t levelRadiusBonus[TOTAL_LEVELS] = {
		0,
		2,
		3
	};
	return 5 + levelRadiusBonus[level];
}
std::wstring Sawmill::getDescription() const {
	return
		L"Лесопилки обеспечивают Ваш город древесиной, необходимой для строительства большинства зданий.\n"
		+ this->getReadableHpInfo();
}
std::wstring Sawmill::getResourcesOverStr() const {
	return
		L"ЛЕСА ВЫРУБЛЕНЫ\n"
		"Одна из Ваших лесопилок прекращает работу.";
}
std::wstring Sawmill::getUpgradeStartDescription() const {
	return
		L"НАЧАТО УЛУЧШЕНИЕ ЛЕСОПИЛКИ\n"
		"Сбор древесины был приостановлен.\n"
		"Число ходов до конца улучшения: " + std::to_wstring(this->getUpgradeMoves());
}
std::wstring Sawmill::getUpgradeFinishDescription() const {
	return
		L"УЛУЧШЕНИЕ ЛЕСОПИЛКИ ЗАВЕРШЕНО\n"
		"Сбор древесины возобновлен.";
}
std::wstring Sawmill::getBusyWithUpgradingDescription() const {
	return L"ЛЕСОПИЛКА НЕДОСТУПНА\n"
		"Подождите, пока будет завершено улучшение.\n"
		"Число ходов до конца улучшения: " + std::to_wstring(this->getUpgradeMoves());
}
Resources Sawmill::getUpgradeCost() const {
	Resources upgradeCosts[TOTAL_LEVELS - 1] = {
		Resources({{"wood", 20000}}),
		Resources({{"wood", 40000}})
	};
	return upgradeCosts[this->getCurrentLevel() - 1];
}
uint32_t Sawmill::getUpgradeMoves() const {
	uint32_t upgradeMoves[TOTAL_LEVELS - 1] = {
		2,
		3
	};
	return upgradeMoves[this->getCurrentLevel() - 1];
}