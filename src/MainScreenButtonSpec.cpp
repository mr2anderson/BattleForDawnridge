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


#include "MainScreenButtonSpec.hpp"
#include "LabelWithImage.hpp"


MainScreenButtonSpec::MainScreenButtonSpec() {
	this->index = 0;
}
MainScreenButtonSpec::MainScreenButtonSpec(uint32_t index) {
	this->index = index;
}
std::shared_ptr<RectangularUiElement> MainScreenButtonSpec::getBase() const {
	return std::make_shared<LabelWithImage>(5, 40 + this->index * (64 + 10), 200, 64, this->getTextureName(), this->getString(), boost::none);
}