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


#include "AcademySpec.hpp"
#include "WhiteMage.hpp"


AcademySpec::AcademySpec() = default;
std::shared_ptr<IBuildingSpec> AcademySpec::clone() const {
    return std::make_shared<AcademySpec>(*this);
}
std::vector<std::shared_ptr<Warrior>> AcademySpec::getWarriorsToProduce(uint32_t playerId) {
    std::vector<std::shared_ptr<Warrior>> warriors;

    warriors.push_back(std::make_shared<WhiteMage>(0, 0, playerId));

    return warriors;
}
std::string AcademySpec::getProducingIconName() const {
    return "book_icon";
}
std::string AcademySpec::getWaitingIconName() const {
    return "mage_icon";
}


BOOST_CLASS_EXPORT_IMPLEMENT(AcademySpec)