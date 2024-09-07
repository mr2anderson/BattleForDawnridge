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



#include <unordered_map>
#include <string>


#pragma once


class Locales {
public:
    static Locales& get() {
        static Locales instance;
        return instance;
    }

    void load();
    void setDefaultPath(const std::string& path);
    std::wstring* get(const std::string& name);
    uint32_t totalMaleNames() const;
    uint32_t totalFemaleNames() const;
private:
    Locales();
    Locales(const Locales& copy);

    std::unordered_map<std::string, std::wstring> texts;
    std::wstring error;
    uint32_t maleNames, femaleNames;

    std::string getMainShortPath() const;

    void loadShortPaths(std::string &base, std::string &maleNames, std::string &femaleNames);
    void loadBase(const std::string &shortPath);
    void loadMaleNames(const std::string &shortPath);
    void loadFemaleNames(const std::string &shortPath);
};