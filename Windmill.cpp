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


Windmill::Windmill() = default;
Windmill::Windmill(uint32_t x, uint32_t y, const Player* playerPtr, const std::vector<ResourcePoint*>* resourcePointsPtr) : 
	ResourceBuilding(x, y, 2, 2, 20000, playerPtr, resourcePointsPtr),
	Building(x, y, 2, 2, 20000, playerPtr){}
Resources Windmill::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 10000));
	return cost;
}
uint32_t Windmill::getRegenerationSpeed() const {
	return 10000;
}
std::string Windmill::getTextureName() const {
	return "windmill";
}
std::wstring Windmill::getIsNotBuiltYetStr() const {
	return
		L"�������� ��� �� ���������\n"
		"��������� ����� �������������.\n"
		+ this->getReadableHpInfo() + L"\n"
		+ this->getReadableRegenerationSpeed();
}
std::wstring Windmill::getBuildingFinishedStr() const {
	return 
		L"�������� ���������\n"
		"��������� ����� �������, �������� ��� �������� �������� ������ ���.";
}
std::string Windmill::getNewWindowSoundName() const {
	return "leaves";
}
std::string Windmill::getResourceType() const {
	return "food";
}
uint32_t Windmill::getResourceNPerMove(uint32_t level) const {
	float levelCollectionSpeedBonus[TOTAL_LEVELS] = {
		1,
		3,
		6
	};
	return 2500 * levelCollectionSpeedBonus[level];
}
uint32_t Windmill::getRadius(uint32_t level) const {
	uint32_t levelRadiusBonus[TOTAL_LEVELS] = {
		0,
		2,
		3
	};
	return 5 + levelRadiusBonus[level];
}
std::wstring Windmill::getDescription() const {
	return
		L"�������� ������������ ��� ����� ����, ����������� ��� ���������� �����.\n"
		+ this->getReadableHpInfo();
}
std::wstring Windmill::getResourcesOverStr() const {
	return 
		L"�������� �����������\n"
		"���� �� ����� ������� ���������� ������.";
}
std::wstring Windmill::getUpgradeStartDescription() const {
	return
		L"������ ��������� ��������\n"
		"���� ��� ��� �������������.\n"
		"����� ����� �� ����� ���������: " + std::to_wstring(this->getUpgradeMoves());
}
std::wstring Windmill::getUpgradeFinishDescription() const {
	return 
		L"��������� �������� ���������\n"
		"���� ��� �����������.";
}
std::wstring Windmill::getBusyWithUpgradingDescription() const {
	return L"�������� ����������\n"
		"���������, ���� ����� ��������� ���������.\n"
		"����� ����� �� ����� ���������: " + std::to_wstring(this->getUpgradeMoves());
}
Resources Windmill::getUpgradeCost() const {
	Resources upgradeCosts[TOTAL_LEVELS - 1] = {
		Resources({{"wood", 20000}}),
		Resources({{"wood", 40000}})
	};
	return upgradeCosts[this->getCurrentLevel() - 1];
}
uint32_t Windmill::getUpgradeMoves() const {
	uint32_t upgradeMoves[TOTAL_LEVELS - 1] = {
		2,
		3
	};
	return upgradeMoves[this->getCurrentLevel() - 1];
}