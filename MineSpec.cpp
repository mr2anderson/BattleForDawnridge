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


#include "MineSpec.hpp"
#include "Parameters.hpp"


MineSpec::MineSpec() = default;
IBuildingSpec* MineSpec::clone() const {
	return new MineSpec(*this);
}
std::string MineSpec::getResourceType() const {
	return "iron";
}
uint32_t MineSpec::getCollectionSpeed() const {
	return Parameters::get()->getInt("mine_collection_speed");
}
uint32_t MineSpec::getCollectionRadius() const {
	return Parameters::get()->getInt("mine_collection_radius");
}