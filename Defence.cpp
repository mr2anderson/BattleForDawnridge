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


#include "Defence.hpp"
#include "Texts.hpp"
#include "UTFEncoder.hpp"


Defense::Defense(uint32_t points, float cut, float stub, float crush) : HP(points) {
    this->cut = cut;
    this->stub = stub;
    this->crush = crush;
}
void Defense::process(Damage damage) {
    switch (damage.type) {
        case Damage::CUT: {
            this->HP::process(damage.points * this->cut);
            break;
        }
        case Damage::STUB: {
            this->HP::process(damage.points * this->stub);
            break;
        }
        case Damage::CRUSH: {
            this->HP::process(damage.points * this->crush);
            break;
        }
    }
}
std::wstring Defense::getReadableDescription() const {
    std::wstring result;

    result = *Texts::get()->get("points") + UTFEncoder::get()->utf8ToUtf16(std::to_string(this->getPoints()) + "(");
    result += *Texts::get()->get("cut") + UTFEncoder::get()->utf8ToUtf16(std::to_string(this->cut) + " ");
    result += *Texts::get()->get("stub") + UTFEncoder::get()->utf8ToUtf16(std::to_string(this->stub) + " ");
    result += *Texts::get()->get("crush") + UTFEncoder::get()->utf8ToUtf16(std::to_string(this->crush) + ")");

    return result;
}