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



#include "Windmill.hpp"


const Resources Windmill::UPGRADE_COSTS[Windmill::TOTAL_LEVELS - 1] = {
	Resources({{"wood", 4000}}),
	Resources({{"wood", 4000}}),
};
const uint32_t Windmill::UPGRADE_MOVES[Windmill::TOTAL_LEVELS - 1] = {
	2,
	2,
};
const float Windmill::LEVEL_BONUS[Windmill::TOTAL_LEVELS] = {
	1,
	1.5,
	2
};


Windmill::Windmill(uint32_t x, uint32_t y, const Player* playerPtr, const std::vector<ResourcePoint*>* resourcePointsPtr) : ResourceBuilding(x, y, 20000, playerPtr, resourcePointsPtr) {}
Resources Windmill::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 10000));
	return cost;
}
uint32_t Windmill::getRegenerationSpeed() const {
	return 4000;
}
std::string Windmill::getTextureName() const {
	return "windmill";
}
std::wstring Windmill::getIsNotBuiltYetStr() const {
	return L"Мельница еще не построена\nДождитесь конца строительства.";
}
std::wstring Windmill::getBuildingFinishedStr() const {
	return L"Мельница построена!\nБлагодаря Вашим рабочим, мельница уже начинает добывать первую еду.";
}
std::string Windmill::getNewWindowSoundName() const {
	return "leaves";
}
std::string Windmill::getResourceType() const {
	return "food";
}
uint32_t Windmill::getResourceNPerMove() const {
	return 2500 * LEVEL_BONUS[this->getCurrentLevel() - 1];
}
uint32_t Windmill::getRadius() const {
	return 5;
}
std::wstring Windmill::getDescription() const {
	return L"Мельницы обеспечивают Ваш город едой, необходимой для содержания солдат.";
}
std::wstring Windmill::getResourcesOverStr() const {
	return L"Растения закончились!\nОдна из Ваших мельниц прекращает работу.";
}
std::wstring Windmill::getUpgradeStartDescription() const {
	return L"Начато улучшение мельницы\nСбор еды был приостановлен.";
}
std::wstring Windmill::getUpgradeFinishDescription() const {
	return L"Улучшение мельницы завершено!\nСбор еды возобновлен.";
}
std::wstring Windmill::getBusyWithUpgradingDescription() const {
	return L"Мельница не доступна\nПодождите, пока будет завершено улучшение.";
}
uint32_t Windmill::getMaxLevel() const {
	return TOTAL_LEVELS;
}
Resources Windmill::getUpgradeCost(uint32_t i) const {
	return UPGRADE_COSTS[i];
}
uint32_t Windmill::getUpgradeMoves(uint32_t i) const {
	return UPGRADE_MOVES[i];
}