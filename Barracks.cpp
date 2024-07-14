/*
 *  Battle for Dawnridge
 *  Copyright (C) 2024 mr2anderson
 *
 *  Battle for Dawnridge is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Battle for Dawnridge is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Battle for Dawnridge.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "Barracks.hpp"
#include "Infantryman.hpp"
#include "Legioner.hpp"
#include "Knight.hpp"
#include "BlackKnight.hpp"
#include "Texts.hpp"


Barracks::Barracks() = default;
Barracks::Barracks(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units, std::shared_ptr<GOCollection<GO>> go, uint32_t mapW, uint32_t mapH) :
	WarriorProducer(x, y, playerId, units, go, mapW, mapH),
	Building(x, y, playerId, units) {

}
Building* Barracks::cloneBuilding() const {
	return new Barracks(*this);
}
uint32_t Barracks::getSX() const {
    return 2;
}
uint32_t Barracks::getSY() const {
    return 2;
}
uint32_t Barracks::getMaxHP() const {
    return 10000;
}
Resources Barracks::getCost() const {
	return Resources({ Resource("stone", 20000) });
}
std::string Barracks::getTextureName() const {
	return "barracks";
}
std::string Barracks::getSoundName() const {
	return "hooray";
}
std::wstring Barracks::getDescription() const {
	return *Texts::get()->get("barracks_description");
}
std::vector<std::shared_ptr<Warrior>> Barracks::getWarriorsToProduce() {
	std::vector<std::shared_ptr<Warrior>> warriors;
	warriors.push_back(std::make_shared<Infantryman>(0, 0, this->getPlayerId(), this->getUnits(), this->getGO(), this->getMapW(), this->getMapH()));
	warriors.push_back(std::make_shared<Legioner>(0, 0, this->getPlayerId(), this->getUnits(), this->getGO(), this->getMapW(), this->getMapH()));
	warriors.push_back(std::make_shared<Knight>(0, 0, this->getPlayerId(), this->getUnits(), this->getGO(), this->getMapW(), this->getMapH()));
	warriors.push_back(std::make_shared<BlackKnight>(0, 0, this->getPlayerId(), this->getUnits(), this->getGO(), this->getMapW(), this->getMapH()));
	return warriors;
}
uint32_t Barracks::getRadius() const {
	return 1;
}
uint32_t Barracks::getRegenerationSpeed() const {
	return 2500;
}
std::wstring Barracks::getUpperCaseReadableName() const {
	return *Texts::get()->get("barracks_upper_case_readable_name");
}