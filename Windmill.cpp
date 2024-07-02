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


Windmill::Windmill(uint32_t x, uint32_t y, const Player* playerPtr, const std::vector<ResourcePoint*>* resourcePointsPtr) : ResourceBuilding(x, y, 20000, playerPtr, resourcePointsPtr) {}
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
	return 2500;
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