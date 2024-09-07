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


#include "CameraDependentPopUpElement.hpp"
#include "Clock.hpp"


#pragma once


class Magick : public CameraDependentPopUpElement {
public:
    Magick();

    void onRestart() override;
    void setTarget(float xInPixels, float yInPixels);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    Events update() override;
    virtual std::string getTextureName() const = 0;
    virtual std::string getSoundName() const = 0;
    virtual float getTime() const = 0;
protected:
    uint32_t getMS() const;
private:
    float x, y;
    Clock clock;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive& ar, const unsigned int version) {
        ar & boost::serialization::base_object<CameraDependentPopUpElement>(*this);
        ar & this->x;
        ar & this->y;
        ar & this->clock;
    }
};


BOOST_CLASS_EXPORT_KEY(Magick)