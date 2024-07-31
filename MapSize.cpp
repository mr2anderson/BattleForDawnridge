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


#include "MapSize.hpp"


MapSize::MapSize() = default;
void MapSize::setWidth(uint32_t newWidth) {
	this->width = newWidth;
}
void MapSize::setHeight(uint32_t newHeight) {
	this->height = newHeight;
}
uint32_t MapSize::getWidth() const {
	return this->width;
}
uint32_t MapSize::getHeight() const {
	return this->height;
}


BOOST_CLASS_EXPORT_IMPLEMENT(MapSize)