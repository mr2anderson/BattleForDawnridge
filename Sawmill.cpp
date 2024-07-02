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


const Resources Sawmill::UPGRADE_COSTS[Sawmill::TOTAL_LEVELS - 1] = {
	Resources({{"wood", 4000}}),
	Resources({{"wood", 4000}}),
};
const uint32_t Sawmill::UPGRADE_MOVES[Sawmill::TOTAL_LEVELS - 1] = {
	2,
	2,
};
const float Sawmill::LEVEL_BONUS[Sawmill::TOTAL_LEVELS] = {
	1,
	1.5,
	2
};


Sawmill::Sawmill(uint32_t x, uint32_t y, const Player* playerPtr, const std::vector<ResourcePoint*>* resourcePointsPtr) : ResourceBuilding(x, y, 20000, playerPtr, resourcePointsPtr) {}
Resources Sawmill::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 10000));
	return cost;
}
uint32_t Sawmill::getRegenerationSpeed() const {
	return 4000;
}
std::string Sawmill::getTextureName() const {
	return "sawmill";
}
std::wstring Sawmill::getIsNotBuiltYetStr() const {
	return L"Лесопилка еще не построена\nДождитесь конца строительства.";
}
std::wstring Sawmill::getBuildingFinishedStr() const {
	return L"Лесопилка построена!\nБлагодаря Вашим рабочим, лесопилка уже начинает добывать первую древесину.";
}
std::string Sawmill::getNewWindowSoundName() const {
	return "sawmill";
}
std::string Sawmill::getResourceType() const {
	return "wood";
}
uint32_t Sawmill::getResourceNPerMove() const {
	return 2500 * LEVEL_BONUS[this->getCurrentLevel() - 1];
}
uint32_t Sawmill::getRadius() const {
	return 5;
}
std::wstring Sawmill::getDescription() const {
	return L"Лесопилки обеспечивают Ваш город древесиной — основным материалом строительства гражданских сооружений.";
}
std::wstring Sawmill::getResourcesOverStr() const {
	return L"Древесина закончилась!\nОдна из Ваших лесопилок прекращает работу.";
}
std::wstring Sawmill::getUpgradeStartDescription() const {
	return L"Начато улучшение лесопилки\nСбор древесины был приостановлен.";
}
std::wstring Sawmill::getUpgradeFinishDescription() const {
	return L"Улучшение лесопилки завершено!\nСбор древесины возобновлен.";
}
std::wstring Sawmill::getBusyWithUpgradingDescription() const {
	return L"Лесопилка не доступна\nПодождите, пока будет завершено улучшение.";
}
uint32_t Sawmill::getMaxLevel() const {
	return TOTAL_LEVELS;
}
Resources Sawmill::getUpgradeCost(uint32_t i) const {
	return UPGRADE_COSTS[i];
}
uint32_t Sawmill::getUpgradeMoves(uint32_t i) const {
	return UPGRADE_MOVES[i];
}