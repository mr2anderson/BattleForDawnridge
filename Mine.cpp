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


Mine::Mine(uint32_t x, uint32_t y, const Player* playerPtr, const std::vector<ResourcePoint*>* resourcePointsPtr) : ResourceBuilding(x, y, 10000, playerPtr, resourcePointsPtr) {}
uint32_t Mine::getRegenerationSpeed() const {
	return 2000;
}
std::string Mine::getTextureName() const {
	return "mine";
}
std::wstring Mine::getIsNotBuiltYetStr() const {
	return L"����� ��� �� ���������\n��������� ����� �������������.";
}
std::wstring Mine::getBuildingFinishedStr() const {
	return L"����� ���������!\n��������� ����� �������, ����� ��� �������� �������� ������ ������.";
}
std::string Mine::getNewWindowSoundName() const {
	return "mine";
}
std::string Mine::getResourceType() const {
	return "iron";
}
uint32_t Mine::getResourceNPerMove() const {
	return 1500;
}
uint32_t Mine::getRadius() const {
	return 5;
}
std::wstring Mine::getDescription() const {
	return L"����� ������������ ��� ����� ������� � ������� ������� �����.";
}
std::wstring Mine::getResourcesOverStr() const {
	return L"������ ��������!\n���� �� ����� ���� ���������� ������.";
}