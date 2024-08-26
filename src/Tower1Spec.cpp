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


#include "Tower1Spec.hpp"
#include "Parameters.hpp"
#include "BigArrow.hpp"


Tower1Spec::Tower1Spec() = default;
std::shared_ptr<IBuildingSpec> Tower1Spec::clone() const {
    return std::make_shared<Tower1Spec>(*this);
}
Damage Tower1Spec::getDamage() const {
    return Parameters::get().getDamage("tower1_damage");
}
uint32_t Tower1Spec::getShotsNumber() const {
    return Parameters::get().getInt("tower1_shots_number");
}
uint32_t Tower1Spec::getShootingRadius() const {
    return Parameters::get().getInt("tower1_shooting_radius");
}
std::shared_ptr<Projectile> Tower1Spec::getProjectile() const {
    return std::make_shared<BigArrow>();
}


BOOST_CLASS_EXPORT_IMPLEMENT(Tower1Spec)