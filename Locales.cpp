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
#include <iostream>
#include <filesystem>
#include "Locales.hpp"
#include "Root.hpp"
#include "UTFEncoder.hpp"
#include "CouldntOpenLocales.hpp"
#include "LanguageAlreadyInUse.hpp"


Locales* Locales::singletone = nullptr;


static const std::string LOCAL_ROOT = DATA_ROOT + "/locales";


void Locales::load() {
    std::string path = LOCAL_ROOT + "/" + this->getPath();
    std::ifstream file(path);
    if (!file.is_open()) {
        throw CouldntOpenLocales(path);
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
void Locales::setDefaultPath(const std::string& path) {
    if (path == this->getPath()) {
        throw LanguageAlreadyInUse();
    }

    if (!std::filesystem::is_directory(USERDATA_ROOT)) {
        std::filesystem::create_directories(USERDATA_ROOT);
    }

    std::ofstream file(USERDATA_ROOT + "/language.cfg");
    file << path;
    file.close();
}
std::wstring* Locales::get(const std::string& name) {
    auto it = this->texts.find(name);
    if (it == this->texts.end()) {
        std::cerr << "Invalid text uid: " << name << std::endl;
    }
    return &it->second;
}
std::string Locales::getPath() const {
    std::ifstream file(USERDATA_ROOT + "/language.cfg");
    if (file.is_open()) {
        std::string path;
        std::getline(file, path);
        file.close();
        return path;
    }
    return "en.cfg";
}