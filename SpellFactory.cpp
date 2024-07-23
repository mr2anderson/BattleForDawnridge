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


#include "SpellFactory.hpp"
#include "Locales.hpp"
#include "SpellFactorySpec.hpp"
#include "Parameters.hpp"


SpellFactory::SpellFactory() = default;
SpellFactory::SpellFactory(uint32_t x, uint32_t y, uint32_t playerId) : Building(x, y, playerId) {
	this->addSpec(new SpellFactorySpec());
}
SpellFactory::Building* SpellFactory::createSameTypeBuilding() const {
	return new SpellFactory(this->getX(), this->getY(), this->getPlayerId());
}
Defence SpellFactory::getDefence() const {
	return Parameters::get()->getDefence("spell_factory_defence");
}
Resources SpellFactory::getCost() const {
	return Parameters::get()->getResources("spell_factory_cost");
}
uint32_t SpellFactory::getSX() const {
	return Parameters::get()->getInt("spell_factory_sx");
}
uint32_t SpellFactory::getSY() const {
	return Parameters::get()->getInt("spell_factory_sy");
}
uint32_t SpellFactory::getMaxHP() const {
	return Parameters::get()->getInt("spell_factory_max_hp");
}
std::string SpellFactory::getTextureName() const {
	return "spell_factory";
}
std::string SpellFactory::getSoundName() const {
	return "spell_factory";
}
std::wstring SpellFactory::getDescription() const {
	return *Locales::get()->get("spell_factory_description");
}
uint32_t SpellFactory::getRegenerationSpeed() const {
	return Parameters::get()->getInt("spell_factory_regeneration_speed");
}