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



#include <cstdint>
#include <vector>
#include <fstream>
#include "Texts.hpp"
#include "Root.hpp"
#include "UTFEncoder.hpp"
#include "CouldntOpenText.hpp"


Texts* Texts::singletone = nullptr;


void Texts::load() {
    std::string path = this->getPath();
    std::ifstream file(path);
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
void Texts::setDefaultPath(const std::string& path) {
    std::ofstream file(ROOT + "/language.txt");
    file << ROOT << "/" << path;
    file.close();
}
std::wstring* Texts::get(const std::string& name) {
    return &this->texts.at(name);
}
std::string Texts::getPath() const {
    std::ifstream file(ROOT + "/language.txt");
    if (file.is_open()) {
        std::string path;
        std::getline(file, path);
        file.close();
        return path;
    }
    return ROOT + "/en.txt";
}