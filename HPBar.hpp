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



#include <SFML/Graphics.hpp>


#pragma once


class HPBar : public sf::Drawable {
public:
	HPBar();
	HPBar(uint32_t current, uint32_t max, uint32_t x, uint32_t y, uint32_t sx, uint32_t sy);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	uint32_t getCurrent() const;
	uint32_t getMax() const;
	void changeCurrent(uint32_t newCurrent);
	void changeMax(uint32_t newMax);
	void setMax();
	friend HPBar operator+(HPBar a, uint32_t b);
	friend HPBar operator-(HPBar a, uint32_t b);
	std::wstring getReadableInfo() const;
	uint32_t getX() const;
	uint32_t getY() const;
	uint32_t getSX() const;
	uint32_t getSY() const;
private:
	uint32_t current, max;
	uint32_t x, y;
	uint32_t sx, sy;
};