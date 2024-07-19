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
#include "Texts.hpp"
#include "HPPointer.hpp"


ResourcePoint::ResourcePoint() = default;
ResourcePoint::ResourcePoint(uint32_t x, uint32_t y) : HPGO(x, y, std::nullopt) {}
Events ResourcePoint::newMove(MapState *state, uint32_t playerId) {
	return Events();
}
uint32_t ResourcePoint::tryToCollect(uint32_t playerId, uint32_t value) {
	return std::min(this->getHP(), value);
}
bool ResourcePoint::warriorCanStay(uint32_t warriorPlayerId) const {
	return true;
}
uint32_t ResourcePoint::getWarriorMovementCost(uint32_t warriorPlayerId) const {
	return 3;
}
uint8_t ResourcePoint::getHPPointerOrientation() const {
	return HPPointer::ORIENTATION::UP;
}
Events ResourcePoint::getResponse(MapState *state, uint32_t playerId) {
	if (!this->exist()) {
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

	std::shared_ptr<HorizontalSelectionWindow> window = std::make_shared<HorizontalSelectionWindow>(components);
    response.add(std::make_shared<PlaySoundEvent>(this->getSoundName()));
	response.add(std::make_shared<CreateEEvent>(window));

	return response;
}
HorizontalSelectionWindowComponent ResourcePoint::getResourceLeftComponent() const {
	HorizontalSelectionWindowComponent component = {
		this->getResourceType() + "_icon",
		*Texts::get()->get("left") + std::to_wstring(this->getHP()),
		false,
		Events()
	};
	return component;
}