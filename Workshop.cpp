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


#include "Workshop.hpp"
#include "WorkshopWarriorProducerSpec.hpp"
#include "WorkshopWarriorHealerSpec.hpp"
#include "Parameters.hpp"
#include "Locales.hpp"


Workshop::Workshop() = default;
Workshop::Workshop(uint32_t x, uint32_t y, uint32_t playerId) :
        Building(x, y, playerId) {
    this->addSpec(new WorkshopWarriorProducerSpec());
    this->addSpec(new WorkshopWarriorHealerSpec());
}
Building* Workshop::createSameTypeBuilding() const {
    return new Workshop(this->getX(), this->getY(), this->getPlayerId());
}
uint32_t Workshop::getSX() const {
    return Parameters::get()->getInt("workshop_sx");
}
uint32_t Workshop::getSY() const {
    return Parameters::get()->getInt("workshop_sy");
}
uint32_t Workshop::getMaxHP() const {
    return Parameters::get()->getInt("workshop_max_hp");
}
Defence Workshop::getDefence() const {
    return Parameters::get()->getDefence("workshop_defence");
}
Resources Workshop::getCost() const {
    return Parameters::get()->getResources("workshop_cost");
}
std::string Workshop::getTextureName() const {
    return "workshop";
}
std::string Workshop::getSoundName() const {
    return "vehicle";
}
std::wstring Workshop::getDescription() const {
    return *Locales::get()->get("workshop_description");
}
uint32_t Workshop::getRegenerationSpeed() const {
    return Parameters::get()->getInt("workshop_regeneration_speed");
}


BOOST_CLASS_EXPORT_IMPLEMENT(Workshop)