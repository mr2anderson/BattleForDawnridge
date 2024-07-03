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



#include "Quarry.hpp"


const Resources Quarry::UPGRADE_COSTS[Quarry::TOTAL_LEVELS - 1] = {
	Resources({{"wood", 5000}}),
	Resources({{"wood", 5000}}),
};
const uint32_t Quarry::UPGRADE_MOVES[Quarry::TOTAL_LEVELS - 1] = {
	2,
	2,
};
const float Quarry::LEVEL_BONUS[Quarry::TOTAL_LEVELS] = {
	1,
	1.5,
	2
};
const uint32_t Quarry::LEVEL_RADIUS_BONUS[Quarry::TOTAL_LEVELS]{
	0,
	1,
	2
};


Quarry::Quarry(uint32_t x, uint32_t y, const Player* playerPtr, const std::vector<ResourcePoint*>* resourcePointsPtr) : ResourceBuilding(x, y, 2, 2, 20000, playerPtr, resourcePointsPtr) {}
Resources Quarry::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 10000));
	return cost;
}
uint32_t Quarry::getRegenerationSpeed() const {
	return 4000;
}
std::string Quarry::getTextureName() const {
	return "quarry";
}
std::wstring Quarry::getIsNotBuiltYetStr() const {
	return L"КАМЕНОЛОМНЯ ЕЩЕ НЕ ПОСТРОЕНА\nДождитесь конца строительства.";
}
std::wstring Quarry::getBuildingFinishedStr() const {
	return L"КАМЕНОЛОМНЯ ПОСТРОЕНА\nБлагодаря Вашим рабочим, каменоломня уже начинает добывать первый камень.";
}
std::string Quarry::getNewWindowSoundName() const {
	return "quarry";
}
std::string Quarry::getResourceType() const {
	return "stone";
}
uint32_t Quarry::getResourceNPerMove() const {
	return 2500 * LEVEL_BONUS[this->getCurrentLevel() - 1];
}
uint32_t Quarry::getRadius() const {
	return 5 + LEVEL_RADIUS_BONUS[this->getCurrentLevel() - 1];
}
std::wstring Quarry::getDescription() const {
	return L"Каменоломни обеспечивают Ваш город камнем — основным материалом строительства оборонительных сооружений. Защита: " + std::to_wstring(this->getHP()) + L" / " + std::to_wstring(this->getMaxHP()) + L".";
}
std::wstring Quarry::getResourcesOverStr() const {
	return L"КАМЕНЬ ЗАКОНЧИЛСЯ\nОдна из Ваших каменеломен прекращает работу.";
}
std::wstring Quarry::getUpgradeStartDescription() const {
	return L"НАЧАТО УЛУЧШЕНИЕ КАМЕНОЛОМНИ\nСбор камня был приостановлен.";
}
std::wstring Quarry::getUpgradeFinishDescription() const {
	return L"УЛУЧШЕНИЕ КАМЕНОЛОМНИ ЗАВЕРШЕНО\nСбор камня возобновлен.";
}
std::wstring Quarry::getBusyWithUpgradingDescription() const {
	return L"КАМЕНОЛОМНЯ НЕДОСТУПНА\nПодождите, пока будет завершено улучшение.";
}
uint32_t Quarry::getMaxLevel() const {
	return TOTAL_LEVELS;
}
Resources Quarry::getUpgradeCost(uint32_t i) const {
	return UPGRADE_COSTS[i];
}
uint32_t Quarry::getUpgradeMoves(uint32_t i) const {
	return UPGRADE_MOVES[i];
}