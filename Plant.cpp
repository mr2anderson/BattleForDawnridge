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


#include "Plant.hpp"


const uint32_t Plant::ANIMATION_N[Plant::TOTAL_TYPES] = { 60, 60, 20, 30, 90, 90, 90, 60, 60, 60, 60, 30, 20};
const std::string Plant::PLANT_NAMES[Plant::TOTAL_TYPES] = {"BlueFlower", "BluePlantClosed", "JumpPlant 2", "Plant Wind 1", "Plant1", "Plant2", "Plant3", "Plant4", "Plant5", "Plant6", "Plant7", "PlantPosion", "JumpPlant"};


Plant::Plant(uint32_t x, uint32_t y) : ResourcePoint(x, y, 10000) {
	std::random_device rd;
	std::mt19937 mersenne(rd());
	this->type = mersenne() % TOTAL_TYPES;
}
std::string Plant::getResourceType() const {
	return "food";
}
std::string Plant::getTextureName() const {
	uint32_t ms = this->clock.getElapsedTime().asMilliseconds();
	uint32_t frames = ms / (1000 / ANIMATION_N[this->type]);
	uint32_t currentFrame = frames % ANIMATION_N[this->type];
	return "plant" + std::to_string(this->type) + "_" + std::to_string(currentFrame);
}
std::string Plant::getIconName() const {
	return "plant" + std::to_string(this->type) + "_0";
}
std::wstring Plant::getDescription() const {
	return L"Ёто волшебное растение содержит еще " + std::to_wstring(this->getHP()) + L" ед. еды. ѕостройте р€дом мельницу, чтобы начать добычу.";
}
std::string Plant::getClickSoundName() const {
	return "leaves";
}