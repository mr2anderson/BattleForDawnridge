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



#include "Mine.hpp"


const Resources Mine::UPGRADE_COSTS[Mine::TOTAL_LEVELS - 1] = {
	Resources({{"wood", 4000}}),
	Resources({{"wood", 4000}}),
};
const uint32_t Mine::UPGRADE_MOVES[Mine::TOTAL_LEVELS - 1] = {
	2,
	2,
};
const float Mine::LEVEL_BONUS[Mine::TOTAL_LEVELS] = {
	1,
	1.5,
	2
};


Mine::Mine(uint32_t x, uint32_t y, const Player* playerPtr, const std::vector<ResourcePoint*>* resourcePointsPtr) : ResourceBuilding(x, y, 20000, playerPtr, resourcePointsPtr) {}
Resources Mine::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 10000));
	return cost;
}
uint32_t Mine::getRegenerationSpeed() const {
	return 4000;
}
std::string Mine::getTextureName() const {
	return "mine";
}
std::wstring Mine::getIsNotBuiltYetStr() const {
	return L"Шахта еще не построена\nДождитесь конца строительства.";
}
std::wstring Mine::getBuildingFinishedStr() const {
	return L"Шахта построена!\nБлагодаря Вашим рабочим, шахта уже начинает добывать первое железо.";
}
std::string Mine::getNewWindowSoundName() const {
	return "mine";
}
std::string Mine::getResourceType() const {
	return "iron";
}
uint32_t Mine::getResourceNPerMove() const {
	return 2500 * LEVEL_BONUS[this->getCurrentLevel() - 1];
}
uint32_t Mine::getRadius() const {
	return 5;
}
std::wstring Mine::getDescription() const {
	return L"Шахты обеспечивают Ваш город железом — основой сильной армии.";
}
std::wstring Mine::getResourcesOverStr() const {
	return L"Залежи истощены!\nОдна из Ваших шахт прекращает работу.";
}
std::wstring Mine::getUpgradeStartDescription() const {
	return L"Начато улучшение шахты\nСбор железа был приостановлен.";
}
std::wstring Mine::getUpgradeFinishDescription() const {
	return L"Улучшение шахты завершено!\nСбор железа возобновлен.";
}
std::wstring Mine::getBusyWithUpgradingDescription() const {
	return L"Шахта не доступна\nПодождите, пока будет завершено улучшение.";
}
uint32_t Mine::getMaxLevel() const {
	return TOTAL_LEVELS;
}
Resources Mine::getUpgradeCost(uint32_t i) const {
	return UPGRADE_COSTS[i];
}
uint32_t Mine::getUpgradeMoves(uint32_t i) const {
	return UPGRADE_MOVES[i];
}