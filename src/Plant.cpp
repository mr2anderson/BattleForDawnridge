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


#include "Plant.hpp"
#include "Locales.hpp"
#include "Parameters.hpp"
#include "GlobalRandomGenerator32.hpp"
#include "GlobalClock.hpp"
#include "UUIDs.hpp"
#include "CircleLightSourceStatic.hpp"


const uint32_t Plant::TOTAL_TYPES = 13;
const uint32_t Plant::ANIMATION_NUMBER[Plant::TOTAL_TYPES] = {60, 60, 90, 90, 90, 60, 60, 60, 60, 30, 20, 20, 30};


Plant::Plant() {
    this->type = 1;
}
Plant::Plant(uint32_t x, uint32_t y) : AreaResourcePoint(x, y) {
    this->type = GlobalRandomGenerator32::get().gen() % TOTAL_TYPES + 1;
}
UUID Plant::getTypeUUID() const {
    return UUIDs::get().get("plant");
}
uint32_t Plant::getSX() const {
    return Parameters::get().getInt("plant_sx");
}
uint32_t Plant::getSY() const {
    return Parameters::get().getInt("plant_sy");
}
uint32_t Plant::getMaxHP() const {
    return Parameters::get().getInt("plant_max_hp");
}
std::string Plant::getResourceType() const {
    return "crystal";
}
std::string Plant::getSoundName() const {
    return "crystal";
}
std::string Plant::getTextureName() const {
    return "plant" + std::to_string(this->type) + "_" + std::to_string((GlobalClock::get().getMs() / (1000 / ANIMATION_NUMBER[this->type - 1])) % ANIMATION_NUMBER[this->type - 1] + 1);
}
StringLcl Plant::getDescription() const {
    return StringLcl("{plant_description}");
}
std::shared_ptr<ILightSource> Plant::getLightSource() const {
    return std::make_shared<CircleLightSourceStatic>(this->getCenterX(), this->getCenterY(), 28);
}


BOOST_CLASS_EXPORT_IMPLEMENT(Plant)