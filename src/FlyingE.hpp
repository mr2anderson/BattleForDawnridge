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


class FlyingE : public CameraDependentPopUpElement {
public:
    FlyingE();
    FlyingE(float x, float y);

    void onRestart() override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void update() override;
protected:
    virtual std::unique_ptr<sf::Drawable> getDrawable(sf::Vector2f position, sf::Color color) const = 0;
private:
    float startX, startY;
	Clock clock;

    sf::Color getTransparencyLevel() const;
    sf::Vector2f getPosition() const;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive& ar, const unsigned int version) {
        ar& boost::serialization::base_object<CameraDependentPopUpElement>(*this);
        ar& this->startX;
        ar& this->startY;
        ar& this->clock;
    }
};


BOOST_CLASS_EXPORT_KEY(FlyingE)