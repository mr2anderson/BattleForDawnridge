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


#include "HolyMagick.hpp"
#include "GlobalRandomGenerator32.hpp"


const uint32_t HolyMagick::TOTAL_TEXTURES = 7;
const uint32_t HolyMagick::TOTAL_SOUNDS = 4;


HolyMagick::HolyMagick() = default;
std::string HolyMagick::getTextureName() const {
    return "holy_magick" + std::to_string(std::min(HolyMagick::TOTAL_TEXTURES, (uint32_t)(this->getMS() / ((uint32_t)(1000 * this->getTime()) / HolyMagick::TOTAL_TEXTURES) + 1)));
}
std::string HolyMagick::getSoundName() const {
    return "holy_magick" + std::to_string(GlobalRandomGenerator32::get().gen() % HolyMagick::TOTAL_SOUNDS + 1);
}
float HolyMagick::getTime() const {
    return 0.5f;
}


BOOST_CLASS_EXPORT_IMPLEMENT(HolyMagick)