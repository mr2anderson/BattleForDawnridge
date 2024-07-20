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
#include "Texts.hpp"
#include "SpellFactorySpec.hpp"


SpellFactory::SpellFactory() = default;
SpellFactory::SpellFactory(uint32_t x, uint32_t y, uint32_t playerId) : Building(x, y, playerId) {
	this->addSpec(new SpellFactorySpec());
}
SpellFactory::Building* SpellFactory::cloneBuilding() const {
	return new SpellFactory(*this);
}
Defence SpellFactory::getDefence() const {
	return Defence::WOOD;
}
Resources SpellFactory::getCost() const {
	Resources cost;
	cost.plus(Resource("stone", 20000));
	return cost;
}
uint32_t SpellFactory::getSX() const {
	return 2;
}
uint32_t SpellFactory::getSY() const {
	return 2;
}
uint32_t SpellFactory::getMaxHP() const {
	return 10000;
}
std::string SpellFactory::getTextureName() const {
	return "spell_factory";
}
std::string SpellFactory::getSoundName() const {
	return "spell_factory";
}
std::wstring SpellFactory::getDescription() const {
	return *Texts::get()->get("spell_factory_description");
}
uint32_t SpellFactory::getRegenerationSpeed() const {
	return this->getMaxHP() / 1;
	return this->getMaxHP() / 4;
}
std::wstring SpellFactory::getUpperCaseReadableName() const {
	return *Texts::get()->get("spell_factory_upper_case_readable_name");
}