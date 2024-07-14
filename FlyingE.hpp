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


#pragma once


class FlyingE : public CameraDependentPopUpElement {
public:
	FlyingE();

	void run(uint32_t windowW, uint32_t windowH) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	Events click() override;
	void update() override;
protected:
    void setSprite(const sf::Sprite &sprite1);
private:
    sf::Sprite sprite;
    float spriteStartX, spriteStartY;
	sf::Clock clock;
	float dst;

	static const float TIME;
	static const float V0;

	sf::Color getTransparencyLevel(float t) const;
	sf::Vector2f getPosition(float t) const;
};