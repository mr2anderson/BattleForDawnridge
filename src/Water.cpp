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


#include "Water.hpp"
#include "Locales.hpp"
#include "Textures.hpp"
#include "TilesetHandler.hpp"
#include "CreateEEvent.hpp"
#include "Parameters.hpp"
#include "Warrior.hpp"
#include "UUIDs.hpp"
#include "PlaySoundEvent.hpp"


Water::Water() = default;
Water::Water(uint32_t x, uint32_t y, uint32_t type) :
	GO(x, y) {
	this->type = type;
}
UUID Water::getTypeUUID() const {
	return UUIDs::get().get("water");
}
uint32_t Water::getSY() const {
	return Parameters::get().getInt("water_sx");
}
uint32_t Water::getSX() const {
	return Parameters::get().getInt("water_sy");
}
std::string Water::getTextureName() const {
	return "water";
}
sf::IntRect Water::getTextureRect() const {
	return TilesetHandler::get().getTextureRect(64 * this->getSX(), 64 * this->getSY(), Textures::get().get(this->getTextureName())->getSize().x, this->type);
}
std::string Water::getSoundName() const {
	return "water";
}
StringLcl Water::getDescription() const {
	return StringLcl("{water_description}");
}
bool Water::isLowObstacle(uint32_t playerId) const {
    return true;
}
bool Water::warriorCanStay(std::shared_ptr<const Warrior> w) const {
	return w->isFlying();
}
uint32_t Water::getWarriorMovementCost(std::shared_ptr<Warrior> w) const {
    if (w->isFlying()) {
        return 1;
    }
	return 10000;
}
Events Water::getResponse(MapState *state, uint32_t playerId, uint32_t button) {
    if (button == sf::Mouse::Button::Right) {
        return Events();
    }

	std::vector<HorizontalSelectionWindowComponent> components;

	components.push_back(this->getExitComponent());
	components.push_back(this->getDescriptionComponent());

	std::shared_ptr<HorizontalSelectionWindow> w = std::make_shared<HorizontalSelectionWindow>(components);

	Events event;
	event.add(std::make_shared<PlaySoundEvent>(this->getSoundName()));
	event.add(std::make_shared<CreateEEvent>(w));

	return event;
}


BOOST_CLASS_EXPORT_IMPLEMENT(Water)