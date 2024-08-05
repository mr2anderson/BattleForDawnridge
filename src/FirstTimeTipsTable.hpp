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


#include <string>


#pragma once


class FirstTimeTipsTable {
public:
    static FirstTimeTipsTable* get() {
        if (FirstTimeTipsTable::singletone == nullptr) {
            FirstTimeTipsTable::singletone = new FirstTimeTipsTable();
        }
        return FirstTimeTipsTable::singletone;
    }

    bool wasDisplayed(const std::string& id) const;
    void markAsDisplayed(const std::string& id);
    void clear();
private:
    FirstTimeTipsTable() = default;
    FirstTimeTipsTable(const FirstTimeTipsTable& copy);
    static FirstTimeTipsTable* singletone;
};