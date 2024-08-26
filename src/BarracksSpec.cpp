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


#include "BarracksSpec.hpp"
#include "Infantryman.hpp"
#include "Legioner.hpp"
#include "Knight.hpp"
#include "Valkyr.hpp"
#include "BlackKnight.hpp"


BarracksSpec::BarracksSpec() = default;
std::shared_ptr<IBuildingSpec> BarracksSpec::clone() const {
	return std::make_shared<BarracksSpec>(*this);
}
std::vector<std::shared_ptr<Warrior> > BarracksSpec::getWarriorsToProduce(uint32_t playerId) {
	std::vector<std::shared_ptr<Warrior> > warriors;

	warriors.push_back(std::make_shared<Infantryman>(0, 0, playerId));
	warriors.push_back(std::make_shared<Legioner>(0, 0, playerId));
	warriors.push_back(std::make_shared<Knight>(0, 0, playerId));
    warriors.push_back(std::make_shared<Valkyr>(0, 0, playerId));
	warriors.push_back(std::make_shared<BlackKnight>(0, 0, playerId));

	return warriors;
}
std::string BarracksSpec::getProducingIconName() const {
    return "axe_icon";
}
std::string BarracksSpec::getWaitingIconName() const {
    return "infantryman_icon";
}


BOOST_CLASS_EXPORT_IMPLEMENT(BarracksSpec)