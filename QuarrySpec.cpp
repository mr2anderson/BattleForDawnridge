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


#include "QuarrySpec.hpp"
#include "Parameters.hpp"


QuarrySpec::QuarrySpec() = default;
IBuildingSpec* QuarrySpec::clone() const {
	return new QuarrySpec(*this);
}
std::string QuarrySpec::getResourceType() const {
	return "stone";
}
uint32_t QuarrySpec::getCollectionSpeed() const {
	return Parameters::get()->getInt("quarry_collection_speed");
}
uint32_t QuarrySpec::getCollectionRadius() const {
	return Parameters::get()->getInt("quarry_collection_radius");
}


BOOST_CLASS_EXPORT_IMPLEMENT(QuarrySpec)