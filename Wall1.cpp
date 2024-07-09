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


#include "Wall1.hpp"


Wall1::Wall1() = default;
Wall1::Wall1(uint32_t x, uint32_t y, std::shared_ptr<Player> playerPtr) :
	Building(x, y, 2, 2, 30000, playerPtr) {

}
Building* Wall1::cloneBuilding() const {
	return new Wall1(*this);
}
Events Wall1::newMove(std::shared_ptr<Player> player) {
	Events response;
	if (this->belongTo(player) and this->exist()) {
		return  this->regenerate();
	}
	return response;
}
Resources Wall1::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 2000));
	return cost;
}
uint32_t Wall1::getRegenerationSpeed() const {
	return 10000;
}
std::string Wall1::getTextureName() const {
	return "wall1";
}
std::string Wall1::getSoundName() const {
	return "stone";
}
std::wstring Wall1::getDescription() const {
	return *Texts::get()->get("wall1_description");
}
std::wstring Wall1::getUpperCaseReadableName() const {
	return *Texts::get()->get("wall1_upper_case_readable_name");
}
Events Wall1::getSelectionW() {
	Events response;

	std::vector<GameActionWindowComponent> components;
	components.push_back(this->getExitComponent());
	components.push_back(this->getDescriptionComponent());
	components.push_back(this->getHpInfoComponent());

	std::shared_ptr<GameActionWindow> window = std::make_shared<GameActionWindow>(this->getSoundName(), "click", components);
	response.add(std::make_shared<CreateEEvent>(window));

	return response;
}
Events Wall1::getGameObjectResponse(std::shared_ptr<Player> player) {
	if (this->belongTo(player)) {
		return this->getSelectionW();
	}
	return this->getUnitOfEnemyResponse();
}