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
	Resources({{"wood", 5000}}),
	Resources({{"wood", 5000}}),
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
const uint32_t Mine::LEVEL_RADIUS_BONUS[Mine::TOTAL_LEVELS]{
	0,
	1,
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
	return L"ШАХТА ЕЩЕ НЕ ПОСТРОЕНА\nДождитесь конца строительства.";
}
std::wstring Mine::getBuildingFinishedStr() const {
	return L"ШАХТА ПОСТРОЕНА\nБлагодаря Вашим рабочим, шахта уже начинает добывать первое железо.";
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
	return 5 + LEVEL_RADIUS_BONUS[this->getCurrentLevel() - 1];
}
std::wstring Mine::getDescription() const {
	return L"Шахты обеспечивают Ваш город железом — основой сильной армии. Защита: " + std::to_wstring(this->getHP()) + L" / " + std::to_wstring(this->getMaxHP()) + L".";
}
std::wstring Mine::getResourcesOverStr() const {
	return L"ЗАЛЕЖИ ИСТОЩЕНЫ\nОдна из Ваших шахт прекращает работу.";
}
std::wstring Mine::getUpgradeStartDescription() const {
	return L"НАЧАТО УЛУЧШЕНИЕ ШАХТЫ\nСбор железа был приостановлен.";
}
std::wstring Mine::getUpgradeFinishDescription() const {
	return L"УЛУЧШЕНИЕ ШАХТЫ ЗАВЕРШЕНО\nСбор железа возобновлен.";
}
std::wstring Mine::getBusyWithUpgradingDescription() const {
	return L"ШАХТА НЕДОСТУПНА\nПодождите, пока будет завершено улучшение.";
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