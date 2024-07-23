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
#include "Balance.hpp"
#include "Building.hpp"


GatesSpec::GatesSpec() = default;
IBuildingSpec* GatesSpec::clone() const {
	return new GatesSpec(*this);
}
uint32_t GatesSpec::getWarriorMovementCost(const Building *building, uint32_t playerId) const {
    return 1 + 9999 * (building->getPlayerId() != playerId);
}
bool GatesSpec::warriorCanStay(const Building *building, uint32_t playerId) const {
    return (building->getPlayerId() == playerId);
}
bool GatesSpec::conductsIfNotWork() const {
	return true;
}
uint32_t GatesSpec::getRadius() const {
	return Balance::get()->getInt("gates_radius");
}