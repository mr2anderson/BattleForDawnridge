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
#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include "Root.hpp"
#include "UTFEncoder.hpp"
#include "CouldntOpenText.hpp"
#include "TextRedefinition.hpp"


#pragma once


class Texts {
public:
    static Texts* get() {
        if (Texts::singletone == nullptr) {
            Texts::singletone = new Texts();
        }
        return Texts::singletone;
    }

    void load(const std::string& path);
    [[nodiscard]] std::wstring* get(const std::string& name);
private:
    Texts() = default;
    Texts(const Texts& copy);
    static Texts* singletone;

    std::unordered_map<std::string, std::wstring> texts;
};