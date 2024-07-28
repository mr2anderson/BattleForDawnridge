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


#include "InfirmaryWarriorProducerSpec.hpp"
#include "Healer.hpp"


InfirmaryWarriorProducerSpec::InfirmaryWarriorProducerSpec() = default;
IBuildingSpec *InfirmaryWarriorProducerSpec::clone() const {
    return new InfirmaryWarriorProducerSpec(*this);
}
std::vector<std::shared_ptr<Warrior>> InfirmaryWarriorProducerSpec::getWarriorsToProduce(uint32_t playerId) {
    std::vector<std::shared_ptr<Warrior>> toProduce;

    toProduce.push_back(std::make_shared<Healer>(0, 0, playerId));

    return toProduce;
}
std::string InfirmaryWarriorProducerSpec::getProducingIconName() const {
    return "cross_icon";
}
std::string InfirmaryWarriorProducerSpec::getWaitingIconName() const {
    return "healer_icon";
}