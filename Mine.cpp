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


Mine::Mine() = default;
Mine::Mine(uint32_t x, uint32_t y, const Player* playerPtr, const std::vector<ResourcePoint*>* resourcePointsPtr) : 
	ResourceBuilding(x, y, 2, 2, 20000, playerPtr, resourcePointsPtr),
	Building(x, y, 2, 2, 20000, playerPtr) {}
Resources Mine::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 10000));
	return cost;
}
uint32_t Mine::getRegenerationSpeed() const {
	return 10000;
}
std::string Mine::getTextureName() const {
	return "mine";
}
std::wstring Mine::getIsNotBuiltYetStr() const {
	return L"ØÀÕÒÀ ÅÙÅ ÍÅ ÏÎÑÒÐÎÅÍÀ\nÄîæäèòåñü êîíöà ñòðîèòåëüñòâà.";
}
std::wstring Mine::getBuildingFinishedStr() const {
	return L"ØÀÕÒÀ ÏÎÑÒÐÎÅÍÀ\nÁëàãîäàðÿ Âàøèì ðàáî÷èì, øàõòà óæå íà÷èíàåò äîáûâàòü ïåðâîå æåëåçî.";
}
std::string Mine::getNewWindowSoundName() const {
	return "mine";
}
std::string Mine::getResourceType() const {
	return "iron";
}
uint32_t Mine::getResourceNPerMove() const {
	float levelCollectionSpeedBonus[TOTAL_LEVELS] = {
		1,
		3,
		6
	};
	return 2500 * levelCollectionSpeedBonus[this->getCurrentLevel() - 1];
}
uint32_t Mine::getRadius() const {
	uint32_t levelRadiusBonus[TOTAL_LEVELS] = {
		0,
		2,
		3
	};
	return 5 + levelRadiusBonus[this->getCurrentLevel() - 1];
}
std::wstring Mine::getDescription() const {
	return L"Øàõòû îáåñïå÷èâàþò Âàø ãîðîä æåëåçîì — îñíîâîé ñèëüíîé àðìèè. Çàùèòà: " + std::to_wstring(this->getHP()) + L" / " + std::to_wstring(this->getMaxHP()) + L".";
}
std::wstring Mine::getResourcesOverStr() const {
	return L"ÇÀËÅÆÈ ÈÑÒÎÙÅÍÛ\nÎäíà èç Âàøèõ øàõò ïðåêðàùàåò ðàáîòó.";
}
std::wstring Mine::getUpgradeStartDescription() const {
	return L"ÍÀ×ÀÒÎ ÓËÓ×ØÅÍÈÅ ØÀÕÒÛ\nÑáîð æåëåçà áûë ïðèîñòàíîâëåí.";
}
std::wstring Mine::getUpgradeFinishDescription() const {
	return L"ÓËÓ×ØÅÍÈÅ ØÀÕÒÛ ÇÀÂÅÐØÅÍÎ\nÑáîð æåëåçà âîçîáíîâëåí.";
}
std::wstring Mine::getBusyWithUpgradingDescription() const {
	return L"ØÀÕÒÀ ÍÅÄÎÑÒÓÏÍÀ\nÏîäîæäèòå, ïîêà áóäåò çàâåðøåíî óëó÷øåíèå.";
}
Resources Mine::getUpgradeCost() const {
	Resources upgradeCosts[TOTAL_LEVELS - 1] = {
		Resources({{"wood", 20000}}),
		Resources({{"wood", 40000}})
	};
	return upgradeCosts[this->getCurrentLevel() - 1];
}
uint32_t Mine::getUpgradeMoves() const {
	uint32_t upgradeMoves[TOTAL_LEVELS - 1] = {
		2,
		3
	};
	return upgradeMoves[this->getCurrentLevel() - 1];
}