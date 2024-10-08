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
#include <optional>


#pragma once


class Root {
public:
    static Root& get() {
        static Root instance;
        return instance;
    }

    void setDataRoot(const std::string &newDataRoot);
    void setUserdataRoot(const std::string &newUserdataRoot);

    std::string getDataRoot() const;
    std::string getUserdataRoot() const;
private:
    Root();
    Root(const Root& copy);

    std::optional<std::string> dataRoot;
    std::optional<std::string> userDataRoot;
};