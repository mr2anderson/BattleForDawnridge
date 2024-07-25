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


#include <iostream>
#include "ResourceOrderPriorityTable.hpp"


ResourceOrderPriorityTable *ResourceOrderPriorityTable::singletone = nullptr;


ResourceOrderPriorityTable::ResourceOrderPriorityTable() {
    this->data["food"] = 0;
    this->data["wood"] = 1;
    this->data["stone"] = 2;
    this->data["iron"] = 3;
    this->data["crystal"] = 4;
    this->data["gold"] = 5;
}
uint32_t ResourceOrderPriorityTable::getPriority(const std::string &id) const {
    if (this->data.find(id) == this->data.end()) {
        std::cerr << "ResourceOrderPriorityTable: Warning: Unknown id: " << id << std::endl;
        return 0;
    }
    return this->data.at(id);
}