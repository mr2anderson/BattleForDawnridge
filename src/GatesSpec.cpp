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


#include "GatesSpec.hpp"
#include "Parameters.hpp"
#include "Building.hpp"
#include "Warrior.hpp"


GatesSpec::GatesSpec() = default;
std::shared_ptr<IBuildingSpec> GatesSpec::clone() const {
	return std::make_shared<GatesSpec>(*this);
}
uint32_t GatesSpec::getWarriorMovementCost(std::shared_ptr<const Building> building, std::shared_ptr<Warrior> w) const {
    if (w->isFlying()) {
        return 1;
    }
    return 1 + 9999 * (building->getPlayerId() != w->getPlayerId());
}
bool GatesSpec::warriorCanStay(std::shared_ptr<const Building> building, std::shared_ptr<const Warrior> w) const {
    if (w->isFlying()) {
        return true;
    }
    return (building->getPlayerId() == w->getPlayerId());
}
bool GatesSpec::conductsIfNotWork() const {
	return true;
}
uint32_t GatesSpec::getRadius() const {
	return Parameters::get().getInt("gates_radius");
}


BOOST_CLASS_EXPORT_IMPLEMENT(GatesSpec)