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


#include "Mountains.hpp"
#include "Locales.hpp"
#include "Textures.hpp"
#include "TilesetHandler.hpp"
#include "CreateEEvent.hpp"
#include "Parameters.hpp"
#include "UUIDs.hpp"
#include "PlaySoundEvent.hpp"


Mountains::Mountains() = default;
Mountains::Mountains(uint32_t x, uint32_t y, uint32_t type) :
	GO(x, y) {
	this->type = type;
}
UUID Mountains::getTypeUUID() const {
	return UUIDs::get().get("mountains");
}
uint32_t Mountains::getSX() const {
	return Parameters::get().getInt("mountains_sx");
}
uint32_t Mountains::getSY() const {
	return Parameters::get().getInt("mountains_sy");
}
std::string Mountains::getTextureName() const {
	return "mountains";
}
sf::IntRect Mountains::getTextureRect() const {
	return TilesetHandler::get().getTextureRect(64 * this->getSX(), 64 * this->getSY(), Textures::get().get(this->getTextureName())->getSize().x, this->type);
}
std::string Mountains::getSoundName() const {
	return "wind";
}
StringLcl Mountains::getDescription() const {
	return StringLcl("{mountains_description}");
}
bool Mountains::isUltraHighObstacle(uint32_t playerId) const {
    return true;
}
bool Mountains::warriorCanStay(std::shared_ptr<const Warrior> w) const {
	return false;
}
uint32_t Mountains::getWarriorMovementCost(std::shared_ptr<Warrior> w) const {
	return 10000;
}
Events Mountains::getResponse(MapState *state, uint32_t playerId, uint32_t button) {
    if (button == sf::Mouse::Button::Right) {
        return Events();
    }

	std::vector<HorizontalSelectionWindowComponent> components;

	components.push_back(this->getExitComponent());
	components.push_back(this->getDescriptionComponent());

	std::shared_ptr<HorizontalSelectionWindow> window = std::make_shared<HorizontalSelectionWindow>(components);

	Events event;
	event.add(std::make_shared<PlaySoundEvent>(this->getSoundName()));
	event.add(std::make_shared<CreateEEvent>(window));

	return event;
}


BOOST_CLASS_EXPORT_IMPLEMENT(Mountains)