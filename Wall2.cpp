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


#include "Wall2.hpp"


Wall2::Wall2() = default;
Wall2::Wall2(uint32_t x, uint32_t y, std::shared_ptr<Player> playerPtr) :
	Building(x, y, 2, 2, 150000, playerPtr) {

}
Building* Wall2::cloneBuilding() const {
	return new Wall2(*this);
}
Events Wall2::newMove(std::shared_ptr<Player> player) {
	Events response;
	if (this->belongTo(player) and this->exist()) {
		return  this->regenerate();
	}
	return response;
}
Resources Wall2::getCost() const {
	Resources cost;
	cost.plus(Resource("stone", 10000));
	return cost;
}
uint32_t Wall2::getRegenerationSpeed() const {
	return 37500;
}
std::string Wall2::getTextureName() const {
	return "wall2";
}
std::string Wall2::getSoundName() const {
	return "stone";
}
std::wstring Wall2::getDescription() const {
	return *Texts::get()->get("wall2_description");
}
std::wstring Wall2::getUpperCaseReadableName() const {
	return *Texts::get()->get("wall2_upper_case_readable_name");
}
Events Wall2::getSelectionW() {
	Events response;

	std::vector<GameActionWindowComponent> components;
	components.push_back(this->getExitComponent());
	components.push_back(this->getDescriptionComponent());
	components.push_back(this->getHpInfoComponent());

	std::shared_ptr<GameActionWindow> window = std::make_shared<GameActionWindow>(this->getSoundName(), "click", components);
	response.add(std::make_shared<CreateEEvent>(window));

	return response;
}
Events Wall2::getGameObjectResponse(std::shared_ptr<Player> player) {
	if (this->belongTo(player)) {
		return this->getSelectionW();
	}
	return this->getUnitOfEnemyResponse();
}