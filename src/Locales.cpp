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


Locales::Locales() {
    this->error = UTFEncoder::get().utf8ToUtf16("UNKNOWN_LOCALES_KEY_REPORT_DEVELOPER");
    this->maleNames = 0;
    this->femaleNames = 0;
}
void Locales::load() {
    std::string basePath, maleNamesPath, femaleNamesPath;
    this->loadShortPaths(basePath, maleNamesPath, femaleNamesPath);

    this->loadBase(basePath);
    this->loadMaleNames(maleNamesPath);
    this->loadFemaleNames(femaleNamesPath);
}
void Locales::setDefaultPath(const std::string& path) {
    if (path == this->getMainShortPath()) {
        throw LanguageAlreadyInUse();
    }

    if (!std::filesystem::is_directory(Root::get().getUserdataRoot())) {
        std::filesystem::create_directories(Root::get().getUserdataRoot());
    }

    std::ofstream file(Root::get().getUserdataRoot() + "/language.cfg");
    file << path;
    file.close();
}
std::wstring* Locales::get(const std::string& name) {
    auto it = this->texts.find(name);
    if (it == this->texts.end()) {
        std::cerr << "Invalid text uid: " << name << std::endl;
        return &this->error;
    }
    return &it->second;
}
uint32_t Locales::totalMaleNames() const {
    return this->maleNames;
}
uint32_t Locales::totalFemaleNames() const {
    return this->femaleNames;
}
std::string Locales::getMainShortPath() const {
    std::ifstream file(Root::get().getUserdataRoot() + "/language.cfg");
    if (file.is_open()) {
        std::string path;
        std::getline(file, path);
        file.close();
        return path;
    }
    return "en.cfg";
}
void Locales::loadShortPaths(std::string &base, std::string &maleNames, std::string &femaleNames) {
    std::string path = Root::get().getDataRoot() + "/locales/" + this->getMainShortPath();
    std::ifstream file(path);
    if (!file.is_open()) {
        throw CouldntOpenLocales(path);
    }

    std::getline(file, base);
    std::getline(file, maleNames);
    std::getline(file, femaleNames);

    file.close();
}
void Locales::loadBase(const std::string &shortPath) {
    std::string path = Root::get().getDataRoot() + "/locales/" + shortPath;
    std::ifstream file(path);
    if (!file.is_open()) {
        throw CouldntOpenLocales(path);
    }

    std::string current;
    uint32_t ctr = 0;
    bool gettingId = false;
    bool gettingData = false;

    std::string currentId;
    std::wstring currentData;

    while (std::getline(file, current)) {
        ctr = ctr + 1;
        if (current == "<ID>") {
            if (gettingData) {
                std::cerr << "Locales: syntax error line " << ctr << std::endl;
                throw CouldntOpenLocales(path);
            }
            gettingId = true;
            currentId.clear();
            continue;
        }
        if (current == "<\\ID>") {
            gettingId = false;
            continue;
        }
        if (current == "<DATA>") {
            if (gettingId) {
                std::cerr << "Locales: syntax error line " << ctr << std::endl;
                throw CouldntOpenLocales(path);
            }
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
            currentData = currentData + UTFEncoder::get().utf8ToUtf16(current + '\n');
        }
    }

    file.close();
}
void Locales::loadMaleNames(const std::string &shortPath) {
    std::string path = Root::get().getDataRoot() + "/locales/" + shortPath;
    std::ifstream file(path);
    if (!file.is_open()) {
        throw CouldntOpenLocales(path);
    }

    std::string buff;
    while (std::getline(file, buff)) {
        this->texts["male_" + std::to_string(this->maleNames)] = UTFEncoder::get().utf8ToUtf16(buff);
        this->maleNames = this->maleNames + 1;
    }

    if (this->maleNames == 0) {
        throw CouldntOpenLocales(path);
    }

    file.close();
}
void Locales::loadFemaleNames(const std::string &shortPath) {
    std::string path = Root::get().getDataRoot() + "/locales/" + shortPath;
    std::ifstream file(path);
    if (!file.is_open()) {
        throw CouldntOpenLocales(path);
    }

    std::string buff;
    while (std::getline(file, buff)) {
        this->texts["female_" + std::to_string(this->femaleNames)] = UTFEncoder::get().utf8ToUtf16(buff);
        this->femaleNames = this->femaleNames + 1;
    }

    if (this->femaleNames == 0) {
        throw CouldntOpenLocales(path);
    }

    file.close();
}