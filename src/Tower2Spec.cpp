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


#include "Tower2Spec.hpp"
#include "Parameters.hpp"
#include "BigArrow.hpp"


Tower2Spec::Tower2Spec() = default;
std::shared_ptr<IBuildingSpec> Tower2Spec::clone() const {
    return std::make_shared<Tower2Spec>(*this);
}
Damage Tower2Spec::getDamage() const {
    return Parameters::get().getDamage("tower2_damage");
}
uint32_t Tower2Spec::getShotsNumber() const {
    return Parameters::get().getInt("tower2_shots_number");
}
uint32_t Tower2Spec::getShootingRadius() const {
    return Parameters::get().getInt("tower2_shooting_radius");
}
std::shared_ptr<Projectile> Tower2Spec::getProjectile() const {
    return std::make_shared<BigArrow>();
}


BOOST_CLASS_EXPORT_IMPLEMENT(Tower2Spec)