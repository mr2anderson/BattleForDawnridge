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


Mountains::Mountains() = default;
Mountains::Mountains(uint32_t x, uint32_t y) :
	GO(x, y, 2, 2) {

}
Events Mountains::newMove(std::shared_ptr<Player> currentPlayer) {
	return Events();
}
std::string Mountains::getTextureName() const {
	return "mountains";
}
std::string Mountains::getSoundName() const {
	return "wind";
}
std::wstring Mountains::getDescription() const {
	return *Texts::get()->get("mountains_description");
}
bool Mountains::exist() const {
	return true;
}
Events Mountains::getGameObjectResponse(std::shared_ptr<Player> currentPlayer) {
	Events response;

	std::vector<GameActionWindowComponent> components;
	components.emplace_back("exit_icon", *Texts::get()->get("leave"), true, true, Events());
	components.emplace_back(this->getTextureName(), this->getDescription(), false, false, Events());

	std::shared_ptr<GameActionWindow> window = std::make_shared<GameActionWindow>(this->getSoundName(), "click", components);
	response.add(std::make_shared<CreateEEvent>(window));

	return response;
}