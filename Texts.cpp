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



#include "Texts.hpp"


Texts* Texts::singletone = nullptr;


void Texts::load(const std::string &path) {
    std::ifstream file(std::string(ROOT) + "/" + path);
    if (!file.is_open()) {
        throw CouldntOpenText(path);
    }

    std::string current;
    bool gettingId = false;
    bool gettingData = false;

    std::string currentId;
    std::wstring currentData;

    while (std::getline(file, current)) {
        if (current == "<ID>") {
            gettingId = true;
            currentId.clear();
            continue;
        }
        if (current == "<\\ID>") {
            gettingId = false;
            continue;
        }
        if (current == "<DATA>") {
            gettingData = true;
            currentData.clear();
            continue;
        }
        if (current == "<\\DATA>") {
            gettingData = false;
            if (!currentId.empty()) {
                currentId.pop_back();
            }
            if (!currentData.empty()) {
                currentData.pop_back();
            }
            if (this->texts.find(currentId) != this->texts.end()) {
                throw TextRedefinition(path, currentId);
            }
            this->texts[currentId] = currentData;
            
            continue;
        }

        if (gettingId) {
            currentId = currentId + current + '\n';
        }
        else if (gettingData) {
            currentData = currentData + UTFEncoder::get()->utf8ToUtf16(current + '\n');
        }
    }

    file.close();
}
std::wstring* Texts::get(const std::string& name) {
    return &this->texts[name];
}