/*
 *  Conquesta
 *  Copyright (C) 2024 mr2anderson
 *
 *  Conquesta is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Conquesta is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Conquesta.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "Stone.hpp"


Stone::Stone() = default;
Stone::Stone(uint32_t x, uint32_t y) : ResourcePoint(x, y, 2, 2, 10000) {

}
std::string Stone::getResourceType() const {
	return "stone";
}
std::string Stone::getSoundName() const {
	return "stone";
}
std::string Stone::getTextureName() const {
	return "stone";
}
std::wstring Stone::getDescription() const {
	return L"Этот камень содержит еще " + std::to_wstring(this->getHP()) + L" ед. камня. Постройте рядом каменоломню, чтобы начать добычу.";
}