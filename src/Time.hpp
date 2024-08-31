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


#include <boost/serialization/access.hpp>
#include <SFML/Graphics.hpp>
#include <optional>
#include "TimeMod.hpp"
#include "ArchiveType.hpp"


#pragma once


class Time {
public:
    Time();

    typedef enum Type {
        Dawn,
        Morning,
        Day,
        Sunset,
        Night1,
        Night2
    } Type;
    static const uint32_t TOTAL = 6;

    bool hasError() const {
        return this->type < 0 or this->type >= 6;
    }

    TimeMod getTimeMod() const;
    Type getType() const;

    void change();

    std::optional<std::string> getSoundName() const;
    std::string getTextureName() const;
    sf::Color getEffectColor() const;
private:
    Type type;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version) {
        ar & this->type;
    }
};