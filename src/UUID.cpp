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


#include "UUID.hpp"
#include "GlobalRandomGenerator64.hpp"
#include "ColorTheme.hpp"


UUID::UUID() {
    uint128_t int64left = GlobalRandomGenerator64::get().gen();
    uint128_t int64right = GlobalRandomGenerator64::get().gen();
    uint128_t randomInt = (int64left << 64) | int64right;
    this->a = randomInt;
}
bool UUID::operator==(const UUID &b) const {
    return this->a == b.a;
}
bool UUID::operator!=(const UUID &b) const {
    return this->a != b.a;
}
bool UUID::operator<(const UUID &b) const {
    return this->a < b.a;
}
bool UUID::operator>(const UUID &b) const {
    return this->a > b.a;
}
StringLcl UUID::toString() const {
    std::string result;
    uint128_t buff = this->a;
    while (buff) {
        uint8_t c = (buff % 36).convert_to<uint8_t>();
        buff = buff / 36;
        if (c >= 0 and c <= 9) {
            result.push_back('0' + c);
        }
        else {
            result.push_back('a' + (c - 10));
        }
    }
    return StringLcl::COLOR(COLOR_THEME::STATE_COLOR_NEUTRAL) + result;
}
uint128_t UUID::value() const {
    return this->a;
}