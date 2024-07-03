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
	Building(x, y, 2, 2, 20000, false, playerPtr) {}
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
	return L"����������� ��� �� ���������\n��������� ����� �������������.";
}
std::wstring Quarry::getBuildingFinishedStr() const {
	return L"����������� ���������\n��������� ����� �������, ����������� ��� �������� �������� ������ ������.";
}
std::string Quarry::getNewWindowSoundName() const {
	return "quarry";
}
std::string Quarry::getResourceType() const {
	return "stone";
}
uint32_t Quarry::getResourceNPerMove() const {
	float levelCollectionSpeedBonus[TOTAL_LEVELS] = {
		1,
		3,
		6
	};
	return 2500 * levelCollectionSpeedBonus[this->getCurrentLevel() - 1];
}
uint32_t Quarry::getRadius() const {
	uint32_t levelRadiusBonus[TOTAL_LEVELS] = {
		0,
		2,
		3
	};
	return 5 + levelRadiusBonus[this->getCurrentLevel() - 1];
}
std::wstring Quarry::getDescription() const {
	return L"����������� ������������ ��� ����� ������ � �������� ���������� ������������� �������������� ����������. ������: " + std::to_wstring(this->getHP()) + L" / " + std::to_wstring(this->getMaxHP()) + L".";
}
std::wstring Quarry::getResourcesOverStr() const {
	return L"������ ����������\n���� �� ����� ����������� ���������� ������.";
}
std::wstring Quarry::getUpgradeStartDescription() const {
	return L"������ ��������� �����������\n���� ����� ��� �������������.";
}
std::wstring Quarry::getUpgradeFinishDescription() const {
	return L"��������� ����������� ���������\n���� ����� �����������.";
}
std::wstring Quarry::getBusyWithUpgradingDescription() const {
	return L"����������� ����������\n���������, ���� ����� ��������� ���������.";
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