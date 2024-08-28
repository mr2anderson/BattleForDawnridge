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


#include <limits>
#include "EntryStrSettings.hpp"


EntryStrSettings::EntryStrSettings() {
    this->minLen = 0;
    this->maxLen = std::numeric_limits<uint32_t>::max();
}
void EntryStrSettings::setMinLen(uint32_t newMinLen) {
    this->minLen = newMinLen;
}
void EntryStrSettings::setMaxLen(uint32_t newMaxLen) {
    this->maxLen = newMaxLen;
}
bool EntryStrSettings::ok(std::shared_ptr<std::string> dstPtr) const {
    return (dstPtr->size() >= this->minLen and dstPtr->size() <= this->maxLen);
}
void EntryStrSettings::reg(sf::Keyboard::Key key, char c) {
    this->table[key] = c;
}
void EntryStrSettings::regDigits() {
    this->reg(sf::Keyboard::Key::Num0, '0');
    this->reg(sf::Keyboard::Key::Num1, '1');
    this->reg(sf::Keyboard::Key::Num2, '2');
    this->reg(sf::Keyboard::Key::Num3, '3');
    this->reg(sf::Keyboard::Key::Num4, '4');
    this->reg(sf::Keyboard::Key::Num5, '5');
    this->reg(sf::Keyboard::Key::Num6, '6');
    this->reg(sf::Keyboard::Key::Num7, '7');
    this->reg(sf::Keyboard::Key::Num8, '8');
    this->reg(sf::Keyboard::Key::Num9, '9');
}
void EntryStrSettings::regLetters(LETTER_REGISTRATION_TYPE type) {
    for (uint8_t i = 0; i < 26; i = i + 1) {
        char c;
        if (type == LETTER_REGISTRATION_TYPE::AS_LOWER_CASE) {
            c = 'a' + i;
        }
        else if (type == LETTER_REGISTRATION_TYPE::AS_UPPER_CASE) {
            c = 'A' + i;
        }
        this->reg((sf::Keyboard::Key)((uint32_t)sf::Keyboard::Key::A + i), c);
    }
}
void EntryStrSettings::mod(std::shared_ptr<std::string> dstPtr, sf::Keyboard::Key key) {
    if (!dstPtr->empty() and key == sf::Keyboard::BackSpace) {
        dstPtr->pop_back();
        return;
    }
    if (dstPtr->size() >= this->maxLen) {
        return;
    }
    auto it = this->table.find(key);
    if (it == this->table.end()) {
        return;
    }
    dstPtr->push_back(it->second);
}