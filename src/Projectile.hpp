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
#include "IWithTextureName.hpp"
#include "Clock.hpp"


#pragma once


class Projectile : public CameraDependentPopUpElement, public IWithTextureName {
public:
    Projectile();

    void setSrc(float xInPixels, float yInPixels);
    void setDst(float xInPixels, float yInPixels);
    void run(uint32_t windowW, uint32_t windowH) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    Events click() override;
    void update() override;
    virtual std::string getSoundName() const = 0;
    virtual float getTime() const = 0;
private:
    float x1, y1;
    float vx, vy;
    float rotation;
    Clock clock;
};