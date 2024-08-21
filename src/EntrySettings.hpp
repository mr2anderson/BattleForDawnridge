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


#include <SFML/Window.hpp>
#include <string>
#include <cstdint>
#include <unordered_map>
#include <memory>


#pragma once


class EntrySettings {
public:
    EntrySettings();

    void setMaxLen(uint32_t newMaxLen);
    void setRequireMaxLen(bool newReguireMaxLen);

    bool ok(std::shared_ptr<std::string> dstPtr) const;

    void reg(sf::Keyboard::Key key, char c);

    void regDigits();

    typedef enum {
        AS_UPPER_CASE,
        AS_LOWER_CASE
    } LETTER_REGISTRATION_TYPE;
    void regLetters(LETTER_REGISTRATION_TYPE type);

    void mod(std::shared_ptr<std::string> dstPtr, sf::Keyboard::Key key);
private:
    uint32_t maxLen;
    bool requireMaxLen;
    std::unordered_map<uint32_t, char> table;
};