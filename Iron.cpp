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


#include "Iron.hpp"
#include "Texts.hpp"


Iron::Iron() = default;
Iron::Iron(uint32_t x, uint32_t y) : ResourcePoint(x, y) {

}
uint32_t Iron::getSX() const {
    return 1;
}
uint32_t Iron::getSY() const {
    return 1;
}
uint32_t Iron::getMaxHP() const {
    return 10500;
}
std::string Iron::getResourceType() const {
	return "iron";
}
std::string Iron::getTextureName() const {
	return "iron";
}
std::string Iron::getSoundName() const {
	return "iron";
}
std::wstring Iron::getDescription() const {
	return *Texts::get()->get("iron_description");
}