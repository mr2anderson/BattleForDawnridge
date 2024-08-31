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


#include "ResourcePoint.hpp"
#include "CreateEEvent.hpp"
#include "WarriorHPPointer.hpp"
#include "Parameters.hpp"
#include "Warrior.hpp"

#include "PlaySoundEvent.hpp"


ResourcePoint::ResourcePoint() = default;
ResourcePoint::ResourcePoint(uint32_t x, uint32_t y) : HPGO(x, y, boost::none) {}
Events ResourcePoint::newMove(MapState *state, uint32_t playerId) {
	return Events();
}
uint32_t ResourcePoint::tryToCollect(uint32_t playerId, uint32_t value) {
	return std::min(this->getHP(), value);
}
bool ResourcePoint::warriorCanStay(std::shared_ptr<const Warrior> w) const {
	return true;
}
uint32_t ResourcePoint::getWarriorMovementCost(std::shared_ptr<Warrior> w) const {
    if (w->isFlying()) {
        return 1;
    }
	return this->getWalkingWarriorMovementCost();
}
StringLcl ResourcePoint::getDetailedDescription(MapState *state) const {
    return GO::getDetailedDescription(state) + "\n" +
            StringLcl("{left}") + std::to_string(this->getHP()) + "\n";
}
Events ResourcePoint::getResponse(MapState *state, uint32_t playerId, uint32_t button) {
	if (!this->exist() or button == sf::Mouse::Button::Right) {
		return Events();
	}
	return this->getSelectionWindow();
}
Events ResourcePoint::getSelectionWindow() {
	Events response;

	std::vector<HorizontalSelectionWindowComponent> components;
	components.push_back(this->getExitComponent());
	components.push_back(this->getDescriptionComponent());
	components.push_back(this->getResourceLeftComponent());
	components.push_back(this->getSlowMovementComponent());

	std::shared_ptr<HorizontalSelectionWindow> window = std::make_shared<HorizontalSelectionWindow>(components);
    response.add(std::make_shared<PlaySoundEvent>(this->getSoundName()));
	response.add(std::make_shared<CreateEEvent>(window));

	return response;
}
HorizontalSelectionWindowComponent ResourcePoint::getResourceLeftComponent() const {
	HorizontalSelectionWindowComponent component = {
		this->getResourceType() + "_icon",
        StringLcl("{left}") + std::to_string(this->getHP()),
		false,
		Events()
	};
	return component;
}
HorizontalSelectionWindowComponent ResourcePoint::getSlowMovementComponent() const {
	HorizontalSelectionWindowComponent component = {
		  "slow_movement_icon",
		StringLcl("{slow_movement}") + std::to_string(this->getWalkingWarriorMovementCost()),
		false,
		Events()
	};
	return component;
}
uint32_t ResourcePoint::getWalkingWarriorMovementCost() const {
	return Parameters::get().getInt("resource_point_warrior_movement_cost");
}


BOOST_CLASS_EXPORT_IMPLEMENT(ResourcePoint)