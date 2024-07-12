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


#include "Wall1.hpp"
#include "Texts.hpp"


Wall1::Wall1() = default;
Wall1::Wall1(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units) :
	Wall(x, y, 2, 2, 40000, playerId, units) {

}
Building* Wall1::cloneBuilding() const {
	return new Wall1(*this);
}
Resources Wall1::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 5000));
	return cost;
}
uint32_t Wall1::getRegenerationSpeed() const {
	return 10000;
}
std::string Wall1::getTextureName() const {
	return "wall1";
}
std::string Wall1::getSoundName() const {
	return "stone";
}
std::wstring Wall1::getDescription() const {
	return *Texts::get()->get("wall1_description");
}
std::wstring Wall1::getUpperCaseReadableName() const {
	return *Texts::get()->get("wall1_upper_case_readable_name");
}