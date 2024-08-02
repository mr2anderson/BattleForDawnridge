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


#include "NewMovePriority.hpp"
#include "GO.hpp"


NewMovePriority::NewMovePriority(uint8_t mainPriority, const GO *type) {
    this->mainPriority = mainPriority;
    this->typeHashCode = typeid(*type).hash_code();
}
bool NewMovePriority::operator>(const NewMovePriority &b) {
    if (this->mainPriority > b.mainPriority) {
        return true;
    }
    if (this->mainPriority < b.mainPriority) {
        return false;
    }
    return this->typeHashCode > b.typeHashCode;
}