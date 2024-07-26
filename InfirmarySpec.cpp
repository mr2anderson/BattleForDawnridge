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


#include "InfirmarySpec.hpp"
#include "Parameters.hpp"
#include "Warrior.hpp"


InfirmarySpec::InfirmarySpec() = default;
IBuildingSpec* InfirmarySpec::clone() const {
	return new InfirmarySpec(*this);
}
uint32_t InfirmarySpec::getHealingSpeed() const {
	return Parameters::get()->getInt("infirmary_healing_speed");
}
uint8_t InfirmarySpec::getWarriorToHealType() const {
    return Warrior::BEEN_HIT_SOUND_TYPE::HUMAN;
}
std::string InfirmarySpec::getHealTextureName() const {
    return "christianity";
}