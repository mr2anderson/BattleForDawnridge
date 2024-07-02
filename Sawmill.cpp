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


Sawmill::Sawmill(uint32_t x, uint32_t y, const Player* playerPtr, const std::vector<ResourcePoint*>* resourcePointsPtr) : ResourceBuilding(x, y, 10000, playerPtr, resourcePointsPtr) {}
uint32_t Sawmill::getRegenerationSpeed() const {
	return 2000;
}
std::string Sawmill::getTextureName() const {
	return "sawmill";
}
std::wstring Sawmill::getIsNotBuiltYetStr() const {
	return L"��������� ��� �� ���������\n��������� ����� �������������.";
}
std::wstring Sawmill::getBuildingFinishedStr() const {
	return L"��������� ���������!\n��������� ����� �������, ��������� ��� �������� �������� ������ ���������.";
}
std::string Sawmill::getNewWindowSoundName() const {
	return "sawmill";
}
std::string Sawmill::getResourceType() const {
	return "wood";
}
uint32_t Sawmill::getResourceNPerMove() const {
	return 7500;
}
uint32_t Sawmill::getRadius() const {
	return 5;
}
std::wstring Sawmill::getDescription() const {
	return L"��������� ������������ ��� ����� ���������� � �������� ���������� ������������� ����������� ����������.";
}
std::wstring Sawmill::getResourcesOverStr() const {
	return L"��������� �����������!\n���� �� ����� ��������� ���������� ������.";
}