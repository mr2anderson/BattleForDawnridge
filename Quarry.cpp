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


Quarry::Quarry(uint32_t x, uint32_t y, const Player* playerPtr, const std::vector<ResourcePoint*>* resourcePointsPtr) : ResourceBuilding(x, y, 10000, playerPtr, resourcePointsPtr) {}
uint32_t Quarry::getRegenerationSpeed() const {
	return 2000;
}
std::string Quarry::getTextureName() const {
	return "quarry";
}
std::wstring Quarry::getIsNotBuiltYetStr() const {
	return L"Каменоломня еще не построена\nДождитесь конца строительства.";
}
std::wstring Quarry::getBuildingFinishedStr() const {
	return L"Каменоломня построена!\nБлагодаря Вашим рабочим, каменоломня уже начинает добывать первый камень.";
}
std::string Quarry::getNewWindowSoundName() const {
	return "quarry";
}
std::string Quarry::getResourceType() const {
	return "stone";
}
uint32_t Quarry::getResourceNPerMove() const {
	return 1500;
}
uint32_t Quarry::getRadius() const {
	return 5;
}
std::wstring Quarry::getDescription() const {
	return L"Каменоломни обеспечивают Ваш город камнем — основным материалом строительства оборонительных сооружений.";
}
std::wstring Quarry::getResourcesOverStr() const {
	return L"Камень закончился!\nОдна из Ваших каменеломен прекращает работу.";
}