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


#include "WallSpec.hpp"
#include "Warrior.hpp"


WallSpec::WallSpec() = default;
std::shared_ptr<IBuildingSpec> WallSpec::clone() const {
	return std::make_shared<WallSpec>(*this);
}
uint32_t WallSpec::getWarriorMovementCost(std::shared_ptr<const Building> b, std::shared_ptr<Warrior> w) const {
    if (w->isFlying()) {
        return 1;
    }
	return 10000;
}
bool WallSpec::warriorCanStay(std::shared_ptr<const Building> b, std::shared_ptr<const Warrior> w) const {
	return w->isFlying();
}
bool WallSpec::isHighObstacle(std::shared_ptr<const Building> b, uint32_t playerId) const {
	return true;
}


BOOST_CLASS_EXPORT_IMPLEMENT(WallSpec)