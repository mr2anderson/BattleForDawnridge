/*
 *  Conquesta
 *  Copyright (C) 2024 mr2anderson
 *
 *  Conquesta is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Conquesta is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Conquesta.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <cmath>
#include "PopUpElement.hpp"
#include "Textures.hpp"


#pragma once


class FlyingE : public PopUpElement {
public:
	FlyingE(const std::string& picture, const std::string& sound, uint32_t x, uint32_t y, uint32_t sx, uint32_t sy);

	Events run(uint32_t windowW, uint32_t windowH) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	Events click(uint32_t x, uint32_t y) override;
	void update() override;
private:
	std::string picture;
	std::string sound;
	uint32_t x, y;
	uint32_t sx, sy;
	sf::Clock clock;
	float dst;

	static constexpr float TIME = 0.75;
	static constexpr float V0 = 80;

	sf::Color getTransparencyLevel(float t) const;
	sf::Vector2f getPosition(float t) const;
};