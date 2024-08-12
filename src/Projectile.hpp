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


class Projectile : public CameraDependentPopUpElement {
public:
    Projectile();

    void onRestart() override;
    void setSrc(float xInPixels, float yInPixels);
    void setDst(float xInPixels, float yInPixels);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void update() override;
    virtual std::string getTextureName() const = 0;
    virtual std::string getSoundName() const = 0;
    virtual float getTime() const = 0;
private:
    float x1, y1;
    float vx, vy;
    float rotation;
    Clock clock;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive& ar, const unsigned int version) {
        ar& boost::serialization::base_object<CameraDependentPopUpElement>(*this);
        ar& this->x1;
        ar& this->y1;
        ar& this->vx;
        ar& this->vy;
        ar& this->rotation;
        ar& this->clock;
    }
};


BOOST_CLASS_EXPORT_KEY(Projectile)