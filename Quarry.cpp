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


Quarry::Quarry() = default;
Quarry::Quarry(uint32_t x, uint32_t y, const Player* playerPtr, const std::vector<ResourcePoint*>* resourcePointsPtr) : 
	ResourceBuilding(x, y, 2, 2, 20000, playerPtr, resourcePointsPtr),
	Building(x, y, 2, 2, 20000, playerPtr) {}
Resources Quarry::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 10000));
	return cost;
}
uint32_t Quarry::getRegenerationSpeed() const {
	return 10000;
}
std::string Quarry::getTextureName() const {
	return "quarry";
}
std::wstring Quarry::getIsNotBuiltYetStr() const {
	return
		L"ÊÀÌÅÍÎËÎÌÍß ÅÙÅ ÍÅ ÏÎÑÒÐÎÅÍÀ\n"
		"Äîæäèòåñü êîíöà ñòðîèòåëüñòâà.\n"
		+ this->getReadableHpInfo() + L"\n"
		+ this->getReadableRegenerationSpeed();
}
std::wstring Quarry::getBuildingFinishedStr() const {
	return
		L"ÊÀÌÅÍÎËÎÌÍß ÏÎÑÒÐÎÅÍÀ\n"
		"Áëàãîäàðÿ Âàøèì ðàáî÷èì, êàìåíîëîìíÿ óæå íà÷èíàåò äîáûâàòü ïåðâûé êàìåíü.";
}
std::string Quarry::getNewWindowSoundName() const {
	return "quarry";
}
std::string Quarry::getResourceType() const {
	return "stone";
}
uint32_t Quarry::getResourceNPerMove(uint32_t level) const {
	float levelCollectionSpeedBonus[TOTAL_LEVELS] = {
		1,
		3,
		6
	};
	return 2500 * levelCollectionSpeedBonus[level];
}
uint32_t Quarry::getRadius(uint32_t level) const {
	uint32_t levelRadiusBonus[TOTAL_LEVELS] = {
		0,
		2,
		3
	};
	return 5 + levelRadiusBonus[level];
}
std::wstring Quarry::getDescription() const {
	return
		L"Êàìåíîëîìíè îáåñïå÷èâàþò Âàø ãîðîä êàìíåì, íåîáõîäèìûì äëÿ ñòðîèòåëüñòâà îáîðîíû.\n"
		+ this->getReadableHpInfo();
}
std::wstring Quarry::getResourcesOverStr() const {
	return
		L"ÊÀÌÅÍÜ ÇÀÊÎÍ×ÈËÑß\n"
		"Îäíà èç Âàøèõ êàìåíîëîìåí ïðåêðàùàåò ðàáîòó.";
}
std::wstring Quarry::getUpgradeStartDescription() const {
	return
		L"ÍÀ×ÀÒÎ ÓËÓ×ØÅÍÈÅ ÊÀÌÅÍÎËÎÌÍÈ\n"
		"Ñáîð êàìíÿ áûë ïðèîñòàíîâëåí.\n"
		"×èñëî õîäîâ äî êîíöà óëó÷øåíèÿ: " + std::to_wstring(this->getUpgradeMoves());
}
std::wstring Quarry::getUpgradeFinishDescription() const {
	return
		L"ÓËÓ×ØÅÍÈÅ ÊÀÌÅÍÎËÎÌÍÈ ÇÀÂÅÐØÅÍÎ\n"
		"Ñáîð êàìíÿ âîçîáíîâëåí.";
}
std::wstring Quarry::getBusyWithUpgradingDescription() const {
	return L"ÊÀÌÅÍÎËÎÌÍß ÍÅÄÎÑÒÓÏÍÀ\n"
		"Ïîäîæäèòå, ïîêà áóäåò çàâåðøåíî óëó÷øåíèå.\n"
		"×èñëî õîäîâ äî êîíöà óëó÷øåíèÿ: " + std::to_wstring(this->getUpgradeMoves());
}
Resources Quarry::getUpgradeCost() const {
	Resources upgradeCosts[TOTAL_LEVELS - 1] = {
		Resources({{"wood", 20000}}),
		Resources({{"wood", 40000}})
	};
	return upgradeCosts[this->getCurrentLevel() - 1];
}
uint32_t Quarry::getUpgradeMoves() const {
	uint32_t upgradeMoves[TOTAL_LEVELS - 1] = {
		2,
		3
	};
	return upgradeMoves[this->getCurrentLevel() - 1];
}