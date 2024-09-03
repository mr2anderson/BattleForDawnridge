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


#include "ChurchWarriorProducerSpec.hpp"
#include "Warrior.hpp"


ChurchWarriorProducerSpec::ChurchWarriorProducerSpec() = default;
std::shared_ptr<IBuildingSpec> ChurchWarriorProducerSpec::clone() const {
    return std::make_shared<ChurchWarriorProducerSpec>(*this);
}
std::vector<std::shared_ptr<Warrior>> ChurchWarriorProducerSpec::getWarriorsToProduce(uint32_t playerId) {
    std::vector<std::shared_ptr<Warrior>> toProduce;

    return toProduce;
}
std::string ChurchWarriorProducerSpec::getProducingIconName() const {
    return "cross_icon";
}
std::string ChurchWarriorProducerSpec::getWaitingIconName() const {
    return "princess_icon";
}


BOOST_CLASS_EXPORT_IMPLEMENT(ChurchWarriorProducerSpec)