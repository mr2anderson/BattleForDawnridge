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


#include "Tree.hpp"


Tree::Tree(uint32_t x, uint32_t y) : ResourcePoint(x, y, 10000) {
	std::random_device rd;
	std::mt19937 mersenne(rd());
	this->type = mersenne() % TOTAL_TYPES;
}
std::string Tree::getResourceType() const {
	return "wood";
}
std::string Tree::getTextureName() const {
	return "tree" + std::to_string(this->type);
}
std::string Tree::getIconName() const {
	return this->getTextureName();
}
std::wstring Tree::getDescription() const {
	return L"Это дерево содержит еще " + std::to_wstring(this->getHP()) + L" ед. древесины. Постройте рядом лесопилку, чтобы начать добычу.";
}
std::string Tree::getClickSoundName() const {
	return "sawmill";
}