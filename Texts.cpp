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
#include <iostream>


Texts* Texts::singletone = nullptr;


void Texts::load(const std::string &path) {
    std::ifstream file(std::string(ROOT) + "/" + path);
    if (!file.is_open()) {
        std::cerr << "Invalid file" << std::endl;
        return;
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
                std::cerr << "Texts: warning: id '" << currentId << "' already exist. Redefinition\n";
            }
            this->texts[currentId] = currentData;
            
            continue;
        }

        if (gettingId) {
            currentId = currentId + current + '\n';
        }
        else if (gettingData) {
            currentData = currentData + UTF8_TO_UTF16(current + '\n');
        }
    }

    file.close();
}
std::wstring* Texts::get(const std::string& name) {
    if (this->texts.find(name) == this->texts.end()) {
        std::cerr << "Texts: id '" << name << "' does not exit\n";
    }
    return &this->texts[name];
}
std::wstring Texts::UTF8_TO_UTF16(const std::string& utf8)
{
    std::vector<unsigned long> unicode;
    size_t i = 0;
    while (i < utf8.size())
    {
        unsigned long uni;
        size_t todo;
        bool error = false;
        unsigned char ch = utf8[i++];
        if (ch <= 0x7F)
        {
            uni = ch;
            todo = 0;
        }
        else if (ch <= 0xBF)
        {
            throw std::logic_error("not a UTF-8 string");
        }
        else if (ch <= 0xDF)
        {
            uni = ch & 0x1F;
            todo = 1;
        }
        else if (ch <= 0xEF)
        {
            uni = ch & 0x0F;
            todo = 2;
        }
        else if (ch <= 0xF7)
        {
            uni = ch & 0x07;
            todo = 3;
        }
        else
        {
            throw std::logic_error("not a UTF-8 string");
        }
        for (size_t j = 0; j < todo; ++j)
        {
            if (i == utf8.size())
                throw std::logic_error("not a UTF-8 string");
            unsigned char ch = utf8[i++];
            if (ch < 0x80 || ch > 0xBF)
                throw std::logic_error("not a UTF-8 string");
            uni <<= 6;
            uni += ch & 0x3F;
        }
        if (uni >= 0xD800 && uni <= 0xDFFF)
            throw std::logic_error("not a UTF-8 string");
        if (uni > 0x10FFFF)
            throw std::logic_error("not a UTF-8 string");
        unicode.push_back(uni);
    }
    std::wstring utf16;
    for (size_t i = 0; i < unicode.size(); ++i)
    {
        unsigned long uni = unicode[i];
        if (uni <= 0xFFFF)
        {
            utf16 += (wchar_t)uni;
        }
        else
        {
            uni -= 0x10000;
            utf16 += (wchar_t)((uni >> 10) + 0xD800);
            utf16 += (wchar_t)((uni & 0x3FF) + 0xDC00);
        }
    }
    return utf16;
}