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


ResourcePoint::ResourcePoint() = default;
ResourcePoint::ResourcePoint(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t size) : HPGO(x, y, sx, sy, size, size) {}
Events ResourcePoint::newMove(Player* player) {
	return Events();
}
Events ResourcePoint::getGameObjectResponse(Player* player) {
	if (!this->exist()) {
		return Events();
	}
	return this->getSelectionWindow();
}
Events ResourcePoint::getSelectionWindow() {
	Events response;

	std::vector<GameActionWindowComponent> components;
	components.emplace_back("exit_icon", *Texts::get()->get("leave"), true, true, Events());
	components.emplace_back(this->getTextureName(), this->getDescription(), false, false, Events());

	std::shared_ptr<GameActionWindow> window = std::make_shared<GameActionWindow>(this->getSoundName(), "click", components);
	response.add(std::make_shared<CreateEEvent>(window));

	return response;
}