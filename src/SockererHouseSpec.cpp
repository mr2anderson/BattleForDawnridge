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


#include "SockererHouseSpec.hpp"
#include "Golem.hpp"


SockererHouseSpec::SockererHouseSpec() = default;
std::shared_ptr<IBuildingSpec> SockererHouseSpec::clone() const {
	return std::make_shared<SockererHouseSpec>(*this);
}
std::vector<std::shared_ptr<Warrior>> SockererHouseSpec::getWarriorsToProduce(uint32_t playerId) {
	std::vector<std::shared_ptr<Warrior>> warriors;

	warriors.push_back(std::make_shared<Golem>(0, 0, playerId));

	return warriors;
}
std::string SockererHouseSpec::getProducingIconName() const {
	return "magick_icon";
}
std::string SockererHouseSpec::getWaitingIconName() const {
	return "sockerer_icon";
}


BOOST_CLASS_EXPORT_IMPLEMENT(SockererHouseSpec)