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


#include "Time.hpp"
#include "GlobalRandomGenerator32.hpp"
#include "Textures.hpp"


Time::Time() {
    this->type = static_cast<Type>(GlobalRandomGenerator32::get().gen() % TOTAL);
}
TimeMod Time::getTimeMod() const {
    switch (this->type) {
        case Type::Dawn:
            return NEUTRAL_TIME_MOD;
        case Type::Morning:
            return POSITIVE_TIME_MOD;
        case Type::Day:
            return POSITIVE_TIME_MOD;
        case Type::Sunset:
            return NEUTRAL_TIME_MOD;
        case Type::Night1:
            return NEGATIVE_TIME_MOD;
        case Type::Night2:
            return NEGATIVE_TIME_MOD;
    }
    return NEUTRAL_TIME_MOD;
}
Time::Type Time::getType() const {
    return this->type;
}

void Time::change() {
    this->type = static_cast<Type>((this->type + 1) % TOTAL);
}

std::optional<std::string> Time::getSoundName() const {
    switch (this->type) {
        case Type::Dawn:
            return "dawn";
        case Type::Morning:
            return "day";
        case Type::Day:
            return std::nullopt;
        case Type::Sunset:
            return "sunset";
        case Type::Night1:
            return "night";
        case Type::Night2:
            return std::nullopt;
    }
    return "";
}

std::string Time::getTextureName() const {
    switch (this->type) {
        case Type::Dawn:
            return "dawn";
        case Type::Morning:
            return "morning";
        case Type::Day:
            return "day";
        case Type::Sunset:
            return "sunset";
        case Type::Night1:
            return "night1";
        case Type::Night2:
            return "night2";
    }
    return "";
}

sf::Color Time::getEffectColor() const {
    switch (this->type) {
    case Type::Dawn:
        return sf::Color(79, 56, 45, 60);
    case Type::Morning:
        return sf::Color::Transparent;
    case Type::Day:
        return sf::Color::Transparent;
    case Type::Sunset:
        return sf::Color(64, 29, 42, 50);
    case Type::Night1:
        return sf::Color(0, 0, 0, 100);
    case Type::Night2:
        return sf::Color(0, 0, 0, 100);
    }
    return sf::Color();
}